/* @file: sha256.c
 * #desc:
 *    The implementations of sha256 hash algorithm.
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
#include <conch/sha2.h>


/* sha256 round constants */
static const uint32_t sha256_constants[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
	};

#define ROL(a, b) (((a) >> (b)) | ((a) << (32 - (b))))
#define CH(a, b, c) (((a) & (b)) ^ (~(a) & (c)))
#define MAJ(a, b, c) (((a) & (b)) ^ ((a) & (c)) ^ ((b) & (c)))
#define EP0(x) (ROL(x, 2) ^ ROL(x, 13) ^ ROL(x, 22))
#define EP1(x) (ROL(x, 6) ^ ROL(x, 11) ^ ROL(x, 25))
#define SIG0(x) (ROL(x, 7) ^ ROL(x, 18) ^ ((x) >> 3))
#define SIG1(x) (ROL(x, 17) ^ ROL(x, 19) ^ ((x) >> 10))

#define BSWAP32(x) \
	((((x) & 0xff) << 24) \
	| (((x) & 0xff00) << 8) \
	| (((x) & 0xff0000) >> 8) \
	| (((x) & 0xff000000) >> 24))


/* @func: _sha256_compress (static)
 * #desc:
 *    sha256 compression function.
 *
 * #1: ctx [in/out] sha256 struct context
 * #2: s   [in]     input block
 */
static void _sha256_compress(struct sha256_ctx *ctx, const uint8_t *s)
{
	uint32_t A, B, C, D, E, F, G, H, t1, t2, m[64];
	A = ctx->state[0];
	B = ctx->state[1];
	C = ctx->state[2];
	D = ctx->state[3];
	E = ctx->state[4];
	F = ctx->state[5];
	G = ctx->state[6];
	H = ctx->state[7];

#if 0

	for (int32_t i = 0; i < 16; i++) {
		m[i] = (uint32_t)s[0] << 24
			| (uint32_t)s[1] << 16
			| (uint32_t)s[2] << 8
			| (uint32_t)s[3];
		s += 4;
	}

	for (int32_t i = 16; i < 64; i++)
		m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

	for (int32_t i = 0; i < 64; i++) {
		t1 = H + EP1(E) + CH(E, F, G) + sha256_constants[i] + m[i];
		t2 = EP0(A) + MAJ(A, B, C);
		H = G;
		G = F;
		F = E;
		E = D + t1;
		D = C;
		C = B;
		B = A;
		A = t1 + t2;
	}

#else

#define PACK4_BE(x) \
	((uint32_t)((x)[0]) << 24 | (uint32_t)((x)[1]) << 16 \
	| (uint32_t)((x)[2]) << 8 | (uint32_t)((x)[3]))

#define RE(n) \
	m[n] = SIG1(m[n - 2]) + m[n - 7] + SIG0(m[n - 15]) + m[n - 16]

#define RO(n) \
	t1 = H + EP1(E) + CH(E, F, G) + sha256_constants[n] + m[n]; \
	t2 = EP0(A) + MAJ(A, B, C); \
	H = G; G = F; F = E; E = D + t1; D = C; C = B; B = A; A = t1 + t2

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

	RE(16);
	RE(17);
	RE(18);
	RE(19);
	RE(20);
	RE(21);
	RE(22);
	RE(23);
	RE(24);
	RE(25);
	RE(26);
	RE(27);
	RE(28);
	RE(29);
	RE(30);
	RE(31);
	RE(32);
	RE(33);
	RE(34);
	RE(35);
	RE(36);
	RE(37);
	RE(38);
	RE(39);
	RE(40);
	RE(41);
	RE(42);
	RE(43);
	RE(44);
	RE(45);
	RE(46);
	RE(47);
	RE(48);
	RE(49);
	RE(50);
	RE(51);
	RE(52);
	RE(53);
	RE(54);
	RE(55);
	RE(56);
	RE(57);
	RE(58);
	RE(59);
	RE(60);
	RE(61);
	RE(62);
	RE(63);

	RO(0);
	RO(1);
	RO(2);
	RO(3);
	RO(4);
	RO(5);
	RO(6);
	RO(7);
	RO(8);
	RO(9);
	RO(10);
	RO(11);
	RO(12);
	RO(13);
	RO(14);
	RO(15);
	RO(16);
	RO(17);
	RO(18);
	RO(19);
	RO(20);
	RO(21);
	RO(22);
	RO(23);
	RO(24);
	RO(25);
	RO(26);
	RO(27);
	RO(28);
	RO(29);
	RO(30);
	RO(31);
	RO(32);
	RO(33);
	RO(34);
	RO(35);
	RO(36);
	RO(37);
	RO(38);
	RO(39);
	RO(40);
	RO(41);
	RO(42);
	RO(43);
	RO(44);
	RO(45);
	RO(46);
	RO(47);
	RO(48);
	RO(49);
	RO(50);
	RO(51);
	RO(52);
	RO(53);
	RO(54);
	RO(55);
	RO(56);
	RO(57);
	RO(58);
	RO(59);
	RO(60);
	RO(61);
	RO(62);
	RO(63);

#endif

	ctx->state[0] += A;
	ctx->state[1] += B;
	ctx->state[2] += C;
	ctx->state[3] += D;
	ctx->state[4] += E;
	ctx->state[5] += F;
	ctx->state[6] += G;
	ctx->state[7] += H;
}

