/* @file: xsalsa20.h
 * #desc:
 *    The definitions of xsalsa20 stream encryption.
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

#ifndef _CONCH_XSALSA20_H
#define _CONCH_XSALSA20_H

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>


#define XSALSA20_KEY_LEN 32
#define XSALSA20_RAN_LEN 24
#define XSALSA20_CTR_LEN 8

#define XSALSA20_ROUNDS 20
#define XSALSA20_BLOCKSIZE 64

struct xsalsa20_ctx {
	uint32_t state[16];
	union {
		uint32_t state[16];
		uint8_t keystream[XSALSA20_BLOCKSIZE];
	} out;
};

#define XSALSA20_NEW(x) struct xsalsa20_ctx x

#define XSALSA20_COUNT0(x) ((x)->state[8])
#define XSALSA20_COUNT1(x) ((x)->state[9])
#define XSALSA20_KEYSTREAM(x, n) ((x)->out.keystream[n])


#ifdef __cplusplus
extern "C" {
#endif

/* xsalsa20.c */
extern
void conch_xsalsa20_init(struct xsalsa20_ctx *ctx, const uint8_t *key,
		const uint8_t *ran, const uint8_t *ctr)
;
extern
void conch_xsalsa20_block(struct xsalsa20_ctx *ctx, int32_t n)
;
extern
void conch_xsalsa20_crypto(struct xsalsa20_ctx *ctx, uint8_t *buf,
		size_t len)
;

#ifdef __cplusplus
}
#endif


#endif
