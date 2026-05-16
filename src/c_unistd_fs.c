/* @file: c_unistd_fs.c
 * #desc:
 *    The implementations of unix standard.
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
#include <conch/c_unistd.h>
#include <conch/c_syscall.h>


/* @func: conch_read
 * #desc:
 *    read from a file descriptor.
 *
 * #1: fd  [in]  file descriptor
 * #2: buf [out] buffer
 * #3: len [in]  buffer length
 * #r:     [ret] >=0: read length, -1: errno
 */
ssize_t conch_read(int32_t fd, void *buf, size_t len)
{
#if defined(CONCH_PLATFORM_LINUX)

	ssize_t ret;

	ret = (ssize_t)conch_syscall_linux(__NR_read,
		fd,
		buf,
		len);

	if (SYSCALL_LINUX_ISERR(ret)) {
		/* errno */
		x_errno = -(int32_t)ret;
		return -1;
	}

	return ret;

#else
# error "!!!unknown platform!!!"
#endif
}

/* @func: conch_write
 * #desc:
 *    write to a file descriptor.
 *
 * #1: fd  [in]  file descriptor
 * #2: buf [in]  input buffer
 * #3: len [in]  buffer length
 * #r:     [ret] >=0: write length, -1: errno
 */
ssize_t conch_write(int32_t fd, const void *buf, size_t len)
{
#if defined(CONCH_PLATFORM_LINUX)

	ssize_t ret;

	ret = (ssize_t)conch_syscall_linux(__NR_write,
		fd,
		buf,
		len);

	if (SYSCALL_LINUX_ISERR(ret)) {
		/* errno */
		x_errno = -(int32_t)ret;
		return -1;
	}

	return ret;

#else
# error "!!!unknown platform!!!"
#endif
}

/* @func: conch_lseek
 * #desc:
 *    repositions the file offset.
 *
 * #1: fd     [in]  file descriptor
 * #2: off    [in]  file offset
 * #3: whence [in]  seek type
 * #r:        [ret] 0: no error, -1: errno
 */
xoff_t conch_lseek(int32_t fd, xoff_t off, int32_t whence)
{
#if defined(CONCH_PLATFORM_LINUX)

	xoff_t ret, _off;

#if defined(CONCH_MARCH_BITS_32)

	ret = (xoff_t)conch_syscall_linux(__NR__llseek,
		fd,
		(uint64_t)off >> 32,
		(uint64_t)off & 0xffffffff,
		&_off,
		whence);

#elif defined(CONCH_MARCH_BITS_64)

	ret = _off = (xoff_t)conch_syscall_linux(__NR_lseek,
		fd,
		off,
		whence);

#else
# error "!!!unknown machine bits!!!"
#endif

	if (SYSCALL_LINUX_ISERR(ret)) {
		/* errno */
		x_errno = -(int32_t)ret;
		return -1;
	}

	return _off;

#else
# error "!!!unknown platform!!!"
#endif
}

/* @func: conch_close
 * #desc:
 *    close a file descriptor.
 *
 * #1: fd [in]  file descriptor
 * #r:    [ret] 0: no error, -1: errno
 */
int32_t conch_close(int32_t fd)
{
#if defined(CONCH_PLATFORM_LINUX)

	int32_t ret;

	ret = (int32_t)conch_syscall_linux(__NR_close,
		fd);

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