/* @func: conch_sha256_init
 * #desc:
 *    sha256 struct context initialization.
 *
 * #1: ctx [out] sha256 struct context
 */
void conch_sha256_init(struct sha256_ctx *ctx)
{
	ctx->count = 0;
	ctx->state[0] = 0x6a09e667;
	ctx->state[1] = 0xbb67ae85;
	ctx->state[2] = 0x3c6ef372;
	ctx->state[3] = 0xa54ff53a;
	ctx->state[4] = 0x510e527f;
	ctx->state[5] = 0x9b05688c;
	ctx->state[6] = 0x1f83d9ab;
	ctx->state[7] = 0x5be0cd19;
}

/* @func: conch_sha224_init
 * #desc:
 *    initialize the sha256 struct context with sha224.
 *
 * #1: ctx [out] sha256 struct context
 */
void conch_sha224_init(struct sha256_ctx *ctx)
{
	ctx->count = 0;
	ctx->state[0] = 0xc1059ed8;
	ctx->state[1] = 0x367cd507;
	ctx->state[2] = 0x3070dd17;
	ctx->state[3] = 0xf70e5939;
	ctx->state[4] = 0xffc00b31;
	ctx->state[5] = 0x68581511;
	ctx->state[6] = 0x64f98fa7;
	ctx->state[7] = 0xbefa4fa4;
}

/* @func: conch_sha256_process
 * #desc:
 *    sha256 processing buffer function.
 *
 * #1: ctx [in/out] sha256 struct context
 * #2: s   [in]     input buffer
 * #3: len [in]     input length
 */
void conch_sha256_process(struct sha256_ctx *ctx, const uint8_t *s,
		size_t len)
{
#define BLOCKSIZE SHA256_BLOCKSIZE

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
			_sha256_compress(ctx, ctx->buf);
			n = 0;
			s += h;
			len -= h;
		case 0:
			while (len >= BLOCKSIZE) {
				/* processing */
				_sha256_compress(ctx, s);
				s += BLOCKSIZE;
				len -= BLOCKSIZE;
			}

			n = len;
			if (n)
				conch_memcpy(ctx->buf, s, n);
	}
	ctx->count = n;
}

/* @func: conch_sha256_finish
 * #desc:
 *    sha256 process the remaining bytes in the buffer and end.
 *
 * #1: ctx [in/out] sha256 struct context
 * #2: len [in]     total length
 */
void conch_sha256_finish(struct sha256_ctx *ctx, uint64_t len)
{
	/* padding */
	uint8_t padbuf[SHA256_BLOCKSIZE];
	conch_memset(padbuf, 0, sizeof(padbuf));
	padbuf[0] = 0x80;
	conch_sha256_process(ctx, padbuf, 1 + ((119 - (len % 64)) % 64));

	/* bit length */
	len *= 8;
	for (int32_t i = 0; i < 8; i++) {
		ctx->buf[63 - i] = (uint8_t)len;
		len >>= 8;
	}
	_sha256_compress(ctx, ctx->buf);

	for (int32_t i = 0; i < 8; i++)
		ctx->state[i] = BSWAP32(ctx->state[i]);
}

/* @func: conch_sha256
 * #desc:
 *    sha256 single-time processing function.
 *
 * #1: ctx [in/out] sha256 struct context
 * #2: s   [in]     input buffer
 * #3: len [in]     input length
 */
void conch_sha256(struct sha256_ctx *ctx, const uint8_t *s, size_t len)
{
	conch_sha256_process(ctx, s, len);
	conch_sha256_finish(ctx, len);
}
