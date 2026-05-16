/* @file: c_sys_stat.c
 * #desc:
 *    The implementations of file stat structure.
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
#include <conch/c_sys_stat.h>
#include <conch/c_fcntl.h>
#include <conch/c_time.h>
#include <conch/c_syscall.h>


/* @func: conch_umask
 * #desc:
 *    set file mode creation mask.
 *
 * #1: mode [in]  file mode
 * #r:      [ret] return the old umask
 */
xmode_t conch_umask(xmode_t mode)
{
#if defined(CONCH_PLATFORM_LINUX)

	return (xmode_t)conch_syscall_linux(__NR_umask,
		mode);

#else
# error "!!!unknown platform!!!"
#endif
}

/* @func: conch_fchmodat
 * #desc:
 *    change file permissions.
 *
 * #1: fd    [in]  directory file descriptor
 * #2: path  [in]  path name
 * #3: mode  [in]  file mode
 * #4: flags [in]  flags
 * #r:       [ret] 0: no error, -1: errno
 */
int32_t conch_fchmodat(int32_t fd, const char *path, xmode_t mode,
		int32_t flags)
{
#if defined(CONCH_PLATFORM_LINUX)

	int32_t ret;

	ret = (int32_t)conch_syscall_linux(__NR_fchmodat,
		fd,
		path,
		mode,
		flags);

	if (SYSCALL_LINUX_ISERR(ret)) {
		/* errno */
		x_errno = -ret;
		ret = -1;
	}

	return ret;

#else
# error "!!!unknown platform!!!"
#endif
}

/* @func: conch_chmod
 * #desc:
 *    change file permissions.
 *
 * #1: path [in]  path name
 * #2: mode [in]  file mode
 * #r:      [ret] 0: no error, -1: errno
 */
int32_t conch_chmod(const char *path, xmode_t mode)
{
	return conch_fchmodat(X_AT_FDCWD, path, mode, 0);
}

/* @func: conch_fchmod
 * #desc:
 *    change file permissions.
 *
 * #1: fd   [in]  file descriptor
 * #2: mode [in]  file mode
 * #r:      [ret] 0: no error, -1: errno
 */
int32_t conch_fchmod(int32_t fd, xmode_t mode)
{
#if defined(CONCH_PLATFORM_LINUX)

	int32_t ret;

	ret = (int32_t)conch_syscall_linux(__NR_fchmod,
		fd,
		mode);

	if (SYSCALL_LINUX_ISERR(ret)) {
		/* errno */
		x_errno = -ret;
		ret = -1;
	}

	return ret;

#else
# error "!!!unknown platform!!!"
#endif
}

/* @func: conch_fstatat
 * #desc:
 *    get the file status.
 *
 * #1: fd    [in]  directory file descriptor
 * #2: path  [in]  path name
 * #3: st    [out] stat struct
 * #4: flags [in]  flags
 * #r:       [ret] 0: no error, -1: errno
 */
int32_t conch_fstatat(int32_t fd, const char *path, struct xstat *st,
		int32_t flags)
{
#if defined(CONCH_PLATFORM_LINUX)

	int32_t ret;

#if (defined(CONCH_MARCH_X86_32) || defined(CONCH_MARCH_ARM_32) \
	|| defined(CONCH_MARCH_RISCV_32))

	ret = (int32_t)conch_syscall_linux(__NR_fstatat64,
		fd,
		path,
		st,
		flags);

#elif (defined(CONCH_MARCH_X86_64) || defined(CONCH_MARCH_ARM_64) \
	|| defined(CONCH_MARCH_RISCV_64))

	ret = (int32_t)conch_syscall_linux(__NR_newfstatat,
		fd,
		path,
		st,
		flags);

#else
# error "!!!unknown architecture!!!"
#endif

	if (SYSCALL_LINUX_ISERR(ret)) {
		/* errno */
		x_errno = -ret;
		ret = -1;
	}

	return ret;

#else
# error "!!!unknown platform!!!"
#endif
}

/* @func: conch_stat
 * #desc:
 *    get the file status.
 *
 * #1: path [in]  path name
 * #2: st   [out] stat struct
 * #r:      [ret] 0: no error, -1: errno
 */
int32_t conch_stat(const char *path, struct xstat *st)
{
	return conch_fstatat(X_AT_FDCWD, path, st, 0);
}

