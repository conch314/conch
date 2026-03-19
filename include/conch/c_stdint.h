/* @file: c_stdint.h
 * #desc:
 *    The definitions of integer types.
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

#ifndef _CONCH_C_STDINT_H
#define _CONCH_C_STDINT_H

#include <conch/config.h>


#ifdef CONCH_STDINT_TYPEDEF
 typedef signed char int8_t;
 typedef signed short int16_t;
 typedef signed int int32_t;
 typedef unsigned char uint8_t;
 typedef unsigned short uint16_t;
 typedef unsigned int uint32_t;
#else
# define int8_t signed char
# define int16_t signed short
# define int32_t signed int
# define uint8_t unsigned char
# define uint16_t unsigned short
# define uint32_t unsigned int
#endif

#undef INT8_MIN
#undef INT8_MAX
#undef UINT8_MAX
#define INT8_MIN (-0x80)
#define INT8_MAX 0x7f
#define UINT8_MAX 0xff

#undef INT16_MIN
#undef INT16_MAX
#undef UINT16_MAX
#define INT16_MIN (-0x8000)
#define INT16_MAX 0x7fff
#define UINT16_MAX 0xffff

#undef INT32_MIN
#undef INT32_MAX
#undef UINT32_MAX
#define INT32_MIN (-0x80000000)
#define INT32_MAX 0x7fffffff
#define UINT32_MAX 0xffffffffU

#undef INT64_MIN
#undef INT64_MAX
#undef UINT64_MAX
#ifdef CONCH_MARCH_BITS
# if (CONCH_MARCH_BITS == CONCH_MARCH_32)
#  ifdef CONCH_STDINT_TYPEDEF
 typedef signed long long int64_t;
 typedef unsigned long long uint64_t;
#  else
#   define int64_t signed long long
#   define uint64_t unsigned long long
#  endif
#  define INT64_MIN (-0x8000000000000000LL)
#  define INT64_MAX 0x7fffffffffffffffLL
#  define UINT64_MAX 0xffffffffffffffffULL
# elif (CONCH_MARCH_BITS == CONCH_MARCH_64)
#  ifdef CONCH_STDINT_TYPEDEF
 typedef signed long int64_t;
 typedef unsigned long uint64_t;
#  else
#   define int64_t signed long
#   define uint64_t unsigned long
#  endif
#  define INT64_MIN (-0x8000000000000000L)
#  define INT64_MAX 0x7fffffffffffffffL
#  define UINT64_MAX 0xffffffffffffffffUL
# else
#  error "!!!unknown CONCH_MARCH_BITS!!!"
# endif
#else
# error "!!!undefined CONCH_MARCH_BITS!!!"
#endif

#ifdef CONCH_STDINT_TYPEDEF
 typedef signed long int intptr_t;
 typedef unsigned long int uintptr_t;
#else
# define intptr_t signed long int
# define uintptr_t unsigned long int
#endif

#undef INTPTR_MIN
#undef INTPTR_MAX
#undef UINTPTR_MAX
#undef PTRDIFF_MIN
#undef PTRDIFF_MAX
#undef SIZE_MAX
#undef SSIZE_MIN
#undef SSIZE_MAX
#ifdef CONCH_MARCH_BITS
# if (CONCH_MARCH_BITS == CONCH_MARCH_32)
#  define INTPTR_MIN INT32_MIN
#  define INTPTR_MAX INT32_MAX
#  define UINTPTR_MAX UINT32_MAX
#  define PTRDIFF_MIN INT32_MIN
#  define PTRDIFF_MAX INT32_MAX
#  define SIZE_MAX UINT32_MAX
#  define SSIZE_MIN INT32_MIN
#  define SSIZE_MAX INT32_MAX
# elif (CONCH_MARCH_BITS == CONCH_MARCH_64)
#  define INTPTR_MIN INT64_MIN
#  define INTPTR_MAX INT64_MAX
#  define UINTPTR_MAX UINT64_MAX
#  define PTRDIFF_MIN INT64_MIN
#  define PTRDIFF_MAX INT64_MAX
#  define SIZE_MAX UINT64_MAX
#  define SSIZE_MIN INT64_MIN
#  define SSIZE_MAX INT64_MAX
# else
#  error "!!!unknown CONCH_MARCH_BITS!!!"
# endif
#else
# error "!!!undefined CONCH_MARCH_BITS!!!"
#endif


#endif
