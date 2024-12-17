/**
 * $Id$
 * SPDX-License-Identifier: Unlicense
 */

#include "newsdist.h"

#include "../config.h"

#include <stddef.h>

#ifdef HAS_OPENSSL
#include <openssl/opensslv.h>
const char     *nd_sslver = OPENSSL_VERSION_TEXT;
#endif

const char *
nd_get_ssl_version(void)
{
#ifdef HAS_SSL
	return nd_sslver;
#else
	return NULL;
#endif
}
