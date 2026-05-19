/* @file: config.h
 * #desc:
 *    The definitions of conch configuration.
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

#ifndef _CONCH_CONFIG_H
#define _CONCH_CONFIG_H


#ifndef CONCH_MARCH_BITS
# ifdef __SIZEOF_POINTER__
#  if (__SIZEOF_POINTER__ == 4)
#   define CONCH_MARCH_BITS_32
#  elif (__SIZEOF_POINTER__ == 8)
#   define CONCH_MARCH_BITS_64
#  else
#   error "!!!unknown __SIZEOF_POINTER__!!!"
#  endif
# else
#  error "!!!undefined __SIZEOF_POINTER__!!!"
# endif
#endif

#ifndef CONCH_MARCH_TYPE
# if defined(__i386__)
#  define CONCH_MARCH_X86_32
# elif defined(__x86_64__)
#  define CONCH_MARCH_X86_64
# elif (defined(__arm__) || defined(__thumb__) || defined(__ARM_EABI__))
#  if (defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || __ARM_ARCH >= 7)
#   define CONCH_MARCH_ARM_32
#  else
#   define CONCH_MARCH_ARM_32
#   warning "!!!not is armv7-a!!!"
#  endif
# elif defined(__aarch64__)
#  define CONCH_MARCH_ARM_64
# elif (defined(__riscv) || defined(__riscv_xlen))
#  if (__riscv_xlen == 32)
#   define CONCH_MARCH_RISCV_32
#  else
#   define CONCH_MARCH_RISCV_64
#  endif
# else
#  error "!!!unknown architecture!!!"
# endif
#endif

/* default linux platform */
#ifndef CONCH_PLATFORM_TYPE
# define CONCH_PLATFORM_LINUX
#endif

/* using the platform's libc runtime */
#if 0
# define CONCH_PLATFORM_LIBC
#endif

#if 0
# define CONCH_STDDEF_TYPEDEF
# define CONCH_STDINT_TYPEDEF
# define CONCH_STDARG_TYPEDEF
#endif


#endif
