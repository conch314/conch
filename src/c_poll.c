/* @file: c_poll.c
 * #desc:
 *    the implementations of synchronous i/o multiplexing.
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
#include <conch/c_signal.h>
#include <conch/c_poll.h>
#include <conch/c_syscall.h>


/* @func: conch_ppoll
 * #desc:
 *    synchronous i/o multiplexing (pending signal).
 *
 * #1: fds  [in/out] pollfd array
 * #2: nfds [in]     number of pollfd
 * #3: ts   [in]     block waiting time / NULL
 * #4: mask [in]     mask signal set
 * #r:      [ret]    >0: number of ready fds, 0: timeout, -1: errno
 */
int32_t conch_ppoll(struct xpollfd *fds, int32_t nfds,
		const struct xtimespec *ts, const xsigset_t *mask)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	int32_t ret;

	ret = (int32_t)conch_syscall_linux(__NR_ppoll,
		fds,
		nfds,
		ts,
		mask,
		sizeof(xsigset_t));

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

/* @func: conch_poll
 * #desc:
 *    synchronous i/o multiplexing.
 *
 * #1: fds  [in/out] pollfd array
 * #2: nfds [in]     number of pollfd
 * #3: msec [in]     block waiting time / NULL
 * #r:      [ret]    >0: number of ready fds, 0: timeout, -1: errno
 */
int32_t conch_poll(struct xpollfd *fds, int32_t nfds, int32_t msec)
{
	struct xtimespec ts;

	if (msec > 0) {
		ts.tv_sec = msec / 1000;
		ts.tv_nsec = (msec % 1000) * 1000000;
	}

	return conch_ppoll(fds, nfds, (msec < 0) ? NULL : &ts, NULL);
}
