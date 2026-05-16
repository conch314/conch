/* @file: c_sys_types.h
 * #desc:
 *    The definitions of system data types.
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

#ifndef _CONCH_C_SYS_TYPES_H
#define _CONCH_C_SYS_TYPES_H

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>


#if defined(CONCH_PLATFORM_LINUX)

typedef ssize_t xoff_t;
typedef int64_t xoff64_t;

typedef int32_t xpid_t;

#if (defined(CONCH_MARCH_X86_32) || defined(CONCH_MARCH_ARM_32))

typedef uint32_t xuid_t;
typedef uint32_t xgid_t;
typedef uint32_t xmode_t;
typedef uint32_t xnlink_t;
typedef uint32_t xblksize_t;

#elif defined(CONCH_MARCH_X86_64)

typedef uint32_t xuid_t;
typedef uint32_t xgid_t;
typedef uint32_t xmode_t;
typedef uint64_t xnlink_t;
typedef uint64_t xblksize_t;

#elif (defined(CONCH_MARCH_ARM_64) \
	|| defined(CONCH_MARCH_RISCV_32) || defined(CONCH_MARCH_RISCV_64))

typedef uint32_t xuid_t;
typedef uint32_t xgid_t;
typedef uint32_t xmode_t;
typedef uint32_t xnlink_t;
typedef uint32_t xblksize_t;

#else
# error "!!!unknown architecture!!!"
#endif

typedef size_t xdev_t;

typedef size_t xino_t;
typedef uint64_t xino64_t;

typedef size_t xblkcnt_t;

typedef size_t fsblkcnt_t;
typedef size_t xsfilcnt_t;

#if defined(CONCH_MARCH_BITS_32)

typedef int32_t xtime_t;
typedef int32_t xsuseconds_t;

#elif defined(CONCH_MARCH_BITS_64)

typedef int64_t xtime_t;
typedef int64_t xsuseconds_t;

#else
# error "!!!unknown machine bits!!!"
#endif

typedef uint32_t xuseconds_t;

typedef int64_t xclock_t;
typedef int32_t xclockid_t;

#else
# error "!!!unknown platform!!!"
#endif


#endif
