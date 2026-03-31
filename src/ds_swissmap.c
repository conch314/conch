/* @file: ds_swissmap.c
 * #desc:
 *    The implementations of swiss high-performance hash table.
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
#include <conch/ds_swissmap.h>


/* @func: conch_swissmap_empty
 * #desc:
 *    swissmap ctrl group set to empty.
 *
 * #1: head [in/out] swissmap head
 */
void conch_swissmap_empty(struct swissmap_head *head)
{
	conch_memset(head->group, SWISSMAP_EMPTY,
		sizeof(union swissmap_group)
		* SWISSMAP_CLIGN(head->total_size));
}

/* @func: conch_swissmap_insert
 * #desc:
 *    swissmap insert function.
 *
 * #1: head [in/out] swissmap head
 * #2: key  [in]     input key
 * #3: len  [in]     key length
 * #r:      [ret]    bucket / NULL pointer
 */
void *conch_swissmap_insert(struct swissmap_head *head,
		const void *key, size_t len)
{
	uint64_t h = head->call_hash(key, len);
	uint64_t h1 = h >> 7;
	uint8_t h2 = h & 0x7f;

	uint32_t _mask = h2 * 0x01010101;
	uint8_t *_ctrl;

	size_t n = head->total_size;
	size_t m = h1 % n;
	for (size_t i = 0; i < n; i += 4) {
		size_t pos = ((m + i) % n) & ~0x3;
		uint32_t x = head->group[pos >> 2].ctrl;

		size_t j = 0;
		if (x & 0x80808080) {
			_ctrl = head->group[pos >> 2]._ctrl;
			for (; !(_ctrl[j] & 0x80) && j < 4; j++);
			_ctrl[j] = h2;
			head->size++;
			return SWISSMAP_ARRAY_OF(head, pos + j);
		}

		x ^= _mask;
		x = (x - 0x01010101) & ~x & 0x80808080;
		if (!x)
			continue;

		_ctrl = head->group[pos >> 2]._ctrl;
		for (; j < 4; j++) {
			void *p = SWISSMAP_ARRAY_OF(head, pos + j);
			if (_ctrl[j] == h2 && !head->call_cmp(p, key, len))
				return p;
		}
	}

	return NULL;
}

/* @func: conch_swissmap_find
 * #desc:
 *    swissmap find function.
 *
 * #1: head [in/out] swissmap head
 * #2: key  [in]     input key
 * #3: len  [in]     key length
 * #r:      [ret]    bucket / NULL pointer
 */
void *conch_swissmap_find(struct swissmap_head *head,
		const void *key, size_t len)
{
	uint64_t h = head->call_hash(key, len);
	uint64_t h1 = h >> 7;
	uint8_t h2 = h & 0x7f;

	uint32_t _mask = h2 * 0x01010101;
	uint8_t *_ctrl;

	size_t n = head->total_size;
	size_t m = h1 % n;
	for (size_t i = 0; i < n; i += 4) {
		size_t pos = ((m + i) % n) & ~0x3;
		uint32_t x = head->group[pos >> 2].ctrl;

		x ^= _mask;
		x = (x - 0x01010101) & ~x & 0x80808080;
		if (!x)
			continue;

		_ctrl = head->group[pos >> 2]._ctrl;
		for (size_t j = 0; j < 4; j++) {
			void *p = SWISSMAP_ARRAY_OF(head, pos + j);
			if (_ctrl[j] == h2 && !head->call_cmp(p, key, len))
				return p;
		}
	}

	return NULL;
}

/* @func: conch_swissmap_delete
 * #desc:
 *    swissmap delete function.
 *
 * #1: head [in/out] swissmap head
 * #2: key  [in]     input key
 * #3: len  [in]     key length
 * #r:      [ret]    bucket / NULL pointer
 */
void *conch_swissmap_delete(struct swissmap_head *head,
		const void *key, size_t len)
{
	uint64_t h = head->call_hash(key, len);
	uint64_t h1 = h >> 7;
	uint8_t h2 = h & 0x7f;

	uint32_t _mask = h2 * 0x01010101;
	uint8_t *_ctrl;

	size_t n = head->total_size;
	size_t m = h1 % n;
	for (size_t i = 0; i < n; i += 4) {
		size_t pos = ((m + i) % n) & ~0x3;
		uint32_t x = head->group[pos >> 2].ctrl;

		x ^= _mask;
		x = (x - 0x01010101) & ~x & 0x80808080;
		if (!x)
			continue;

		_ctrl = head->group[pos >> 2]._ctrl;
		for (size_t j = 0; j < 4; j++) {
			void *p = SWISSMAP_ARRAY_OF(head, pos + j);
			if (_ctrl[j] == h2 && !head->call_cmp(p, key, len)) {
				_ctrl[j] = SWISSMAP_DELETE;
				head->size--;
				return p;
			}
		}
	}

	return NULL;
}
