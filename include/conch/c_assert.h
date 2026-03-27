/* @file: c_assert.h
 * #desc:
 *    The definitions of verify program assertion.
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

#ifndef _CONCH_C_ASSERT_H
#define _CONCH_C_ASSERT_H

#include <conch/config.h>


#undef xassert
#ifdef NDEBUG
# define xassert(x) ((void)0)
#else
# define xassert(x) ((void)((x) \
	|| (___assert_crash(#x, __FILE__, __LINE__, __func__), 0)))
#endif


inline
static void ___assert_crash(const char *expr, const char *file,
		long line, const char *func) {
	volatile char *p = (char *)0;
	*p = 0;

	(void)expr;
	(void)file;
	(void)line;
	(void)func;
}


#endif
