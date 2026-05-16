/* @file: c_syscall.h
 * #desc:
 *    The definitions of system call.
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

#ifndef _CONCH_C_SYSCALL_H
#define _CONCH_C_SYSCALL_H

#include <conch/config.h>
#if defined(CONCH_PLATFORM_LINUX)
# include <linux/unistd.h>
# define SYSCALL_LINUX_ISERR(x) ((unsigned long)(x) > -4096UL)
#else
# error "!!!unknown platform!!!"
#endif


#ifdef __cplusplus
extern "C" {
#endif

/* c_syscall_linux.S */
extern
long conch_syscall_linux(long n, ...)
;

#ifdef __cplusplus
}
#endif


#endif
