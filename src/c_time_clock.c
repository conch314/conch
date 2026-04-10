/* @file: c_time_clock.c
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
#include <conch/c_sys_types.h>
#include <conch/c_time.h>
#include <conch/c_syscall.h>


/* @func: conch_clock_gettime
 * #desc:
 *    get the clock and time function.
 *
 * #1: cid [in]  clock type
 * #2: ts  [out] timespec struct
 * #r:     [ret] 0: no error, -1: errno
 */
int32_t conch_clock_gettime(int32_t cid, struct xtimespec *ts)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	int32_t ret;

	ret = (int32_t)conch_syscall_linux(__NR_clock_gettime,
		cid,
		ts);

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

/* @func: conch_clock_settime
 * #desc:
 *    set the clock and time function.
 *
 * #1: cid [in]  clock type
 * #2: ts  [in]  timespec struct
 * #r:     [ret] 0: no error, -1: errno
 */
int32_t conch_clock_settime(int32_t cid, const struct xtimespec *ts)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	int32_t ret;

	ret = (int32_t)conch_syscall_linux(__NR_clock_settime,
		cid,
		ts);

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

/* @func: conch_clock
 * #desc:
 *    get the determine cpu time function.
 *
 * #r: [ret] >0: cpu time (usec), -1: errno
 */
xclock_t conch_clock(void)
{
	xclock_t ret = 0;
	struct xtimespec st;

	ret = conch_clock_gettime(X_CLOCK_PROCESS_CPUTIME_ID, &st);
	if (ret)
		return ret;

	ret = st.tv_sec * 1000000;
	ret += st.tv_nsec / 1000;

	return ret;
}

/* @func: conch_time
 * #desc:
 *    get determine system time function.
 *
 * #1: r [out] result time / NULL
 * #r:   [ret] >0: system time (sec), -1: errno
 */
xtime_t conch_time(xtime_t *r)
{
	xclock_t ret = 0;
	struct xtimespec st;

	ret = conch_clock_gettime(X_CLOCK_REALTIME, &st);
	if (ret)
		return ret;

	ret = st.tv_sec;
	if (r)
		*r = ret;

	return ret;
}