/* @func: conch_fstat
 * #desc:
 *    get the file status.
 *
 * #1: fd [in]  file descriptor
 * #2: st [out] stat struct
 * #r:    [ret] 0: no error, -1: errno
 */
int32_t conch_fstat(int32_t fd, struct xstat *st)
{
	return conch_fstatat(fd, NULL, st, 0);
}

/* @func: conch_lstat
 * #desc:
 *    get the file status (not dereferences symbolic link).
 *
 * #1: path [in]  path name
 * #2: st   [out] stat struct
 * #r:      [ret] 0: no error, -1: errno
 */
int32_t conch_lstat(const char *path, struct xstat *st)
{
	return conch_fstatat(X_AT_FDCWD, path, st, X_AT_SYMLINK_NOFOLLOW);
}

/* @func: conch_mkdirat
 * #desc:
 *    create a directory.
 *
 * #1: fd   [in]  directory file descriptor
 * #2: path [in]  path name
 * #3: mode [in]  file mode
 * #r:      [ret] 0: no error, -1: errno
 */
int32_t conch_mkdirat(int32_t fd, const char *path, xmode_t mode)
{
#if defined(CONCH_PLATFORM_LINUX)

	int32_t ret;

	ret = (int32_t)conch_syscall_linux(__NR_mkdirat,
		fd,
		path,
		mode);

	if (SYSCALL_LINUX_ISERR(ret)) {
		/* errno */
		x_errno = -ret;
		ret = -1;
	}

	return ret;

#else
# error "!!!unknown platform!!!"
#endif
}

/* @func: conch_mkdir
 * #desc:
 *    create a directory.
 *
 * #1: path [in]  path name
 * #2: mode [in]  file mode
 * #r:      [ret] 0: no error, -1: errno
 */
int32_t conch_mkdir(const char *path, xmode_t mode)
{
	return conch_mkdirat(X_AT_FDCWD, path, mode);
}

/* @func: conch_mknodat
 * #desc:
 *    create a special or ordinary file.
 *
 * #1: fd   [in]  directory file descriptor
 * #2: path [in]  path name
 * #3: mode [in]  file mode
 * #4: dev  [in]  device id
 * #r:      [ret] 0: no error, -1: errno
 */
int32_t conch_mknodat(int32_t fd, const char *path, xmode_t mode,
		xdev_t dev)
{
#if defined(CONCH_PLATFORM_LINUX)

	int32_t ret;

	ret = (int32_t)conch_syscall_linux(__NR_mknodat,
		fd,
		path,
		mode,
		dev);

	if (SYSCALL_LINUX_ISERR(ret)) {
		/* errno */
		x_errno = -ret;
		ret = -1;
	}

	return ret;

#else
# error "!!!unknown platform!!!"
#endif
}

/* @func: conch_mknod
 * #desc:
 *    create a special or ordinary file.
 *
 * #1: path [in]  path name
 * #2: mode [in]  file mode
 * #3: dev  [in]  device id
 * #r:      [ret] 0: no error, -1: errno
 */
int32_t conch_mknod(const char *path, xmode_t mode, xdev_t dev)
{
	return conch_mknodat(X_AT_FDCWD, path, mode, dev);
}

/* @func: conch_utimensat
 * #desc:
 *    change file timestamps with nanosecond precision.
 *
 * #1: fd    [in]  directory file descriptor
 * #2: path  [in]  path name
 * #3: ts    [in]  timestamps
 * #4: flags [in]  flags
 * #r:       [ret] 0: no error, -1: errno
 */
int32_t conch_utimensat(int32_t fd, const char *path,
		const struct xtimespec ts[2], int32_t flags)
{
#if defined(CONCH_PLATFORM_LINUX)

	int32_t ret;

	ret = (int32_t)conch_syscall_linux(__NR_utimensat,
		fd,
		path,
		ts,
		flags);

	if (SYSCALL_LINUX_ISERR(ret)) {
		/* errno */
		x_errno = -ret;
		ret = -1;
	}

	return ret;

#else
# error "!!!unknown platform!!!"
#endif
}

/* @func: conch_futimens
 * #desc:
 *    change file timestamps with nanosecond precision.
 *
 * #1: fd [in]  directory file descriptor
 * #2: ts [in]  timestamps
 * #r:    [ret] 0: no error, -1: errno
 */
int32_t conch_futimens(int32_t fd, const struct xtimespec ts[2])
{
	return conch_utimensat(fd, NULL, ts, 0);
}
