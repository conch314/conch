/* @file: c_sys_mman.c
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
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>
#include <conch/c_sys_types.h>
#include <conch/c_sys_mman.h>
#include <conch/c_syscall.h>


/* @func: conch_mmap
 * #desc:
 *    map memory pages (assigned by page, 4096 aligned).
 *
 * #1: addr  [in/out] fixed address / NULL (kernel decision)
 * #2: len   [in]     pages length
 * #3: prot  [in]     pages permission
 * #4: flags [in]     mapping flags
 * #5: fd    [in]     file descriptor
 * #6: off   [in]     file offset
 * #r:       [ret]    return address (X_MAP_FAILED: errno)
 */
void *conch_mmap(void *addr, size_t len, int32_t prot, int32_t flags,
		int32_t fd, xoff_t off)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	void *ret;

	if ((addr && (uintptr_t)addr & 4095) || len & 4095 || off & 4095) {
		/* errno */
		return X_MAP_FAILED;
	}

	ret = (void *)conch_syscall_linux(__NR_mmap,
		addr,
		len,
		prot,
		flags,
		fd,
		off);

	if ((ssize_t)ret < 0) {
		/* errno */
		ret = X_MAP_FAILED;
	}

	return ret;

# else
#  error "!!!unknown CONCH_PLATFORM!!!"
# endif
#else
# error "!!!undefined CONCH_PLATFORM!!!"
#endif
}

/* @func: conch_munmap
 * #desc:
 *    unmap memory pages (4096 aligned).
 *
 * #1: addr [in/out] pages address
 * #2: len  [in]     pages length
 * #r:      [ret]    0: no error, -1: errno
 */
int32_t conch_munmap(void *addr, size_t len)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	int32_t ret;

	if ((addr && (uintptr_t)addr & 4095) || len & 4095) {
		/* errno */
		return -1;
	}

	ret = (int32_t)conch_syscall_linux(__NR_munmap,
		addr,
		len);

	if (ret) {
		/* errno */
		ret = -1;
	}

	return ret;

# else
#  error "!!!unknown CONCH_PLATFORM!!!"
# endif
#else
# error "!!!undefined CONCH_PLATFORM!!!"
#endif
}

/* @func: conch_mprotect
 * #desc:
 *    set memory pages permission (4096 aligned).
 *
 * #1: addr [in/out] pages address
 * #2: len  [in]     pages length
 * #3: prot [in]     pages permission
 * #r:      [ret]    0: no error, -1: errno
 */
int32_t conch_mprotect(void *addr, size_t len, int32_t prot)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	int32_t ret;

	if ((addr && (uintptr_t)addr & 4095) || len & 4095) {
		/* errno */
		return -1;
	}

	ret = (int32_t)conch_syscall_linux(__NR_mprotect,
		addr,
		prot);

	if (ret) {
		/* errno */
		ret = -1;
	}

	return ret;

# else
#  error "!!!unknown CONCH_PLATFORM!!!"
# endif
#else
# error "!!!undefined CONCH_PLATFORM!!!"
#endif
}

/* @func: conch_msync
 * #desc:
 *    synchronize a file with a memory map.
 *
 * #1: addr  [in/out] pages address
 * #2: len   [in]     pages length
 * #3: flags [in]     flags
 * #r:       [ret]    0: no error, -1: errno
 */
int32_t conch_msync(void *addr, size_t len, int32_t flags)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	int32_t ret;

	if ((addr && (uintptr_t)addr & 4095) || len & 4095) {
		/* errno */
		return -1;
	}

	ret = (int32_t)conch_syscall_linux(__NR_msync,
		addr,
		len,
		flags);

	if (ret) {
		/* errno */
		ret = -1;
	}

	return ret;

# else
#  error "!!!unknown CONCH_PLATFORM!!!"
# endif
#else
# error "!!!undefined CONCH_PLATFORM!!!"
#endif
}

/* @func: conch_mlock
 * #desc:
 *    lock memory pages.
 *
 * #1: addr [in/out] pages address
 * #2: len  [in]     pages length
 * #r:      [ret]    0: no error, -1: errno
 */
int32_t conch_mlock(const void *addr, size_t len)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	int32_t ret;

	if ((addr && (uintptr_t)addr & 4095) || len & 4095) {
		/* errno */
		return -1;
	}

	ret = (int32_t)conch_syscall_linux(__NR_mlock,
		addr,
		len);

	if (ret) {
		/* errno */
		ret = -1;
	}

	return ret;

# else
#  error "!!!unknown CONCH_PLATFORM!!!"
# endif
#else
# error "!!!undefined CONCH_PLATFORM!!!"
#endif
}

/* @func: conch_munlock
 * #desc:
 *    unlock memory pages.
 *
 * #1: addr [in/out] pages address
 * #2: len  [in]     pages length
 * #r:      [ret]    0: no error, -1: errno
 */
int32_t conch_munlock(const void *addr, size_t len)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	int32_t ret;

	if ((addr && (uintptr_t)addr & 4095) || len & 4095) {
		/* errno */
		return -1;
	}

	ret = (int32_t)conch_syscall_linux(__NR_munlock,
		addr,
		len);

	if (ret) {
		/* errno */
		ret = -1;
	}

	return ret;

# else
#  error "!!!unknown CONCH_PLATFORM!!!"
# endif
#else
# error "!!!undefined CONCH_PLATFORM!!!"
#endif
}

/* @func: conch_mlockall
 * #desc:
 *    lock all memory pages.
 *
 * #1: flags [in]  lock flags
 * #r:       [ret] 0: no error, -1: errno
 */
int32_t conch_mlockall(int32_t flags)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	int32_t ret;

	ret = (int32_t)conch_syscall_linux(__NR_mlockall,
		flags);

	if (ret) {
		/* errno */
		ret = -1;
	}

	return ret;

# else
#  error "!!!unknown CONCH_PLATFORM!!!"
# endif
#else
# error "!!!undefined CONCH_PLATFORM!!!"
#endif
}

/* @func: conch_munlockall
 * #desc:
 *    unlock all memory pages.
 *
 * #1: flags [in]  lock flags
 * #r:       [ret] 0: no error, -1: errno
 */
int32_t conch_munlockall(int32_t flags)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	int32_t ret;

	ret = (int32_t)conch_syscall_linux(__NR_munlockall,
		flags);

	if (ret) {
		/* errno */
		ret = -1;
	}

	return ret;

# else
#  error "!!!unknown CONCH_PLATFORM!!!"
# endif
#else
# error "!!!undefined CONCH_PLATFORM!!!"
#endif
}
