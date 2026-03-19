/* @file: config.h
 * #desc:
 *    The definitions of lazylib configuration.
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

#ifndef _CONCH_CONFIG_H
#define _CONCH_CONFIG_H


/* byte order default to little-endian */

#define CONCH_MARCH_32 1
#define CONCH_MARCH_64 2

#ifndef CONCH_MARCH_BITS
# ifdef __SIZEOF_POINTER__
#  if (__SIZEOF_POINTER__ == 4)
#   define CONCH_MARCH_BITS CONCH_MARCH_32
#  elif (__SIZEOF_POINTER__ == 8)
#   define CONCH_MARCH_BITS CONCH_MARCH_64
#  else
#   error "!!!unknown __SIZEOF_POINTER__!!!"
#  endif
# else
#  error "!!!undefined __SIZEOF_POINTER__!!!"
# endif
#endif

#define CONCH_MARCH_X86_32 1
#define CONCH_MARCH_X86_64 2
#define CONCH_MARCH_ARM_32 3
#define CONCH_MARCH_ARM_64 4
#define CONCH_MARCH_RISCV_32 5
#define CONCH_MARCH_RISCV_64 6

#ifndef CONCH_MARCH_TYPE
# if defined(__i386__)
#  define CONCH_MARCH_TYPE CONCH_MARCH_X86_32
# elif defined(__x86_64__)
#  define CONCH_MARCH_TYPE CONCH_MARCH_X86_64
# elif (defined(__arm__) || defined(__thumb__) || defined(__ARM_EABI__))
#  define CONCH_MARCH_TYPE CONCH_MARCH_ARM_32
# elif defined(__aarch64__)
#  define CONCH_MARCH_TYPE CONCH_MARCH_ARM_64
# elif (defined(__riscv) || defined(__riscv_xlen))
#  if (__riscv_xlen == 32)
#   define CONCH_MARCH_TYPE CONCH_MARCH_RISCV_32
#  else
#   define CONCH_MARCH_TYPE CONCH_MARCH_RISCV_64
#  endif
# else
#  error "!!!unknown CONCH_MARCH_TYPE!!!"
# endif
#endif

/* default linux platform */

#define CONCH_PLATFORM_NOSYS 1
#define CONCH_PLATFORM_LINUX 2

#ifndef CONCH_PLATFORM
# define CONCH_PLATFORM CONCH_PLATFORM_LINUX
#endif

#if 0
# define CONCH_STDDEF_TYPEDEF
# define CONCH_STDINT_TYPEDEF
# define CONCH_STDARG_TYPEDEF
#endif


#endif
