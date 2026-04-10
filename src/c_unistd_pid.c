/* @file: c_unistd_pid.c
 * #desc:
 *    The implementations of unix standard.
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
#include <conch/c_errno.h>
#include <conch/c_sys_types.h>
#include <conch/c_unistd.h>
#include <conch/c_syscall.h>


/* @func: conch_getpid
 * #desc:
 *    get the process id.
 *
 * #r: [ret] process id
 */
xpid_t conch_getpid(void)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	xpid_t ret;

	ret = (xpid_t)conch_syscall_linux(__NR_getpid);

	return ret;

# else
#  error "!!!unknown CONCH_PLATFORM!!!"
# endif
#else
# error "!!!undefined CONCH_PLATFORM!!!"
#endif
}

/* @func: conch_getppid
 * #desc:
 *    get the parent process id of the process.
 *
 * #r: [ret] parent process id
 */
xpid_t conch_getppid(void)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	xpid_t ret;

	ret = (xpid_t)conch_syscall_linux(__NR_getppid);

	return ret;

# else
#  error "!!!unknown CONCH_PLATFORM!!!"
# endif
#else
# error "!!!undefined CONCH_PLATFORM!!!"
#endif
}

/* @func: conch_getpgid
 * #desc:
 *    get the process group id of the process.
 *
 * #1: pid [in]  process id
 * #r:     [ret] >=0: process group id, -1: errno
 */
xpid_t conch_getpgid(xpid_t pid)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	xpid_t ret;

	ret = (xpid_t)conch_syscall_linux(__NR_getpgid,
		pid);

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

/* @func: conch_setpgid
 * #desc:
 *    set the process group id of the process.
 *
 * #1: pid [in]  process id
 * #2: pg  [in]  process group id
 * #r:     [ret] 0: no error, -1: errno
 */
int32_t conch_setpgid(xpid_t pid, xpid_t pg)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	int32_t ret;

	ret = (int32_t)conch_syscall_linux(__NR_setpgid,
		pid,
		pg);

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

/* @func: conch_getuid
 * #desc:
 *    get the user id of the process.
 *
 * #r: [ret] user id
 */
xuid_t conch_getuid(void)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	xuid_t ret;

	ret = (xuid_t)conch_syscall_linux(__NR_getuid);

	return ret;

# else
#  error "!!!unknown CONCH_PLATFORM!!!"
# endif
#else
# error "!!!undefined CONCH_PLATFORM!!!"
#endif
}

/* @func: conch_geteuid
 * #desc:
 *    get the effective user id of the process.
 *
 * #r: [ret] effective user id
 */
xuid_t conch_geteuid(void)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	xuid_t ret;

	ret = (xuid_t)conch_syscall_linux(__NR_geteuid);

	return ret;

# else
#  error "!!!unknown CONCH_PLATFORM!!!"
# endif
#else
# error "!!!undefined CONCH_PLATFORM!!!"
#endif
}

/* @func: conch_setuid
 * #desc:
 *    set the user id of the process.
 *
 * #1: uid [in]  user id
 * #r:     [ret] 0: no error, -1: errno
 */
int32_t conch_setuid(xuid_t uid)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	int32_t ret;

	ret = (int32_t)conch_syscall_linux(__NR_setuid,
		uid);

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

/* @func: conch_seteuid
 * #desc:
 *    set the effective user id of the process.
 *
 * #1: euid [in]  effective user id
 * #r:      [ret] 0: no error, -1: errno
 */
int32_t conch_seteuid(xuid_t euid)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	int32_t ret;

	ret = (int32_t)conch_syscall_linux(__NR_setresuid,
		-1,
		euid,
		-1);

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

/* @func: conch_getgid
 * #desc:
 *    get the group id of the process.
 *
 * #r: [ret] group id
 */
xgid_t conch_getgid(void)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	xgid_t ret;

	ret = (xgid_t)conch_syscall_linux(__NR_getgid);

	return ret;

# else
#  error "!!!unknown CONCH_PLATFORM!!!"
# endif
#else
# error "!!!undefined CONCH_PLATFORM!!!"
#endif
}

/* @func: conch_getegid
 * #desc:
 *    get the effective group id of the process.
 *
 * #r: [ret] effective group id
 */
xgid_t conch_getegid(void)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	xgid_t ret;

	ret = (xgid_t)conch_syscall_linux(__NR_getegid);

	return ret;

# else
#  error "!!!unknown CONCH_PLATFORM!!!"
# endif
#else
# error "!!!undefined CONCH_PLATFORM!!!"
#endif
}

/* @func: conch_setgid
 * #desc:
 *    set the group id of the process.
 *
 * #1: gid [in]  group id
 * #r:     [ret] 0: no error, -1: errno
 */
int32_t conch_setgid(xgid_t gid)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	int32_t ret;

	ret = (int32_t)conch_syscall_linux(__NR_setgid,
		gid);

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

/* @func: conch_setegid
 * #desc:
 *    set the effective group id of the process.
 *
 * #1: egid [in]  effective group id
 * #r:      [ret] 0: no error, -1: errno
 */
int32_t conch_setegid(xgid_t egid)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	int32_t ret;

	ret = (int32_t)conch_syscall_linux(__NR_setresgid,
		-1,
		egid,
		-1);

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

/* @func: conch_getgroups
 * #desc:
 *    get the process related group id.
 *
 * #1: size [in]  array size number (0: returns the gids number)
 * #2: gids [out] array of groups id
 * #r:      [ret] >=0: gids number, -1: errno
 */
int32_t conch_getgroups(int32_t size, xgid_t gids[])
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	int32_t ret;

	ret = (int32_t)conch_syscall_linux(__NR_getgroups,
		size,
		gids);

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

/* @func: conch_setgroups
 * #desc:
 *    set the related group id of the process.
 *
 * #1: size [in]  array size number
 * #2: gids [in]  array of groups id
 * #r:      [ret] 0: no error, -1: errno
 */
int32_t conch_setgroups(int32_t size, xgid_t gids[])
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	int32_t ret;

	ret = (int32_t)conch_syscall_linux(__NR_setgroups,
		size,
		gids);

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
