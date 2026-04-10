/* @file: ringbuf.c
 * #desc:
 *    The implementations of ring buffer.
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
#include <conch/c_string.h>
#include <conch/ringbuf.h>


/* @func: conch_ringbuf_write
 * #desc:
 *    write to the ring buffer.
 *
 * #1: head [in/out] ring buffer head
 * #2: s    [in]     input buffer
 * #3: len  [in]     input length
 * #r:      [ret]    length of write
 */
uint32_t conch_ringbuf_write(struct ringbuf_head *head, const uint8_t *s,
		uint32_t len)
{
	uint32_t l, f;

	/* NOTE: unsigned overflow features */

	l = head->size - (head->write - head->read); /* avail length */
	f = head->write % head->size; /* first offset */

#undef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

	head->write += len = MIN(len, l);
	l = MIN(len, head->size - f); /* first length */

	conch_memcpy(head->buf + f, s, l);
	conch_memcpy(head->buf, s + l, len - l);

	return len;
}

/* @func: conch_ringbuf_read
 * #desc:
 *    read from the ring buffer.
 *
 * #1: head [in/out] ring buffer head
 * #2: s    [out]    output buffer
 * #3: len  [in]     buffer length
 * #r:      [ret]    length of read
 */
uint32_t conch_ringbuf_read(struct ringbuf_head *head, uint8_t *s,
		uint32_t len)
{
	uint32_t l, f;

	/* NOTE: unsigned overflow features */

	l = head->write - head->read; /* use length */
	f = head->read % head->size; /* first offset */

#undef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

	head->read += len = MIN(len, l);
	l = MIN(len, head->size - f); /* first length */

	conch_memcpy(s, head->buf + f, l);
	conch_memcpy(s + l, head->buf, len - l);

	return len;
}

/* @func: conch_ringbuf_peek
 * #desc:
 *    peek only the ring buffer.
 *
 * #1: head [in/out] ring buffer head
 * #2: s    [out]    output buffer
 * #3: len  [in]     buffer length
 * #4: off  [in]     offset of read
 * #r:      [ret]    output length
 */
uint32_t conch_ringbuf_peek(struct ringbuf_head *head, uint8_t *s,
		uint32_t len, uint32_t off)
{
	uint32_t l, f;

	/* NOTE: unsigned overflow features */

	l = head->write - head->read; /* use length */
	if (off > l)
		return 0;

	l -= off;
	f = (head->read + off) % head->size; /* first offset */

#undef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

	len = MIN(len, l);
	l = MIN(len, head->size - f); /* first length */

	conch_memcpy(s, head->buf + f, l);
	conch_memcpy(s + l, head->buf, len - l);

	return len;
}
