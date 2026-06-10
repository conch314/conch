/* @file: c_time_timer.c
 * #desc:
 *    The implementations of timer functions.
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

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>
#include <conch/c_errno.h>
#include <conch/c_sys_types.h>
#include <conch/c_time.h>
#include <conch/c_syscall.h>


/* @func: conch_setitimer
 * #desc:
 *    set the new interval timer.
 *
 * #1: which [in]  timer type
 * #2: new   [in]  new itimerval
 * #3: old   [out] old itimerval / NULL
 * #r:       [ret] 0: no error, -1: errno
 */
int32_t conch_setitimer(int32_t which, const struct xitimerval *new,
		struct xitimerval *old)
{
#if defined(CONCH_PLATFORM_LINUX)

	int32_t ret;

	ret = (int32_t)conch_syscall_linux(__NR_setitimer,
		which,
		new,
		old);

	if (SYSCALL_LINUX_ISERR(ret)) {
		/* errno */
		x_errno = -ret;
		return -1;
	}

	return ret;

#else
# error "!!!unknown platform!!!"
#endif
}

/* @func: conch_getitimer
 * #desc:
 *    get the current interval timer.
 *
 * #1: which [in]  timer type
 * #2: curr  [out] current itimerval
 * #r:       [ret] 0: no error, -1: errno
 */
int32_t conch_getitimer(int32_t which, struct xitimerval *curr)
{
#if defined(CONCH_PLATFORM_LINUX)

	int32_t ret;

	ret = (int32_t)conch_syscall_linux(__NR_getitimer,
		which,
		curr);

	if (SYSCALL_LINUX_ISERR(ret)) {
		/* errno */
		x_errno = -ret;
		return -1;
	}

	return ret;

#else
# error "!!!unknown platform!!!"
#endif
}
