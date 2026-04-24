/* @file: c_sys_ioctl.h
 * #desc:
 *    The definitions of control device.
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

#ifndef _CONCH_C_SYS_IOCTL_H
#define _CONCH_C_SYS_IOCTL_H

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* c_sys_ioctl.c */
extern
int32_t conch_ioctl(int32_t fd, int32_t cmd, ...)
;

#ifdef __cplusplus
}
#endif


#endif
