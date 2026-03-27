/* @file: chacha20.h
 * #desc:
 *    The definitions of chacha20 stream encryption.
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

#ifndef _CONCH_CHACHA20_H
#define _CONCH_CHACHA20_H

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>


/* chacha20 */
#define CHACHA20_KEY_LEN 32
#define CHACHA20_RAN_LEN 8
#define CHACHA20_CTR_LEN 8
#define CHACHA20_IETF_RAN_LEN 12
#define CHACHA20_IETF_CTR_LEN 4

#define CHACHA20_ROUNDS 20
#define CHACHA20_BLOCKSIZE 64

struct chacha20_ctx {
	uint32_t state[16];
	union {
		uint32_t state[16];
		uint8_t keystream[CHACHA20_BLOCKSIZE];
	} out;
};

#define CHACHA20_NEW(x) struct chacha20_ctx x

#define CHACHA20_COUNT0(x) ((x)->state[12])
#define CHACHA20_COUNT1(x) ((x)->state[13])
#define CHACHA20_KEYSTREAM(x, n) ((x)->out.keystream[n])


/* xchacha20 */
#define XCHACHA20_KEY_LEN 32
#define XCHACHA20_RAN_LEN 24
#define XCHACHA20_CTR_LEN 8

#define XCHACHA20_ROUNDS 20
#define XCHACHA20_BLOCKSIZE 64

struct xchacha20_ctx {
	uint32_t state[16];
	union {
		uint32_t state[16];
		uint8_t keystream[XCHACHA20_BLOCKSIZE];
	} out;
};

#define XCHACHA20_NEW(x) struct xchacha20_ctx x

#define XCHACHA20_COUNT0(x) ((x)->state[12])
#define XCHACHA20_COUNT1(x) ((x)->state[13])
#define XCHACHA20_KEYSTREAM(x, n) ((x)->out.keystream[n])


#ifdef __cplusplus
extern "C" {
#endif

/* chacha20.c */
extern
void conch_chacha20_init(struct chacha20_ctx *ctx, const uint8_t *key,
		const uint8_t *ran, const uint8_t *ctr)
;
extern
void conch_chacha20_ietf_init(struct chacha20_ctx *ctx, const uint8_t *key,
		const uint8_t *ran, const uint8_t *ctr)
;
extern
void conch_chacha20_block(struct chacha20_ctx *ctx, int32_t n)
;
extern
void conch_chacha20_crypto(struct chacha20_ctx *ctx, uint8_t *buf,
		size_t len)
;

/* xchacha20.c */
extern
void conch_xchacha20_init(struct xchacha20_ctx *ctx, const uint8_t *key,
		const uint8_t *ran, const uint8_t *ctr)
;
extern
void conch_xchacha20_block(struct xchacha20_ctx *ctx, int32_t n)
;
extern
void conch_xchacha20_crypto(struct xchacha20_ctx *ctx, uint8_t *buf,
		size_t len)
;

#ifdef __cplusplus
}
#endif


#endif
