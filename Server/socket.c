/**
 * $Id$
 * SPDX-License-Identifier: Unlicense
 */

#include "newsdist.h"

#include "../config.h"

#include <stddef.h>

#ifdef HAS_OPENSSL
#include <openssl/opensslv.h>
#endif

char *
nd_get_ssl(void)
{
#ifdef HAS_SSL
#ifdef HAS_OPENSSL
	return OPENSSL_VERSION_TEXT;
#else
	return NULL;
#endif
#else
	return NULL;
#endif
}
