/* @file: c_stdio.h
 * #desc:
 *    The implementations of standard input/output.
 *
 * #copy:
 *    Copyright (C) 1970 Public Free Software
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

#ifndef _CONCH_C_STDIO_H
#define _CONCH_C_STDIO_H

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>
#include <conch/c_stdarg.h>


#undef EOF
#define EOF (-1)


#ifdef __cplusplus
extern "C" {
#endif

/* c_stdio__printf.c */
extern
int32_t __conch_printf(const char *fmt, va_list ap, void *arg,
		int32_t (*out)(const char *, int32_t, void *))
;

/* c_stdio__scanf.c */
extern
int32_t __conch_scanf(const char *s, char **e, const char *fmt,
		va_list ap)
;

#ifdef __cplusplus
}
#endif


#endif
