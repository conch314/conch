/* @file: kfifo.h
 * #desc:
 *    The definitions of first-in-first-out circular buffer.
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

#ifndef _CONCH_KFIFO_H
#define _CONCH_KFIFO_H

#include <conch/config.h>
#include <conch/c_stdint.h>


struct kfifo_head {
	uint32_t in;
	uint32_t out;
	uint32_t mask;
	uint32_t size;
	void *buf;
};

#define KFIFO_HEAD_SET(_buf, _size) { 0, 0, (_size) - 1, _size, _buf }
#define KFIFO_NEW(x, _buf, _size) \
	struct kfifo_head x = KFIFO_HEAD_SET(_buf, _size);
#define KFIFO_INIT(x, _buf, _size) \
	(x)->in = 0; \
	(x)->out = 0; \
	(x)->mask = (_size) - 1; \
	(x)->size = _size; \
	(x)->buf = buf

#define KFIFO_RESET(x) \
	do { \
		(x)->in = 0; \
		(x)->out = 0; \
	} while (0)

#define KFIFO_USED(x) ((x)->in - (x)->out)
#define KFIFO_AVAIL(x) ((x)->size - ((x)->in - (x)->out))
#define KFIFO_IS_EMPTY(x) ((x)->in == (x)->out)
#define KFIFO_IS_FULL(x) ((x)->size == ((x)->in - (x)->out))


#ifdef __cplusplus
extern "C" {
#endif

/* kfifo.c */
extern
uint32_t conch_kfifo_in(struct kfifo_head *head, const void *s, uint32_t len)
;
extern
uint32_t conch_kfifo_out(struct kfifo_head *head, void *t, uint32_t len)
;
extern
uint32_t conch_kfifo_peek(struct kfifo_head *head, void *t, uint32_t len,
		uint32_t off)
;

#ifdef __cplusplus
}
#endif


#endif
