/* @file: md5.c
 * #desc:
 *    The implementations of md5 hash algorithm.
 *
 * #copy:
 *    Copyright (C) 1970 Public Free Software.
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
#include <conch/md5.h>


/* md5 round constants */
static const uint32_t md5_constants[64] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
	};

/* md5 round number */
static const uint32_t md5_number[64] = {
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
	};

#define ROL(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

/* round functions */
#define FF(b, c, d) ((d) ^ ((b) & ((c) ^ (d))))
#define FG(b, c, d) FF(d, b, c)
#define FH(b, c, d) ((b) ^ (c) ^ (d))
#define FI(b, c, d) ((c) ^ ((b) | ~(d)))


/* @func: _md5_compress (static)
 * #desc:
 *    md5 compression function.
 *
 * #1: ctx [in/out] md5 struct context
 * #2: s   [in]     input block
 */
static void _md5_compress(struct md5_ctx *ctx, const uint8_t *s)
{
	uint32_t A, B, C, D, m[16], tmp, f, g;
	A = ctx->state[0];
	B = ctx->state[1];
	C = ctx->state[2];
	D = ctx->state[3];

#if 0

	for (int32_t i = 0; i < 16; i++) {
		m[i] = (uint32_t)s[0]
			| (uint32_t)s[1] << 8
			| (uint32_t)s[2] << 16
			| (uint32_t)s[3] << 24;
		s += 4;
	}

	for (uint32_t i = 0; i < 64; i++) {
		if (i < 16) {
			f = FF(B, C, D);
			g = i;
		} else if (i < 32) {
			f = FG(B, C, D);
			g = (5 * i + 1) % 16;
		} else if (i < 48) {
			f = FH(B, C, D);
			g = (3 * i + 5) % 16;
		} else {
			f = FI(B, C, D);
			g = (7 * i) % 16;
		}

		tmp = D;
		D = C;
		C = B;
		B = B + ROL(A + f + md5_constants[i] + m[g], md5_number[i]);
		A = tmp;
	}

#else

#define PACK4(x) \
	((uint32_t)((x)[0]) | (uint32_t)((x)[1]) << 8 \
	| (uint32_t)((x)[2]) << 16 | (uint32_t)((x)[3]) << 24)

#define RM64(n) \
	tmp = D; D = C; C = B; \
	B = B + ROL(A + f + md5_constants[n] + m[g], md5_number[n]); \
	A = tmp

#define FF16(n) \
	f = FF(B, C, D); g = n; RM64(n)
#define FG32(n) \
	f = FG(B, C, D); g = (5 * n + 1) % 16; RM64(n)
#define FH48(n) \
	f = FH(B, C, D); g = (3 * n + 5) % 16; RM64(n)
#define FI64(n) \
	f = FI(B, C, D); g = (7 * n) % 16; RM64(n)

	m[0] = PACK4(s); s += 4;
	m[1] = PACK4(s); s += 4;
	m[2] = PACK4(s); s += 4;
	m[3] = PACK4(s); s += 4;
	m[4] = PACK4(s); s += 4;
	m[5] = PACK4(s); s += 4;
	m[6] = PACK4(s); s += 4;
	m[7] = PACK4(s); s += 4;
	m[8] = PACK4(s); s += 4;
	m[9] = PACK4(s); s += 4;
	m[10] = PACK4(s); s += 4;
	m[11] = PACK4(s); s += 4;
	m[12] = PACK4(s); s += 4;
	m[13] = PACK4(s); s += 4;
	m[14] = PACK4(s); s += 4;
	m[15] = PACK4(s);

	FF16(0);
	FF16(1);
	FF16(2);
	FF16(3);
	FF16(4);
	FF16(5);
	FF16(6);
	FF16(7);
	FF16(8);
	FF16(9);
	FF16(10);
	FF16(11);
	FF16(12);
	FF16(13);
	FF16(14);
	FF16(15);

	FG32(16);
	FG32(17);
	FG32(18);
	FG32(19);
	FG32(20);
	FG32(21);
	FG32(22);
	FG32(23);
	FG32(24);
	FG32(25);
	FG32(26);
	FG32(27);
	FG32(28);
	FG32(29);
	FG32(30);
	FG32(31);

	FH48(32);
	FH48(33);
	FH48(34);
	FH48(35);
	FH48(36);
	FH48(37);
	FH48(38);
	FH48(39);
	FH48(40);
	FH48(41);
	FH48(42);
	FH48(43);
	FH48(44);
	FH48(45);
	FH48(46);
	FH48(47);

	FI64(48);
	FI64(49);
	FI64(50);
	FI64(51);
	FI64(52);
	FI64(53);
	FI64(54);
	FI64(55);
	FI64(56);
	FI64(57);
	FI64(58);
	FI64(59);
	FI64(60);
	FI64(61);
	FI64(62);
	FI64(63);

#endif

	ctx->state[0] += A;
	ctx->state[1] += B;
	ctx->state[2] += C;
	ctx->state[3] += D;
}

/* @func: conch_md5_init
 * #desc:
 *    md5 struct context initialization.
 *
 * #1: ctx [out] md5 struct context
 */
void conch_md5_init(struct md5_ctx *ctx)
{
	ctx->count = 0;
	ctx->state[0] = 0x67452301;
	ctx->state[1] = 0xefcdab89;
	ctx->state[2] = 0x98badcfe;
	ctx->state[3] = 0x10325476;
}

/* @func: conch_md5_process
 * #desc:
 *    md5 processing buffer function.
 *
 * #1: ctx [in/out] md5 struct context
 * #2: s   [in]     input buffer
 * #3: len [in]     input length
 */
void conch_md5_process(struct md5_ctx *ctx, const uint8_t *s,
		size_t len)
{
#define BLOCKSIZE MD5_BLOCKSIZE

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
			_md5_compress(ctx, ctx->buf);
			n = 0;
			s += h;
			len -= h;
		case 0:
			while (len >= BLOCKSIZE) {
				/* processing */
				_md5_compress(ctx, s);
				s += BLOCKSIZE;
				len -= BLOCKSIZE;
			}

			n = len;
			if (n)
				conch_memcpy(ctx->buf, s, n);
	}
	ctx->count = n;
}

/* @func: conch_md5_finish
 * #desc:
 *    md5 process the remaining bytes in the buffer and end.
 *
 * #1: ctx [in/out] md5 struct context
 * #2: len [in]     total length
 */
void conch_md5_finish(struct md5_ctx *ctx, uint64_t len)
{
	/* padding */
	uint8_t padbuf[MD5_BLOCKSIZE];
	conch_memset(padbuf, 0, sizeof(padbuf));
	padbuf[0] = 0x80;
	conch_md5_process(ctx, padbuf, 1 + ((119 - (len % 64)) % 64));

	/* bit length */
	len *= 8;
	for (int32_t i = 0; i < 8; i++) {
		ctx->buf[56 + i] = (uint8_t)len;
		len >>= 8;
	}
	_md5_compress(ctx, ctx->buf);
}

/* @func: conch_md5
 * #desc:
 *    md5 single-time processing function.
 *
 * #1: ctx [in/out] md5 struct context
 * #2: s   [in]     input buffer
 * #3: len [in]     input length
 */
void conch_md5(struct md5_ctx *ctx, const uint8_t *s, size_t len)
{
	conch_md5_process(ctx, s, len);
	conch_md5_finish(ctx, len);
}
