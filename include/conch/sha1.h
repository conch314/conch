/* @file: sha1.h
 * #desc:
 *    The definitions of sha1 hash algorithm.
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

#ifndef _CONCH_SHA1_H
#define _CONCH_SHA1_H

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>


#define SHA1_LEN 20
#define SHA1_BLOCKSIZE 64

struct sha1_ctx {
	uint32_t state[5];
	uint32_t count;
	uint8_t buf[SHA1_BLOCKSIZE];
};

#define	SHA1_NEW(x) struct sha1_ctx x
#define SHA1_STATE(x, n) (((uint8_t *)((x)->state))[n])


#ifdef __cplusplus
extern "C" {
#endif

/* sha1.c */
extern
void conch_sha1_init(struct sha1_ctx *ctx)
;
extern
void conch_sha1_process(struct sha1_ctx *ctx, const uint8_t *s,
		size_t len)
;
extern
void conch_sha1_finish(struct sha1_ctx *ctx, uint64_t len)
;
extern
void conch_sha1(struct sha1_ctx *ctx, const uint8_t *s, size_t len)
;

#ifdef __cplusplus
}
#endif


#endif
