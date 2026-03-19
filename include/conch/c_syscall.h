/* @file: c_syscall.h
 * #desc:
 *    The definitions of system call.
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

#ifndef _CONCH_C_SYSCALL_H
#define _CONCH_C_SYSCALL_H

#include <conch/config.h>
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)
#  include <linux/unistd.h>
#  include <linux/mman.h>
#  include <linux/signal.h>
#  include <linux/time.h>
#  include <linux/fcntl.h>
#  include <linux/fs.h>
#  include <linux/stat.h>
#  include <linux/errno.h>
# else
#  error "!!!unknown CONCH_PLATFORM!!!"
# endif
#else
# error "!!!undefined CONCH_PLATFORM!!!"
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
