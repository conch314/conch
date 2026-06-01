/* @file: sha1.c
 * #desc:
 *    The implementations sha1 hash algorithm.
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
#include <conch/sha1.h>


#define ROL(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

/* round functions */
#define F1(b, c, d) ((d) ^ ((b) & ((c) ^ (d))))
#define F2(b, c, d) ((b) ^ (c) ^ (d))
#define F3(b, c, d) (((b) & (c)) | ((d) & ((b) | (c))))
#define F4(b, c, d) ((b) ^ (c) ^ (d))

/* round constants */
#define K1 0x5a827999
#define K2 0x6ed9eba1
#define K3 0x8f1bbcdc
#define K4 0xca62c1d6

#define BSWAP32(x) \
	((((x) & 0xff) << 24) \
	| (((x) & 0xff00) << 8) \
	| (((x) & 0xff0000) >> 8) \
	| (((x) & 0xff000000) >> 24))


/* @func: _sha1_compress (static)
 * #desc:
 *    sha1 compression function.
 *
 * #1: ctx [in/out] sha1 struct context
 * #2: s   [in]     input block
 */
static void _sha1_compress(struct sha1_ctx *ctx, const uint8_t *s)
{
	uint32_t A, B, C, D, E, m[80], tmp;
	A = ctx->state[0];
	B = ctx->state[1];
	C = ctx->state[2];
	D = ctx->state[3];
	E = ctx->state[4];

#if 0

	for (int32_t i = 0; i < 16; i++) {
		m[i] = (uint32_t)s[0] << 24
			| (uint32_t)s[1] << 16
			| (uint32_t)s[2] << 8
			| (uint32_t)s[3];
		s += 4;
	}

	for (int32_t i = 16; i < 80; i++)
		m[i] = ROL(m[i - 3] ^ m[i - 8] ^ m[i - 14] ^ m[i - 16], 1);

	for (int32_t i = 0; i < 80; i++) {
		if (i < 20) {
			tmp = ROL(A, 5) + F1(B, C, D) + E + K1 + m[i];
		} else if (i < 40) {
			tmp = ROL(A, 5) + F2(B, C, D) + E + K2 + m[i];
		} else if (i < 60) {
			tmp = ROL(A, 5) + F3(B, C, D) + E + K3 + m[i];
		} else {
			tmp = ROL(A, 5) + F4(B, C, D) + E + K4 + m[i];
		}

		E = D;
		D = C;
		C = ROL(B, 30);
		B = A;
		A = tmp;
	}

#else

#define PACK4_BE(x) \
	((uint32_t)((x)[0]) << 24 | (uint32_t)((x)[1]) << 16 \
	| (uint32_t)((x)[2]) << 8 | (uint32_t)((x)[3]))

#define RE_80(n) \
	m[n] = ROL(m[n - 3] ^ m[n - 8] ^ m[n - 14] ^ m[n - 16], 1)

#define F1K1_20(n) \
	tmp = ROL(A, 5) + F1(B, C, D) + E + K1 + m[n]; \
	E = D; D = C; C = ROL(B, 30); B = A; A = tmp
#define F2K2_40(n) \
	tmp = ROL(A, 5) + F2(B, C, D) + E + K2 + m[n]; \
	E = D; D = C; C = ROL(B, 30); B = A; A = tmp
#define F3K3_60(n) \
	tmp = ROL(A, 5) + F3(B, C, D) + E + K3 + m[n]; \
	E = D; D = C; C = ROL(B, 30); B = A; A = tmp
#define F4K4_80(n) \
	tmp = ROL(A, 5) + F4(B, C, D) + E + K4 + m[n]; \
	E = D; D = C; C = ROL(B, 30); B = A; A = tmp

	m[0] = PACK4_BE(s); s += 4;
	m[1] = PACK4_BE(s); s += 4;
	m[2] = PACK4_BE(s); s += 4;
	m[3] = PACK4_BE(s); s += 4;
	m[4] = PACK4_BE(s); s += 4;
	m[5] = PACK4_BE(s); s += 4;
	m[6] = PACK4_BE(s); s += 4;
	m[7] = PACK4_BE(s); s += 4;
	m[8] = PACK4_BE(s); s += 4;
	m[9] = PACK4_BE(s); s += 4;
	m[10] = PACK4_BE(s); s += 4;
	m[11] = PACK4_BE(s); s += 4;
	m[12] = PACK4_BE(s); s += 4;
	m[13] = PACK4_BE(s); s += 4;
	m[14] = PACK4_BE(s); s += 4;
	m[15] = PACK4_BE(s);

	RE_80(16);
	RE_80(17);
	RE_80(18);
	RE_80(19);
	RE_80(20);
	RE_80(21);
	RE_80(22);
	RE_80(23);
	RE_80(24);
	RE_80(25);
	RE_80(26);
	RE_80(27);
	RE_80(28);
	RE_80(29);
	RE_80(30);
	RE_80(31);
	RE_80(32);
	RE_80(33);
	RE_80(34);
	RE_80(35);
	RE_80(36);
	RE_80(37);
	RE_80(38);
	RE_80(39);
	RE_80(40);
	RE_80(41);
	RE_80(42);
	RE_80(43);
	RE_80(44);
	RE_80(45);
	RE_80(46);
	RE_80(47);
	RE_80(48);
	RE_80(49);
	RE_80(50);
	RE_80(51);
	RE_80(52);
	RE_80(53);
	RE_80(54);
	RE_80(55);
	RE_80(56);
	RE_80(57);
	RE_80(58);
	RE_80(59);
	RE_80(60);
	RE_80(61);
	RE_80(62);
	RE_80(63);
	RE_80(64);
	RE_80(65);
	RE_80(66);
	RE_80(67);
	RE_80(68);
	RE_80(69);
	RE_80(70);
	RE_80(71);
	RE_80(72);
	RE_80(73);
	RE_80(74);
	RE_80(75);
	RE_80(76);
	RE_80(77);
	RE_80(78);
	RE_80(79);

	F1K1_20(0);
	F1K1_20(1);
	F1K1_20(2);
	F1K1_20(3);
	F1K1_20(4);
	F1K1_20(5);
	F1K1_20(6);
	F1K1_20(7);
	F1K1_20(8);
	F1K1_20(9);
	F1K1_20(10);
	F1K1_20(11);
	F1K1_20(12);
	F1K1_20(13);
	F1K1_20(14);
	F1K1_20(15);
	F1K1_20(16);
	F1K1_20(17);
	F1K1_20(18);
	F1K1_20(19);

	F2K2_40(20);
	F2K2_40(21);
	F2K2_40(22);
	F2K2_40(23);
	F2K2_40(24);
	F2K2_40(25);
	F2K2_40(26);
	F2K2_40(27);
	F2K2_40(28);
	F2K2_40(29);
	F2K2_40(30);
	F2K2_40(31);
	F2K2_40(32);
	F2K2_40(33);
	F2K2_40(34);
	F2K2_40(35);
	F2K2_40(36);
	F2K2_40(37);
	F2K2_40(38);
	F2K2_40(39);

	F3K3_60(40);
	F3K3_60(41);
	F3K3_60(42);
	F3K3_60(43);
	F3K3_60(44);
	F3K3_60(45);
	F3K3_60(46);
	F3K3_60(47);
	F3K3_60(48);
	F3K3_60(49);
	F3K3_60(50);
	F3K3_60(51);
	F3K3_60(52);
	F3K3_60(53);
	F3K3_60(54);
	F3K3_60(55);
	F3K3_60(56);
	F3K3_60(57);
	F3K3_60(58);
	F3K3_60(59);

	F4K4_80(60);
	F4K4_80(61);
	F4K4_80(62);
	F4K4_80(63);
	F4K4_80(64);
	F4K4_80(65);
	F4K4_80(66);
	F4K4_80(67);
	F4K4_80(68);
	F4K4_80(69);
	F4K4_80(70);
	F4K4_80(71);
	F4K4_80(72);
	F4K4_80(73);
	F4K4_80(74);
	F4K4_80(75);
	F4K4_80(76);
	F4K4_80(77);
	F4K4_80(78);
	F4K4_80(79);

#endif

	ctx->state[0] += A;
	ctx->state[1] += B;
	ctx->state[2] += C;
	ctx->state[3] += D;
	ctx->state[4] += E;
}

