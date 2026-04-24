/* @file: c_sys_select.c
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
#include <conch/c_sys_select.h>
#include <conch/c_syscall.h>


/* @func: conch_pselect
 * #desc:
 *    synchronous i/o multiplexing (pending signal).
 *
 * #1: nfds [in]     max file descriptor (+1)
 * #2: rfds [in/out] read fd set / NULL
 * #3: wfds [in/out] write fd set / NULL
 * #4: efds [in/out] except fd set / NULL
 * #5: ts   [in]     block waiting time / NULL
 * #6: mask [in]     mask signal set
 * #r:      [ret]    >0: number of ready fds, 0: timeout, -1: errno
 */
int32_t conch_pselect(int32_t nfds, xfd_set *rfds, xfd_set *wfds,
		xfd_set *efds, const struct xtimespec *ts,
		const xsigset_t *mask)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	int32_t ret;

	ret = (int32_t)conch_syscall_linux(__NR_pselect6,
		nfds,
		rfds,
		wfds,
		efds,
		ts,
		mask);

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

/* @func: conch_select
 * #desc:
 *    synchronous i/o multiplexing.
 *
 * #1: nfds [in]     max file descriptor (+1)
 * #2: rfds [in/out] read fd set / NULL
 * #3: wfds [in/out] write fd set / NULL
 * #4: efds [in/out] except fd set / NULL
 * #5: tv   [in]     block waiting time / NULL
 * #r:      [ret]    >0: number of ready fds, 0: timeout, -1: errno
 */
int32_t conch_select(int32_t nfds, xfd_set *rfds, xfd_set *wfds,
		xfd_set *efds, struct xtimeval *tv)
{
	struct xtimespec ts;

	if (tv) {
		if (tv->tv_sec < 0 || tv->tv_usec < 0) {
			/* errno */
			x_errno = X_EINVAL;
			return -1;
		}
		ts.tv_sec = tv->tv_sec + (tv->tv_usec / 1000000);
		ts.tv_nsec = (tv->tv_usec % 1000000) * 1000;
	}

	return conch_pselect(nfds, rfds, wfds, efds, tv ? &ts : NULL, NULL);
}
