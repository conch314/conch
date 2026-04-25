/* @file: c_stdlib_malloc.c
 * #desc:
 *    The implementations of standard library.
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
#include <conch/c_stdlib.h>
#include <conch/c_sys_mman.h>
#include <conch/c_umalloc.h>
#include <conch/c_atomic.h>


static void *_alloc(size_t size, void *arg) {
	(void)arg;
	void *p = conch_mmap(NULL, (size + 4095) & ~4095UL,
		X_PROT_READ | X_PROT_WRITE,
		X_MAP_PRIVATE | X_MAP_ANONYMOUS, -1, 0);

	return (p == X_MAP_FAILED) ? NULL : p;
}

static int32_t _free(void *p, size_t size, void *arg) {
	(void)arg;
	conch_munmap(p, (size + 4095) & ~4095UL);

	return 0;
}

UMALLOC_NEW(__umalloc_ctx, _alloc, _free, NULL);
static spinlock_t _lock = 0;


/* @func: conch_malloc
 * #desc:
 *    heap memory allocator.
 *
 * #1: size [in]  alloc length
 * #r:      [ret] heap pointer
 */
void *conch_malloc(size_t size)
{
	SPIN_LOCK(&_lock);
	void *p = conch_umalloc(&__umalloc_ctx, size);
	SPIN_UNLOCK(&_lock);

	return p;
}

/* @func: conch_free
 * #desc:
 *    heap memory free.
 *
 * #1: p [in/out] heap pointer
 */
void conch_free(void *p)
{
	if (!p)
		return;

	SPIN_LOCK(&_lock);
	conch_umalloc_free(&__umalloc_ctx, p);
	SPIN_UNLOCK(&_lock);
}
