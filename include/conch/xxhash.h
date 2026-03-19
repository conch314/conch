/* @file: xxhash.h
 * #desc:
 *    The definitions of extremely fast hash algorithm.
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

#ifndef _CONCH_XXHASH_H
#define _CONCH_XXHASH_H

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>


/* xxhash32 */
#define XXHASH32_BLOCKSIZE 16

struct xxhash32_ctx {
	uint32_t state[4];
	uint8_t buf[XXHASH32_BLOCKSIZE];
	uint32_t count;
	uint32_t seed;
};

#define XXHASH32_NEW(x) struct xxhash32_ctx x


/* xxhash64 */
#define XXHASH64_BLOCKSIZE 32

struct xxhash64_ctx {
	uint64_t state[4];
	uint8_t buf[XXHASH64_BLOCKSIZE];
	uint32_t count;
	uint64_t seed;
};

#define XXHASH64_NEW(x) struct xxhash64_ctx x


#ifdef __cplusplus
extern "C" {
#endif

/* xxhash32.c */
extern
void conch_xxhash32_init(struct xxhash32_ctx *ctx, uint32_t seed)
;
extern
void conch_xxhash32_process(struct xxhash32_ctx *ctx, const uint8_t *s,
		size_t len)
;
extern
uint32_t conch_xxhash32_finish(struct xxhash32_ctx *ctx, size_t len)
;
extern
uint32_t conch_xxhash32(struct xxhash32_ctx *ctx, const uint8_t *s,
		size_t len)
;

/* xxhash64.c */
extern
void conch_xxhash64_init(struct xxhash64_ctx *ctx, uint64_t seed)
;
extern
void conch_xxhash64_process(struct xxhash64_ctx *ctx, const uint8_t *s,
		size_t len)
;
extern
uint64_t conch_xxhash64_finish(struct xxhash64_ctx *ctx, uint64_t len)
;
extern
uint64_t conch_xxhash64(struct xxhash64_ctx *ctx, const uint8_t *s,
		size_t len)
;

#ifdef __cplusplus
}
#endif


#endif
