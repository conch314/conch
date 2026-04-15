/* @file: c_string_strdup.c
 * #desc:
 *    The implementations of string operations.
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

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_string.h>
#include <conch/c_stdlib.h>


/* @func: conch_strndup
 * #desc:
 *    duplicate a string.
 *
 * #1: s   [in]  input string
 * #2: len [in]  input length
 * #r:     [ret] returns a new string
 */
char *conch_strndup(const char *s, size_t len)
{
	char *p = conch_malloc(len + 1);
	p[len] = '\0';

	conch_memcpy(p, s, len);

	return p;
}

/* @func: conch_strdup
 * #desc:
 *    duplicate a string.
 *
 * #1: s [in]  input string
 * #r:   [ret] returns a new string
 */
char *conch_strdup(const char *s)
{
	return conch_strndup(s, conch_strlen(s));
}
