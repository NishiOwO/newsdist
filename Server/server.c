/**
 * $Id$
 * SPDX-License-Identifier: Unlicense
 */

#include "newsdist.h"

#include "../config.h"

#ifdef HAS_WINSOCK
#include <winsock2.h>
#else
#endif

int
nd_init_server(void)
{
#ifdef HAS_WINSOCK
	WSADATA		wsa;

	WSAStartup(MAKEWORD(2, 0), &wsa);
#endif
	return 0;
}
