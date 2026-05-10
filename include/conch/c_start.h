/* @file: c_start.h
 * #desc:
 *    The definitions of c runtime entry.
 *
 * #copy:
 *    Copyright (C) 1970 Public Free Software.
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 2.1 of the License, or (at your option) any later version.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with this library; if not,
 *    see <https://www.gnu.org/licenses/>.
 */

#ifndef _CONCH_C_START_H
#define _CONCH_C_START_H

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* c_start_main.c */
extern
char **__conch_envp
;
extern
char **__conch_auxp
;
extern
void _start_main(long *sp)
;

/* c_start_vdso.c */
extern
void *conch_vdso_sym(const char *sym)
;

#ifdef __cplusplus
}
#endif


#endif
