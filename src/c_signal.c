/* @file: c_signal.c
 * #desc:
 *    The implementations of posix signals.
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
#include <conch/c_string.h>
#include <conch/c_errno.h>
#include <conch/c_signal.h>
#include <conch/c_sys_types.h>
#include <conch/c_syscall.h>


/* @func: conch_kill
 * #desc:
 *    send a signal to process or process group.
 *
 * #1: pid [in]  process id (0/-pid: process group)
 * #2: sig [in]  signal number
 * #r:     [ret] 0: no error, -1: errno
 */
int32_t conch_kill(xpid_t pid, int32_t sig)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	int32_t ret;

	ret = (int32_t)conch_syscall_linux(__NR_kill,
		pid,
		sig);

	if (ret) {
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

/* @func: conch_killpg
 * #desc:
 *    send a signal to process group.
 *
 * #1: pg  [in]  process id
 * #2: sig [in]  signal number
 * #r:     [ret] 0: no error, -1: errno
 */
int32_t conch_killpg(xpid_t pg, int32_t sig)
{
	return conch_kill(-pg, sig);
}

/* @func: conch_sigaction
 * #desc:
 *    examine and change a signal action.
 *
 * #1: sig [in]  signal number
 * #2: act [in]  new action / NULL
 * #3: old [out] old action / NULL
 * #r:     [ret] 0: no error, -1: errno
 */
int32_t conch_sigaction(int32_t sig, const struct xsigaction *act,
		struct xsigaction *old)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	int32_t ret;

	ret = (int32_t)conch_syscall_linux(__NR_rt_sigaction,
		sig,
		act,
		old,
		sizeof(xsigset_t));

	if (ret) {
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

/* @func: conch_sigprocmask
 * #desc:
 *    examine and change blocked signals.
 *
 * #1: how [in]  behavior type
 * #2: set [in]  new signal set / NULL
 * #3: old [out] old signal set / NULL
 * #r:     [ret] 0: no error, -1: errno
 */
int32_t conch_sigprocmask(int32_t how, const xsigset_t *set,
		xsigset_t *old)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	int32_t ret;

	ret = (int32_t)conch_syscall_linux(__NR_rt_sigprocmask,
		how,
		set,
		old,
		sizeof(xsigset_t));

	if (ret) {
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

/* @func: conch_sigpending
 * #desc:
 *    get the set of signals waiting.
 *
 * #1: set [in]  signal set
 * #r:     [ret] 0: no error, -1: errno
 */
int32_t conch_sigpending(xsigset_t *set)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	int32_t ret;

	ret = (int32_t)conch_syscall_linux(__NR_rt_sigpending,
		set,
		sizeof(xsigset_t));

	if (ret) {
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

/* @func: conch_sigsuspend
 * #desc:
 *    replace the signal set and wait for signal (atomic safe).
 *
 * #1: set [in]  signal set
 * #r:     [ret] 0: no error, -1: errno
 */
int32_t conch_sigsuspend(const xsigset_t *set)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	int32_t ret;

	ret = (int32_t)conch_syscall_linux(__NR_rt_sigsuspend,
		set,
		sizeof(xsigset_t));

	if (ret) {
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

/* @func: conch_sigaddset
 * #desc:
 *    added a signal to signal set.
 *
 * #1: set [in]  signal set
 * #2: sig [in]  signal number
 * #r:     [ret] 0: no error, -1: errno
 */
int32_t conch_sigaddset(xsigset_t *set, int32_t sig)
{
	if (sig <= 0 || sig > X_NSIG) {
		/* errno */
		x_errno = -X_EINVAL;
		return -1;
	}

	sig--;
	((uint8_t *)set)[sig / 8] |= 1U << (sig % 8);

	return 0;
}

/* @func: conch_sigdelset
 * #desc:
 *    delete a signal from a signal set.
 *
 * #1: set [in]  signal set
 * #2: sig [in]  signal number
 * #r:     [ret] 0: no error, -1: errno
 */
int32_t conch_sigdelset(xsigset_t *set, int32_t sig)
{
	if (sig <= 0 || sig > X_NSIG) {
		/* errno */
		x_errno = -X_EINVAL;
		return -1;
	}

	sig--;
	((uint8_t *)set)[sig / 8] &= ~(1U << (sig % 8));

	return 0;
}

/* @func: conch_sigismember
 * #desc:
 *    check the signal in the signal set.
 *
 * #1: set [in]  signal set
 * #2: sig [in]  signal number
 * #r:     [ret] 0: false, 1: true, -1: errno
 */
int32_t conch_sigismember(const xsigset_t *set, int32_t sig)
{
	if (sig <= 0 || sig > X_NSIG) {
		/* errno */
		x_errno = -X_EINVAL;
		return -1;
	}

	sig--;
	if (((uint8_t *)set)[sig / 8] & (1U << (sig % 8)))
		return 1;

	return 0;
}

/* @func: conch_sigemptyset
 * #desc:
 *    init signal set.
 *
 * #1: set [in]  signal set
 * #r:     [ret] 0: no error
 */
int32_t conch_sigemptyset(xsigset_t *set)
{
	conch_memset(set, 0, sizeof(xsigset_t));

	return 0;
}

/* @func: conch_sigfillset
 * #desc:
 *    filling signal set.
 *
 * #1: set [in]  signal set
 * #r:     [ret] 0: no error, -1: errno
 */
int32_t conch_sigfillset(xsigset_t *set)
{
	conch_memset(set, 0xff, sizeof(xsigset_t));

	if (conch_sigdelset(set, X_SIGHUP))
		return -1;
	if (conch_sigdelset(set, X_SIGINT))
		return -1;

	return 0;
}
