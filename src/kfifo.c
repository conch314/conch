/* @file: kfifo.c
 * #desc:
 *    The implementations of first-in-first-out circular buffer.
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
#include <conch/kfifo.h>


/* @func: conch_kfifo_in
 * #desc:
 *    put the data into the fifo buffer.
 *
 * #1: head [in/out] kfifo head
 * #2: s    [in]     input buffer
 * #3: len  [in]     input length
 * #r:      [ret]    real input length
 */
uint32_t conch_kfifo_in(struct kfifo_head *head, const void *s, uint32_t len)
{
	uint32_t l, f;

	l = KFIFO_AVAIL(head); /* avail length */
	f = head->in & head->mask; /* first offset */

#undef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

	head->in += len = MIN(len, l);
	l = MIN(len, head->size - f); /* first length */

	conch_memcpy((char *)head->buf + f, s, l);
	conch_memcpy(head->buf, (char *)s + l, len - l);

	return len;
}

/* @func: conch_kfifo_out
 * #desc:
 *    get the data from the fifo buffer.
 *
 * #1: head [in/out] kfifo head
 * #2: t    [out]    output buffer
 * #3: len  [in]     output length
 * #r:      [ret]    real output length
 */
uint32_t conch_kfifo_out(struct kfifo_head *head, void *t, uint32_t len)
{
	uint32_t l, f;

	l = KFIFO_USED(head); /* used length */
	f = head->out & head->mask; /* first offset */

#undef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

	head->out += len = MIN(len, l);
	l = MIN(len, head->size - f); /* first length */

	conch_memcpy(t, (char *)head->buf + f, l);
	conch_memcpy((char *)t + l, head->buf, len - l);

	return len;
}

/* @func: conch_kfifo_peek
 * #desc:
 *    peek only the fifo buffer.
 *
 * #1: head [in/out] kfifo head
 * #2: t    [out]    output buffer
 * #3: len  [in]     output length
 * #4: off  [in]     fifo offset
 * #r:      [ret]    real output length
 */
uint32_t conch_kfifo_peek(struct kfifo_head *head, void *t, uint32_t len,
		uint32_t off)
{
	uint32_t l, f;

	l = KFIFO_USED(head); /* used length */
	if (off > l)
		return 0;

	l -= off;
	f = (head->out + off) & head->mask; /* first offset */

#undef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

	len = MIN(len, l);
	l = MIN(len, head->size - f); /* first length */

	conch_memcpy(t, (char *)head->buf + f, l);
	conch_memcpy((char *)t + l, head->buf, len - l);

	return len;
}
