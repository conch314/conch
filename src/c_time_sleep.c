/* @file: c_time_sleep.c
 * #desc:
 *    The implementations of time functions.
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
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>
#include <conch/c_errno.h>
#include <conch/c_time.h>
#include <conch/c_syscall.h>


/* @func: nanosleep
 * #desc:
 *    nanoseconds sleep function.
 *
 * #1: req [in]  request time
 * #2: rem [out] remaining time / NULL
 * #r:     [ret] 0: no error, -1: errno
 */
int32_t conch_nanosleep(const struct xtimespec *req, struct xtimespec *rem)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	int32_t ret;

	ret = (int32_t)conch_syscall_linux(__NR_nanosleep,
		req,
		rem);

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
