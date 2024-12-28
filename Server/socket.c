/**
 * $Id$
 */

#include "newsdist.h"

#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef HAS_OPENSSL
#include <openssl/opensslv.h>
#include <openssl/ssl.h>
const char     *nd_sslver = OPENSSL_VERSION_TEXT;
#endif

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

void
nd_create_ssl(nd_pass_t *pass)
{
	const void     *method;

	pass->ssl = malloc(sizeof(*pass->ssl));
#ifdef HAS_OPENSSL
	method = nd_create_method();
	if (method == NULL) {
		free(pass->ssl);
		pass->ssl = NULL;
		return;
	}
	pass->ssl->ctx = SSL_CTX_new(method);
	pass->ssl->ssl = SSL_new(pass->ssl->ctx);
	SSL_use_PrivateKey_file(pass->ssl->ssl, "", SSL_FILETYPE_PEM);
	SSL_use_certificate_file(pass->ssl->ssl, "", SSL_FILETYPE_PEM);
	SSL_set_fd(pass->ssl->ssl, pass->sock);
	if (SSL_accept(pass->ssl->ssl) <= 0) {
		SSL_free(pass->ssl->ssl);
		free(pass->ssl);
		pass->ssl = NULL;
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
