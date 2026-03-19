/* @file: c_umalloc.h
 * #desc:
 *    The definitions of a simple memory allocator.
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

#ifndef _CONCH_C_UMALLOC_H
#define _CONCH_C_UMALLOC_H

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>
#include <conch/list.h>


/* stack 16-byte aligend */
struct umalloc_chunk {
	uint32_t prev_size;
	uint32_t size_flags;
};

struct umalloc_chunk_node {
	struct list_node list;
	size_t size;
#ifdef CONCH_MARCH_BITS
# if (CONCH_MARCH_BITS == CONCH_MARCH_32)
	uint32_t _allgend1; /* 8+4+(4+4+4)+8 */
	uint32_t _allgend2;
	uint32_t _allgend3;
# endif
#else
# error "!!!undefined CONCH_MARCH_BITS!!!"
#endif
	struct umalloc_chunk chunk;
};

struct umalloc_ctx {
	struct list_head chunk;
	struct umalloc_chunk *idle;
	void *arg;
	/* size, arg */
	void *(*call_alloc)(size_t, void *);
	/* alloc pointer, size, arg */
	int32_t (*call_free)(void *, size_t, void *);
};

#define UMALLOC_NEW(name, alloc, free, _arg) \
	struct umalloc_ctx name = { \
		.chunk.node = NULL, \
		.call_alloc = alloc, \
		.call_free = free, \
		.arg = _arg \
		}
#define UMALLOC_INIT(x, alloc, free, _arg) \
	(x)->chunk.node = NULL; \
	(x)->call_alloc = alloc; \
	(x)->call_free = free; \
	(x)->arg = _arg


#ifdef __cplusplus
extern "C" {
#endif

/* c_umalloc.c */
extern
void *conch_umalloc(struct umalloc_ctx *ctx, size_t size)
;
extern
int32_t conch_umalloc_free(struct umalloc_ctx *ctx, void *p)
;
extern
int32_t conch_umalloc_free_all(struct umalloc_ctx *ctx)
;

#ifdef __cplusplus
}
#endif


#endif
