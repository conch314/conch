/* @file: c_fcntl.c
 * #desc:
 *    The implementations of.
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
#include <conch/c_sys_types.h>
#include <conch/c_fcntl.h>
#include <conch/c_syscall.h>


/* @func: conch_fcntl
 * #desc:
 *    file descriptor operations.
 *
 * #1: fd  [in]     file descriptor
 * #2: cmd [in]     command
 * #N:     [in/out] command argument
 * #r:     [ret]    return value of command
 */
int32_t conch_fcntl(int32_t fd, int32_t cmd, ...)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	int32_t ret;
	va_list ap;
	va_start(ap, cmd);
	unsigned long arg = va_arg(ap, unsigned long);

	ret = (int32_t)conch_syscall_linux(__NR_fcntl,
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

/* @func: _openat (static)
 * #desc:
 *    open and possibly create a file (TOCTOU).
 *
 * #1: fd    [in]  directory file descriptor
 * #2: path  [in]  path name
 * #3: flags [in]  open flags
 * #4: mode  [in]  file mode (O_CREAT)
 * #r:       [ret] >=0: file descriptor, -1: errno
 */
static int32_t _openat(int32_t fd, const char *path, int32_t flags,
		xmode_t mode)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	int32_t ret;

	ret = (int32_t)conch_syscall_linux(__NR_openat,
		fd,
		path,
		flags,
		mode);

	if (ret < 0) {
		/* errno */
		x_errno = -ret;
		return -1;
	}

	if (flags & X_O_CLOEXEC)
		conch_fcntl(ret, X_F_SETFD, X_FD_CLOEXEC);

	return ret;

# else
#  error "!!!unknown CONCH_PLATFORM!!!"
# endif
#else
# error "!!!undefined CONCH_PLATFORM!!!"
#endif
}

/* @func: conch_openat
 * #desc:
 *    open and possibly create a file (TOCTOU).
 *
 * #1: fd    [in]  directory file descriptor
 * #2: path  [in]  path name
 * #3: flags [in]  open flags
 * #N:       [in]  file mode (O_CREAT)
 * #r:       [ret] >=0: file descriptor, -1: errno
 */
int32_t conch_openat(int32_t fd, const char *path, int32_t flags, ...)
{
	va_list ap;
	va_start(ap, flags);
	xmode_t mode = va_arg(ap, xmode_t);

	return _openat(fd, path, flags, mode);
}

/* @func: conch_open
 * #desc:
 *    open and possibly create a file.
 *
 * #1: path  [in]  path name
 * #2: flags [in]  open flags
 * #N:       [in]  file mode (O_CREAT)
 * #r:       [ret] >=0: file descriptor, -1: errno
 */
int32_t conch_open(const char *path, int32_t flags, ...)
{
	va_list ap;
	va_start(ap, flags);
	xmode_t mode = va_arg(ap, xmode_t);

	return _openat(X_AT_FDCWD, path, flags, mode);
}

/* @func: conch_creat
 * #desc:
 *    possibly create a file.
 *
 * #1: path [in]  path name
 * #2: mode [in]  file mode
 * #r:      [ret] 0: no errno, -1: errno
 */
int32_t conch_creat(const char *path, xmode_t mode)
{
	return conch_open(path, X_O_CREAT | X_O_WRONLY | X_O_TRUNC, mode);
}
