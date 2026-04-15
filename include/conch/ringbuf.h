/* @file: ringbuf.h
 * #desc:
 *    The definitions of ring buffer.
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

#ifndef _CONCH_RINGBUF_H
#define _CONCH_RINGBUF_H

#include <conch/config.h>
#include <conch/c_stdint.h>


struct ringbuf_head {
	uint8_t *buf;
	uint32_t size;
	uint32_t read;
	uint32_t write;
};

#define RINGBUF_HEAD_SET(_buf, _size) \
	{ \
		.buf = _buf, .size = _size, \
		.read = 0, .write = 0 \
	}
#define RINGBUF_NEW(name, _buf, _size) \
	struct ringbuf_head name = RINGBUF_HEAD_SET(_buf, _size)
#define RINGBUF_INIT(x, _buf, _size) \
	(x)->buf = _buf; \
	(x)->size = _size;\
	(x)->read = 0; \
	(x)->write = 0

#define RINGBUF_RESET(x) \
	do { \
		(x)->read = 0; \
		(x)->write = 0; \
	} while (0)

#define RINGBUF_USED(x) ((x)->write - (x)->read)
#define RINGBUF_AVAIL(x) ((x)->size - ((x)->write - (x)->read))

#define RINGBUF_IS_EMPTY(x) ((x)->write == (x)->read)
#define RINGBUF_IS_FULL(x) ((x)->size == ((x)->write - (x)->read))


#ifdef __cplusplus
extern "C" {
#endif

/* ringbuf.c */
extern
uint32_t conch_ringbuf_write(struct ringbuf_head *head, const uint8_t *s,
		uint32_t len)
;
extern
uint32_t conch_ringbuf_read(struct ringbuf_head *head, uint8_t *s,
		uint32_t len)
;
extern
uint32_t conch_ringbuf_peek(struct ringbuf_head *head, uint8_t *s,
		uint32_t len, uint32_t off)
;

#ifdef __cplusplus
}
#endif


#endif
