/**
 * $Id$
 */

#include "newsdist.h"

#include <stddef.h>
#include <stdlib.h>

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
	SSL_set_fd(pass->ssl->ssl, pass->sock);
	SSL_use_PrivateKey_file(pass->ssl->ssl, "", SSL_FILETYPE_PEM);
	SSL_use_certificate_file(pass->ssl->ssl, "", SSL_FILETYPE_PEM);
#endif
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
