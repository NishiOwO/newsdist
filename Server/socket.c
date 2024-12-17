/**
 * $Id$
 * SPDX-License-Identifier: Unlicense
 */

#include "newsdist.h"

#include "../config.h"

#include <stddef.h>

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

const char *
nd_get_ssl_version(void)
{
#ifdef HAS_SSL
	return nd_sslver;
#else
	return NULL;
#endif
}
