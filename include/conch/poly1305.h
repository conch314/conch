/* @file: poly1305.h
 * #desc:
 *    The definitions of poly1305 message authentication code.
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

#ifndef _CONCH_POLY1305_H
#define _CONCH_POLY1305_H

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>


#define POLY1305_KEY_LEN 32
#define POLY1305_TAG_LEN 16

#define POLY1305_BLOCKSIZE 16

struct poly1305_ctx {
	uint32_t h[5];
	uint32_t r[5];
	uint32_t s[5];
	uint32_t count;
	union {
		uint8_t buf[POLY1305_BLOCKSIZE];
		uint8_t tag[POLY1305_TAG_LEN];
	} u;
};

#define POLY1305_NEW(x) struct poly1305_ctx x

#define POLY1305_TAG(x, n) (((uint8_t *)(x)->u.tag)[n])


#ifdef __cplusplus
extern "C" {
#endif

/* poly1305.c */
extern
void conch_poly1305_init(struct poly1305_ctx *ctx, const uint8_t *key)
;
extern
void conch_poly1305_block(struct poly1305_ctx *ctx, const uint8_t *s,
		uint32_t padbit)
;
extern
void conch_poly1305_tag(struct poly1305_ctx *ctx, uint8_t *tag)
;
extern
void conch_poly1305_process(struct poly1305_ctx *ctx, const uint8_t *s,
		size_t len)
;
extern
void conch_poly1305_finish(struct poly1305_ctx *ctx)
;
extern
void conch_poly1305(struct poly1305_ctx *ctx, const uint8_t *s,
		size_t len)
;

#ifdef __cplusplus
}
#endif


#endif
