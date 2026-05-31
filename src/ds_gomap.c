/* @file: ds_gomap.c
 * #desc:
 *    The implementations of high-performance SwissTable in golang.
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
#include <conch/c_string.h>
#include <conch/ds_gomap.h>


/* @func: conch_gomap_ctrl_empty
 * #desc:
 *    ctrl groups set to empty.
 *
 * #1: head [in/out] gomap head struct
 */
void conch_gomap_ctrl_empty(struct gomap_head *head)
{
	conch_memset(head->group, GOMAP_EMPTY,
		sizeof(union gomap_group)
		* GOMAP_CTRL_ALIGN(head->capacity));
}

/* @func: conch_gomap_insert
 * #desc:
 *    gomap insert function.
 *
 * #1: head [in/out] gomap head struct
 * #2: key  [in]     input key
 * #3: len  [in]     key length
 * #r:      [ret]    bucket / NULL pointer
 */
void *conch_gomap_insert(struct gomap_head *head,
		const void *key, size_t len)
{
	uint64_t h = head->call_hash(key, len);
	uint64_t h1 = h >> 7;
	uint8_t h2 = h & 0x7f;

	uint32_t mask = h2 * 0x01010101;
	uint8_t *ctrl;
	size_t j;

	size_t n = head->capacity;
	size_t m = h1 % n;
	for (size_t i = 0; i < n; i += 4) {
		size_t pos = ((m + i) % n) & ~0x3UL;
		uint32_t x = head->group[pos >> 2].ctrl;

		if (x & 0x80808080) /* empty or delete */
			goto e;

		x ^= mask;
		x = (x - 0x01010101) & ~x & 0x80808080;
		if (!x)
			continue;

e:
		/* used */
		ctrl = head->group[pos >> 2]._ctrl;
		for (j = 0; j < 4; j++) {
			if (ctrl[j] != h2)
				continue;

			void *p = GOMAP_ARRAY_OF(head, pos + j);
			if (head->call_cmp(p, key, len))
				return p;
		}

		/* empty or delete */
		for (j = 0; j < 4; j++) {
			if (!(ctrl[j] & 0x80))
				continue;

			ctrl[j] = h2;
			head->used++;
			return GOMAP_ARRAY_OF(head, pos + j);
		}
	}

	return NULL;
}

/* @func: conch_gomap_find
 * #desc:
 *    gomap find function.
 *
 * #1: head [in/out] gomap head struct
 * #2: key  [in]     input key
 * #3: len  [in]     key length
 * #r:      [ret]    bucket / NULL pointer
 */
void *conch_gomap_find(struct gomap_head *head,
		const void *key, size_t len)
{
	uint64_t h = head->call_hash(key, len);
	uint64_t h1 = h >> 7;
	uint8_t h2 = h & 0x7f;

	uint32_t mask = h2 * 0x01010101;
	uint8_t *ctrl;
	size_t j;

	size_t n = head->capacity;
	size_t m = h1 % n;
	for (size_t i = 0; i < n; i += 4) {
		size_t pos = ((m + i) % n) & ~0x3UL;
		uint32_t x = head->group[pos >> 2].ctrl;

		x ^= mask;
		x = (x - 0x01010101) & ~x & 0x80808080;
		if (!x)
			continue;

		/* used */
		ctrl = head->group[pos >> 2]._ctrl;
		for (j = 0; j < 4; j++) {
			if (ctrl[j] != h2)
				continue;

			void *p = GOMAP_ARRAY_OF(head, pos + j);
			if (head->call_cmp(p, key, len))
				return p;
		}
	}

	return NULL;
}

/* @func: conch_gomap_delete
 * #desc:
 *    gomap delete function.
 *
 * #1: head [in/out] gomap head struct
 * #2: key  [in]     input key
 * #3: len  [in]     key length
 * #r:      [ret]    bucket / NULL pointer
 */
void *conch_gomap_delete(struct gomap_head *head,
		const void *key, size_t len)
{
	uint64_t h = head->call_hash(key, len);
	uint64_t h1 = h >> 7;
	uint8_t h2 = h & 0x7f;

	uint32_t mask = h2 * 0x01010101;
	uint8_t *ctrl;
	size_t j;

	size_t n = head->capacity;
	size_t m = h1 % n;
	for (size_t i = 0; i < n; i += 4) {
		size_t pos = ((m + i) % n) & ~0x3UL;
		uint32_t x = head->group[pos >> 2].ctrl;

		x ^= mask;
		x = (x - 0x01010101) & ~x & 0x80808080;
		if (!x)
			continue;

		/* used */
		ctrl = head->group[pos >> 2]._ctrl;
		for (j = 0; j < 4; j++) {
			if (ctrl[j] != h2)
				continue;

			void *p = GOMAP_ARRAY_OF(head, pos + j);
			if (head->call_cmp(p, key, len)) {
				ctrl[j] = GOMAP_DELETE;
				head->used--;
				return p;
			}
		}
	}

	return NULL;
}
