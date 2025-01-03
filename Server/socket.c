/**
 * $Id$
 */

#define INCLUDE_ERRNO
#define INCLUDE_SOCKET

#include "newsdist.h"

#include <stddef.h>
#include <string.h>
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
		return SSL_read(pass->ssl->ssl, buffer, size) <= 0 ? -1 : 0;
#else
		return -1;
#endif
	} else {
		return recv(pass->sock, buffer, size, 0) <= 0 ? -1 : 0;
	}
}

int
nd_write(nd_pass_t *pass, void *buffer, int size)
{
	if (pass->do_ssl) {
#ifdef HAS_OPENSSL
		return SSL_write(pass->ssl->ssl, buffer, size) <= 0 ? -1 : 0;
#else
		return -1;
#endif
	} else {
		return send(pass->sock, buffer, size, 0) <= 0 ? -1 : 0;
	}
}

int
nd_write_string(nd_pass_t *pass, const char *string)
{
	return nd_write(pass, (void *)string, strlen(string));
}

int
nd_timeout(nd_pass_t *pass, int time)
{
	int		result;

	while (1) {
#if defined(HAS_POLL)
		struct pollfd	pfd;

		pfd.fd = pass->sock;
		pfd.events = POLLIN | POLLPRI;
		result = poll(&pfd, 1, time);
#elif defined(HAS_SELECT)
		fd_set		fdset;
		struct timeval	tv;

		tv.tv_sec = time / 1000;
		tv.tv_usec = time % 1000;
		FD_ZERO(&fdset);
		FD_SET(pass->sock, &fdset);
		result = select(FD_SETSIZE, &fdset, NULL, NULL, &tv);
#endif
		if (result < 0) {
			if (errno == EINTR)
				continue;
		}
		break;
	}
	return (result > 0) ? 0 : -1;
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
