/* @file: ghash.c
 * #desc:
 *    The implementations of galois message authentication code.
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
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>
#include <conch/c_string.h>
#include <conch/ghash.h>


/* y = GF((y ^ x) * h), 0x87 = x^128'+x^7+x^2+x+1 (reflected 0xe1) */

#define PACK8_BE(x) ((uint64_t)((x)[0]) << 56 \
	| (uint64_t)((x)[1]) << 48 | (uint64_t)((x)[2]) << 40 \
	| (uint64_t)((x)[3]) << 32 | (uint64_t)((x)[4]) << 24 \
	| (uint64_t)((x)[5]) << 16 | (uint64_t)((x)[6]) << 8 \
	| (uint64_t)((x)[7]))

/*
 *  0 0000 0x0000
 *  1 0001 0x1c20 [8] >> 3
 *  2 0010 0x3840 [8] >> 2
 *  3 0011 0x2460 [2] ^ [1]
 *  4 0100 0x7080 [8] >> 1
 *  5 0101 0x6ca0 [4] ^ [1]
 *  6 0110 0x48c0 [4] ^ [2]
 *  7 0111 0x54e0 [4] ^ [3]
 *  8 1000 0xe100
 *  9 1001 0xfd20 [8] ^ [1]
 * 10 1010 0xd940 [8] ^ [2]
 * 11 1011 0xc560 [8] ^ [3]
 * 12 1100 0x9180 [8] ^ [4]
 * 13 1101 0x8da0 [8] ^ [5]
 * 14 1110 0xa9c0 [8] ^ [6]
 * 15 1111 0xb5e0 [8] ^ [7]
 */
static const uint64_t rem4bit[16] = {
	0x0000000000000000ULL, 0x1c20000000000000ULL,
	0x3840000000000000ULL, 0x2460000000000000ULL,
	0x7080000000000000ULL, 0x6ca0000000000000ULL,
	0x48c0000000000000ULL, 0x54e0000000000000ULL,
	0xe100000000000000ULL, 0xfd20000000000000ULL,
	0xd940000000000000ULL, 0xc560000000000000ULL,
	0x9180000000000000ULL, 0x8da0000000000000ULL,
	0xa9c0000000000000ULL, 0xb5e0000000000000ULL
	};


/* @func: conch_ghash_init
 * #desc:
 *    ghash initialization function.
 *
 * #1: ctx [out] ghash struct context
 * #2: key [in]  input key
 */
void conch_ghash_init(struct ghash_ctx *ctx, const uint8_t *key)
{
	uint64_t v[2];
	ctx->h[0][0] = 0;
	ctx->h[0][1] = 0;
	ctx->h[8][0] = v[0] = PACK8_BE(key);     /* hig */
	ctx->h[8][1] = v[1] = PACK8_BE(key + 8); /* low */

	/* 4-bit lookup table method */

	for (uint32_t i = 4; i > 0; i >>= 1) {
		uint64_t t = 0xe100000000000000ULL & (0 - (v[1] & 1));
		v[1] = (v[0] << 63) | (v[1] >> 1);
		v[0] = (v[0] >> 1) ^ t;
		ctx->h[i][0] = v[0];
		ctx->h[i][1] = v[1];
	}

	for (uint32_t i = 2; i < 16; i <<= 1) {
		v[0] = ctx->h[i][0];
		v[1] = ctx->h[i][1];
		for (uint32_t j = 1; j < i; j++) {
			ctx->h[i + j][0] = v[0] ^ ctx->h[j][0];
			ctx->h[i + j][1] = v[1] ^ ctx->h[j][1];
		}
	}

	conch_memset(ctx->s, 0, sizeof(ctx->s));
	ctx->count = 0;
}

/* @func: conch_ghash_gfmul
 * #desc:
 *    galois field multiply by H function.
 *
 * #1: ctx [in/out] ghash struct context
 * #2: x   [in/out] number
 */
