/* @file: c_stddef.h
 * #desc:
 *    The definitions of standard type.
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

#ifndef _CONCH_C_STDDEF_H
#define _CONCH_C_STDDEF_H

#include <conch/config.h>


/* nil pointer */
#undef NULL
#ifdef __cplusplus
# define NULL (nullptr)
#else
# define NULL ((void *)0)
#endif

#ifdef CONCH_STDDEF_TYPEDEF
 typedef unsigned long int size_t;
 typedef signed long int ptrdiff_t;
 /* posix */
 typedef signed long int ssize_t;
#else
# define size_t unsigned long int
# define ptrdiff_t signed long int
# define ssize_t signed long int
#endif

/* struct member offset size */
#undef offsetof
#define offsetof(type, member) \
	((size_t)&(((type *)0)->member))

/* struct member and container offset size */
#undef container_of
#define container_of(ptr, type, member) \
	((type *)((char *)(ptr) - offsetof(type, member)))


#endif