/* @func: conch_sha1_init
 * #desc:
 *    sha1 struct context initialization.
 *
 * #1: ctx [out] sha1 struct context
 */
void conch_sha1_init(struct sha1_ctx *ctx)
{
	ctx->count = 0;
	ctx->state[0] = 0x67452301;
	ctx->state[1] = 0xefcdab89;
	ctx->state[2] = 0x98badcfe;
	ctx->state[3] = 0x10325476;
	ctx->state[4] = 0xc3d2e1f0;
}

/* @func: conch_sha1_process
 * #desc:
 *    sha1 processing buffer function.
 *
 * #1: ctx [in/out] sha1 struct context
 * #2: s   [in]     input buffer
 * #3: len [in]     input length
 */
void conch_sha1_process(struct sha1_ctx *ctx, const uint8_t *s,
		size_t len)
{
#define BLOCKSIZE SHA1_BLOCKSIZE

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
			_sha1_compress(ctx, ctx->buf);
			n = 0;
			s += h;
			len -= h;
		case 0:
			while (len >= BLOCKSIZE) {
				/* processing */
				_sha1_compress(ctx, s);
				s += BLOCKSIZE;
				len -= BLOCKSIZE;
			}

			n = len;
			if (n)
				conch_memcpy(ctx->buf, s, n);
	}
	ctx->count = n;
}

/* @func: conch_sha1_finish
 * #desc:
 *    sha1 process the remaining bytes in the buffer and end.
 *
 * #1: ctx [in/out] sha1 struct context
 * #2: len [in]     total length
 */
void conch_sha1_finish(struct sha1_ctx *ctx, uint64_t len)
{
	/* padding */
	uint8_t padbuf[SHA1_BLOCKSIZE];
	conch_memset(padbuf, 0, sizeof(padbuf));
	padbuf[0] = 0x80;
	conch_sha1_process(ctx, padbuf, 1 + ((119 - (len % 64)) % 64));

	/* bit length */
	len *= 8;
	for (int32_t i = 0; i < 8; i++) {
		ctx->buf[63 - i] = (uint8_t)len;
		len >>= 8;
	}
	_sha1_compress(ctx, ctx->buf);

	for (int32_t i = 0; i < 5; i++)
		ctx->state[i] = BSWAP32(ctx->state[i]);
}

/* @func: conch_sha1
 * #desc:
 *    sha1 single-time processing function.
 *
 * #1: ctx [in/out] sha1 struct context
 * #2: s   [in]     input buffer
 * #3: len [in]     input length
 */
void conch_sha1(struct sha1_ctx *ctx, const uint8_t *s, size_t len)
{
	conch_sha1_process(ctx, s, len);
	conch_sha1_finish(ctx, len);
}
