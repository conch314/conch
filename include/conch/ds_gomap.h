/* @file: ds_gomap.c
 * #desc:
 *    The definitions of high-performance SwissTable in golang.
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

#ifndef _CONCH_DS_GOMAP_H
#define _CONCH_DS_GOMAP_H

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>


union gomap_group {
	/* XXX: simd optimization */
	uint32_t ctrl;
	uint8_t _ctrl[4];
};

struct gomap_head {
	union gomap_group *group;
	void *array;
	size_t size;
	size_t used;
	size_t capacity;
	/* input key, length */
	uint64_t (*call_hash)(const void *, size_t);
	/* bucket, input key, length */
	int32_t (*call_cmp)(void *, const void *, size_t);
};

#define GOMAP_HEAD_SET(_ctrl, _array, _size, _capacity, \
		_hash, _cmp) \
	{ \
		.group = _ctrl, \
		.array = _array, \
		.size = _size, \
		.used = 0, \
		.capacity = _capacity, \
		.call_hash = _hash, \
		.call_cmp = _cmp \
	}
#define GOMAP_HEAD_NEW(name, _ctrl, _array, _size, _capacity, \
		_hash, _cmp) \
	struct gomap_head name = GOMAP_HEAD_SET(_ctrl, _array, \
		_size, _capacity, _hash, _cmp)
#define GOMAP_HEAD_INIT(x, _ctrl, _array, _size, _capacity, \
		_hash, _cmp) \
	do { \
		(x)->group = _ctrl; \
		(x)->array = _array; \
		(x)->size = _size; \
		(x)->used = 0; \
		(x)->capacity = _capacity; \
		(x)->call_hash = _hash; \
		(x)->call_cmp = _cmp; \
	} while (0)

/* 0b1'0000000 */
#define GOMAP_EMPTY 0x80
/* 0b1'1111110 */
#define GOMAP_DELETE 0xfe

#define GOMAP_CTRL_OF(x, n) ((x)->group[(n) >> 2]._ctrl[(n) & 3])
#define GOMAP_ARRAY_OF(x, n) \
	((void *)((char *)(x)->array + (x)->size * (n)))

#define GOMAP_CTRL_ALIGN(x) (((x) + 3) / 4)
#define GOMAP_ARRAY_ALIGN(x) (4 * GOMAP_CTRL_ALIGN(x))

#define GOMAP_FACTOR(x) (((x)->used * 1000) / (x)->capacity)
#define GOMAP_USED(x) ((x)->used)
#define GOMAP_CAPACITY(x) ((x)->capacity)


#ifdef __cplusplus
extern "C" {
#endif

/* ds_gomap.c */
extern
void conch_gomap_ctrl_empty(struct gomap_head *head)
;
extern
void *conch_gomap_insert(struct gomap_head *head,
		const void *key, size_t len)
;
extern
void *conch_gomap_find(struct gomap_head *head,
		const void *key, size_t len)
;
extern
void *conch_gomap_delete(struct gomap_head *head,
		const void *key, size_t len)
;

#ifdef __cplusplus
}
#endif


#endif
