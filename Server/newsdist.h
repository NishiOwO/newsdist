/**
 * $Id$
 * SPDX-License-Identifier: Unlicense
 */

#ifndef __NEWSDIST_H__
#define __NEWSDIST_H__

/* log.c */
void		nd_init_log(void);
void		nd_log_info(const char *info);
void		nd_log_notice(const char *info);

/* server.c */
int		nd_init_server(void);

#endif
