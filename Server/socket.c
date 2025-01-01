
/**
 * $Id$
 */

#define INCLUDE_SOCKET

#include "newsdist.h"

#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef HAS_OPENSSL
#include <openssl/opensslv.h>
#include <openssl/ssl.h>
const char     *nd_sslver = OPENSSL_VERSION_TEXT;

SSL_CTX	       *openssl_ctx;
#endif

int
nd_init_ssl(void)
{
	const void     *method;

#ifdef HAS_SSL
	if (ssl_key == NULL || ssl_cert == NULL) {
		nd_log_info
			("SSLKey and/or SSLCertificate not specified, SSL not used");
		return 0;
	}
#endif
#ifdef HAS_OPENSSL
	method = nd_create_method();

	SSL_load_error_strings();
	SSL_library_init();
	OpenSSL_add_all_algorithms();

	if ((openssl_ctx = SSL_CTX_new(method)) == NULL)
		return -1;
	if (SSL_CTX_use_PrivateKey_file
	    (openssl_ctx, ssl_key, SSL_FILETYPE_PEM)
	    <= 0) {
		SSL_CTX_free(openssl_ctx);
		return -1;
	}
	if (SSL_CTX_use_certificate_file
	    (openssl_ctx, ssl_cert, SSL_FILETYPE_PEM)
	    <= 0) {
		SSL_CTX_free(openssl_ctx);
		return -1;
	}
	return 0;
#else
	return 0;
#endif
}

const void *
nd_create_method(void)
{
#ifdef HAS_OPENSSL
#if defined(HAS_TLS_SERVER_METHOD)
	return TLS_server_method();
#elif defined(HAS_SSLV23_SERVER_METHOD)
	return SSLv23_server_method();
#elif defined(HAS_TLSV1_2_SERVER_METHOD)
	return TLSv1_2_server_method();
#elif defined(HAS_TLSV1_1_SERVER_METHOD)
	return TLSv1_1_server_method();
#elif defined(HAS_SSLV3_SERVER_METHOD)
	return SSLv3_server_method();
#elif defined(HAS_SSLV2_SERVER_METHOD)
	return SSLv2_server_method();
#else
	return NULL;
#endif
#else
	return NULL;
#endif
}

int
nd_read(nd_pass_t *pass, void *buffer, int size)
{
	if (pass->do_ssl) {
#ifdef HAS_OPENSSL
		return SSL_read(pass->ssl->ssl, buffer, size);
#else
		return -1;
#endif
	} else {
		return recv(pass->sock, buffer, size, 0);
	}
}

int
nd_write(nd_pass_t *pass, void *buffer, int size)
{
	if (pass->do_ssl) {
#ifdef HAS_OPENSSL
		return SSL_write(pass->ssl->ssl, buffer, size);
#else
		return -1;
#endif
	} else {
		return send(pass->sock, buffer, size, 0);
	}
}

void
nd_create_ssl(nd_pass_t *pass)
{

	pass->ssl = malloc(sizeof(*pass->ssl));
#ifdef HAS_OPENSSL
	pass->ssl->ssl = SSL_new(openssl_ctx);
	SSL_set_fd(pass->ssl->ssl, pass->sock);
	if (SSL_accept(pass->ssl->ssl) <= 0) {
		SSL_free(pass->ssl->ssl);
		free(pass->ssl);
		pass->ssl = NULL;
		nd_log_notice("SSL failure");
	}
#endif
}

void
nd_close_socket(nd_pass_t *pass)
{
	if (pass->do_ssl) {
#ifdef HAS_OPENSSL
		SSL_shutdown(pass->ssl->ssl);
		SSL_free(pass->ssl->ssl);
#endif
		free(pass->ssl);
		pass->ssl = NULL;
		pass->do_ssl = 0;
	}
	CLOSE_SOCKET(pass->sock);
}

int
nd_accept_ssl(nd_pass_t *pass)
{
	if (pass->do_ssl) {
		nd_create_ssl(pass);
		if (pass->ssl == NULL) {
			return -1;
		}
	}
	return 0;
}

const char *
nd_get_ssl_version(void)
{
#ifdef HAS_SSL
	return nd_sslver;
#else
	return NULL;
#endif
}
