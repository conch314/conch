/* @file: c_sys_ioctl.c
 * #desc:
 *    The implementations of control device.
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
#include <conch/c_stdint.h>
#include <conch/c_stdarg.h>
#include <conch/c_errno.h>
#include <conch/c_sys_ioctl.h>
#include <conch/c_syscall.h>


/* @func: conch_ioctl
 * #desc:
 *   control device based on file descriptor.
 *
 * #1: fd  [in]     file descriptor
 * #2: cmd [in]     command
 * #N:     [in/out] command argument
 * #r:     [ret]    return value of command
 */
int32_t conch_ioctl(int32_t fd, int32_t cmd, ...)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	int32_t ret;
	va_list ap;
	va_start(ap, cmd);
	unsigned long arg = va_arg(ap, unsigned long);
	va_end(ap);

	ret = (int32_t)conch_syscall_linux(__NR_ioctl,
		fd,
		cmd,
		arg);

	if (ret < 0) {
		/* errno */
		x_errno = -ret;
		return -1;
	}

	return ret;

# else
#  error "!!!unknown CONCH_PLATFORM!!!"
# endif
#else
# error "!!!undefined CONCH_PLATFORM!!!"
#endif
}