void conch_ghash_gfmul(struct ghash_ctx *ctx, uint8_t x[16])
{
	uint64_t v[2];
	uint8_t hi, lo, rem;
	int32_t i = 15;

	/* x = x * H */
	hi = x[i] >> 4;
	lo = x[i] & 0xf;

	v[0] = ctx->h[lo][0];
	v[1] = ctx->h[lo][1];

	while (1) {
		rem = v[1] & 0xf;
		v[1] = (v[0] << 60) | (v[1] >> 4);
		v[0] >>= 4;
		v[0] ^= rem4bit[rem];

		v[0] ^= ctx->h[hi][0];
		v[1] ^= ctx->h[hi][1];

		if (--i < 0)
			break;

		hi = x[i] >> 4;
		lo = x[i] & 0xf;

		rem = v[1] & 0xf;
		v[1] = (v[0] << 60) | (v[1] >> 4);
		v[0] >>= 4;
		v[0] ^= rem4bit[rem];

		v[0] ^= ctx->h[lo][0];
		v[1] ^= ctx->h[lo][1];
	}

	for (i = 0; i < 2; i++) {
		uint64_t A = v[i];
		x[0] = (uint8_t)(A >> 56);
		x[1] = (uint8_t)(A >> 48);
		x[2] = (uint8_t)(A >> 40);
		x[3] = (uint8_t)(A >> 32);
		x[4] = (uint8_t)(A >> 24);
		x[5] = (uint8_t)(A >> 16);
		x[6] = (uint8_t)(A >> 8);
		x[7] = (uint8_t)A;
		x += 8;
	}
}

/* @func: conch_ghash_block
 * #desc:
 *    ghash block processing function.
 *
 * #1: ctx [in/out] ghash struct context
 * #2: s   [in]     input block
 */
void conch_ghash_block(struct ghash_ctx *ctx, const uint8_t *s)
{
	for (int32_t i = 0; i < GHASH_BLOCKSIZE; i++)
		ctx->s[i] ^= s[i];

	conch_ghash_gfmul(ctx, ctx->s);
}

/* @func: conch_ghash_process
 * #desc:
 *    ghash processing buffer function.
 *
 * #1: ctx [in/out] ghash struct context
 * #2: s   [in]     input buffer
 * #3: len [in]     input length
 */
void conch_ghash_process(struct ghash_ctx *ctx, const uint8_t *s,
		size_t len)
{
#define BLOCKSIZE GHASH_BLOCKSIZE

	size_t n = ctx->count, h;
	switch (n) {
		default:
			h = BLOCKSIZE - n;
			h = (len < h) ? len : h;
			conch_memcpy(ctx->buf + n, s, h);
			n += h;
			if (n != BLOCKSIZE)
				break;

			/* processing */
			conch_ghash_block(ctx, ctx->buf);
			n = 0;
			s += h;
			len -= h;
		case 0:
			while (len >= BLOCKSIZE) {
				/* processing */
				conch_ghash_block(ctx, s);
				s += BLOCKSIZE;
				len -= BLOCKSIZE;
			}

			n = len;
			if (n)
				conch_memcpy(ctx->buf, s, n);
	}
	ctx->count = n;
}

/* @func: conch_ghash_finish
 * #desc:
 *    ghash process the remaining bytes in the buffer and end.
 *
 * #1: ctx [in/out] ghash struct context
 */
void conch_ghash_finish(struct ghash_ctx *ctx)
{
	if (ctx->count) {
		conch_memset(ctx->buf + ctx->count, 0,
			GHASH_BLOCKSIZE - ctx->count);
		conch_ghash_block(ctx, ctx->buf);
	}
}

/* @func: conch_ghash
 * #desc:
 *    ghash single-time processing function.
 *
 * #1: ctx [in/out] ghash struct context
 * #2: s   [in]     input buffer
 * #3: len [in]     input length
 */
void conch_ghash(struct ghash_ctx *ctx, const uint8_t *s, size_t len)
{
	conch_ghash_process(ctx, s, len);
	conch_ghash_finish(ctx);
}
