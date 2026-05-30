/* @file: curve448.c
 * #desc:
 *    The implementations of curve448 ecdh and eddsa.
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
#include <conch/sha3.h>
#include <conch/ecc.h>


/* P = 2^448 - 2^224 - 1 */
static const uint32_t _fp448_p[14] = {
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xfffffffe,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffffffff
	};

/* B = 5 */
static const uint32_t _x448_b[14] = {
	5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

/* D = -39081 % P */
static const uint32_t _ed448_d[14] = {
	0xffff6756, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xfffffffe,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffffffff
	};

/* one = 1 */
static const uint32_t _ed448_one[14] = {
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

/* p14 = (P + 1) / 4 */
static const uint32_t _ed448_p14[14] = {
	0, 0, 0, 0, 0, 0,
	0xc0000000, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0x3fffffff
	};

/* Q = 2^446 - 0x8335dc163bb124b65129c96fde933d8d723a70aadc873d6d54a7bb0d */
static const uint32_t _sc448_q[14] = {
	0xab5844f3, 0x2378c292, 0x8dc58f55, 0x216cc272,
	0xaed63690, 0xc44edb49, 0x7cca23e9, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0x3fffffff
	};

/*
 * By = 298819210078481492676017930443930673437544040154080242095928241372
 *      331506189835876003536878655418784733982303233503462500531545062832
 *      660
 * Bx = rec_x(By, 0)
 *   224580040295924300187604334099896036246789641632564134246125461686950
 *   415467406032909029192869357953282578032075146446173674602635247710
 * Bz = 1
 */
struct ed448_point {
	uint32_t x[14];
	uint32_t y[14];
	uint32_t z[14];
};

static const struct ed448_point _ed448_base = {
	{
		0xc70cc05e, 0x2626a82b, 0x8b00938e, 0x433b80e1,
		0x2ab66511, 0x12ae1af7, 0xa3d3a464, 0xea6de324,
		0x470f1767, 0x9e146570, 0x22bf36da, 0x221d15a6,
		0x6bed0ded, 0x4f1970c6
	},
	{
		0xf230fa14, 0x9808795b, 0x4ed7c8ad, 0xfdbd132c,
		0xe67c39c4, 0x3ad3ff1c, 0x05a0c2d7, 0x87789c1e,
		0x6ca39840, 0x4bea7373, 0x56c9c762, 0x88762037,
		0x6eb6bc24, 0x693f4671
	},
	{
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	}
	};

/*
 * Base Point Order
 *   q = 2^446 - 0x8335dc163bb124b65129c96fde933d8d723a70aadc873d6d54a7bb0d
 *   P1..P4 = q * (1..4)
 */
static const uint32_t _sc448_BPO[5][14] = {
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	},
	{
		0xab5844f3, 0x2378c292, 0x8dc58f55, 0x216cc272,
		0xaed63690, 0xc44edb49, 0x7cca23e9, 0xffffffff,
		0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
		0xffffffff, 0x3fffffff
	},
	{
		0x56b089e6, 0x46f18525, 0x1b8b1eaa, 0x42d984e5,
		0x5dac6d20, 0x889db693, 0xf99447d3, 0xfffffffe,
		0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
		0xffffffff, 0x7fffffff
	},
	{
		0x0208ced9, 0x6a6a47b8, 0xa950adff, 0x64464757,
		0x0c82a3b0, 0x4cec91dd, 0x765e6bbd, 0xfffffffe,
		0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
		0xffffffff, 0xbfffffff
	},
	{
		0xad6113cc, 0x8de30a4a, 0x37163d54, 0x85b309ca,
		0xbb58da40, 0x113b6d26, 0xf3288fa7, 0xfffffffd,
		0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
		0xffffffff, 0xffffffff
	}
	};

/*
 * R = 2^448 % q
 *   55272267239580461408029546994061707521346769899528714439578190015540
 */
static const uint32_t _sc448_R[14] = {
	0x529eec34, 0x721cf5b5, 0xc8e9c2ab, 0x7a4cf635,
	0x44a725bf, 0xeec492d9, 0x0cd77058,
	2, 0, 0, 0, 0, 0, 0
	};

/* ed448 context prefix */
#define ED448_CTX_LEN 10

static const uint8_t _ed448_ctx[] = "SigEd448\0\0";


/* @func: _fp448_swap (static)
 * #desc:
 *    constant-time the numerical swap.
 *
 * #1: a   [in/out]  number
 * #2: b   [in/out]  number
 * #3: bit [in]      low bit (0: keep, 1: swap)
 */
static void _fp448_swap(uint32_t a[14], uint32_t b[14], uint32_t bit)
{
	bit = (~bit & 1) - 1;
	for (int32_t i = 0; i < 14; i++) {
		uint32_t t = bit & (a[i] ^ b[i]);
		a[i] ^= t;
		b[i] ^= t;
	}
}

/* @func: _fp448_iszero (static)
 * #desc:
 *    check if a number is zero in constant-time.
 *
 * #1: a [in]  number
 * #r:   [ret] 0: non-zero, 1: is zero
 */
static uint32_t _fp448_iszero(const uint32_t a[14])
{
	uint64_t r = 0;
	for (int32_t i = 0; i < 14; i++)
		r |= a[i];

	return ((r - 1) >> 32) & 1;
}

/* @func: _fp448_add (static)
 * #desc:
 *    prime field addition.
 *
 * #1: r [out] sum
 * #2: a [in]  addend
 * #3: b [in]  addend
 */
static void _fp448_add(uint32_t r[14],
		const uint32_t a[14], const uint32_t b[14])
{
#if 0

	uint32_t carry = 0, carry2 = 0;
	uint64_t tmp = 0;

	/* r = a + b */
	for (int32_t i = 0; i < 14; i++) {
		tmp = (uint64_t)a[i] + b[i] + carry2;
		r[i] = tmp & 0xffffffff;
		carry2 = tmp >> 32;
	}

	/* NOTE: 2^448 == 2^224+1 (mod 2^448-2^224-1) */

	/* r = r % p modular reduction */
	carry += carry2;
	for (int32_t i = 0; i < 7; i++) {
		tmp = (uint64_t)r[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	carry += carry2;
	for (int32_t i = 7; i < 14; i++) {
		tmp = (uint64_t)r[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

#else

	uint64_t t0 = (uint64_t)a[0] + b[0];
	uint64_t t1 = (uint64_t)a[1] + b[1];
	uint64_t t2 = (uint64_t)a[2] + b[2];
	uint64_t t3 = (uint64_t)a[3] + b[3];
	uint64_t t4 = (uint64_t)a[4] + b[4];
	uint64_t t5 = (uint64_t)a[5] + b[5];
	uint64_t t6 = (uint64_t)a[6] + b[6];
	uint64_t t7 = (uint64_t)a[7] + b[7];
	uint64_t t8 = (uint64_t)a[8] + b[8];
	uint64_t t9 = (uint64_t)a[9] + b[9];
	uint64_t t10 = (uint64_t)a[10] + b[10];
	uint64_t t11 = (uint64_t)a[11] + b[11];
	uint64_t t12 = (uint64_t)a[12] + b[12];
	uint64_t t13 = (uint64_t)a[13] + b[13];
	t1 += t0 >> 32;
	t2 += t1 >> 32;
	t3 += t2 >> 32;
	t4 += t3 >> 32;
	t5 += t4 >> 32;
	t6 += t5 >> 32;
	t7 += t6 >> 32;
	t8 += t7 >> 32;
	t9 += t8 >> 32;
	t10 += t9 >> 32;
	t11 += t10 >> 32;
	t12 += t11 >> 32;
	t13 += t12 >> 32;
	uint32_t carry = t13 >> 32;
	t0 &= 0xffffffff;
	t1 &= 0xffffffff;
	t2 &= 0xffffffff;
	t3 &= 0xffffffff;
	t4 &= 0xffffffff;
	t5 &= 0xffffffff;
	t6 &= 0xffffffff;
	t7 &= 0xffffffff;
	t8 &= 0xffffffff;
	t9 &= 0xffffffff;
	t10 &= 0xffffffff;
	t11 &= 0xffffffff;
	t12 &= 0xffffffff;
	t13 &= 0xffffffff;

	t0 += carry;
	t1 += t0 >> 32;
	t2 += t1 >> 32;
	t3 += t2 >> 32;
	t4 += t3 >> 32;
	t5 += t4 >> 32;
	t6 += t5 >> 32;
	t7 += carry;
	t7 += t6 >> 32;
	t8 += t7 >> 32;
	t9 += t8 >> 32;
	t10 += t9 >> 32;
	t11 += t10 >> 32;
	t12 += t11 >> 32;
	t13 += t12 >> 32;
	r[0] = t0 & 0xffffffff;
	r[1] = t1 & 0xffffffff;
	r[2] = t2 & 0xffffffff;
	r[3] = t3 & 0xffffffff;
	r[4] = t4 & 0xffffffff;
	r[5] = t5 & 0xffffffff;
	r[6] = t6 & 0xffffffff;
	r[7] = t7 & 0xffffffff;
	r[8] = t8 & 0xffffffff;
	r[9] = t9 & 0xffffffff;
	r[10] = t10 & 0xffffffff;
	r[11] = t11 & 0xffffffff;
	r[12] = t12 & 0xffffffff;
	r[13] = t13 & 0xffffffff;

#endif
}

/* @func: _fp448_sub (static)
 * #desc:
 *    prime field subtraction.
 *
 * #1: r [out] difference
 * #2: a [in]  minuend
 * #3: b [in]  subtract
 */
static void _fp448_sub(uint32_t r[14],
		const uint32_t a[14], const uint32_t b[14])
{
#if 0

	uint32_t carry = 0, carry2 = 0;
	uint64_t tmp = 0;

	/* r = a - b */
	for (int32_t i = 0; i < 14; i++) {
		tmp = (uint64_t)a[i] - b[i] + (int32_t)carry2;
		r[i] = tmp & 0xffffffff;
		carry2 = tmp >> 32;
	}

	/* NOTE: 2^448 == 2^224+1 (mod 2^448-2^224-1) */

	/* r = r % p modular reduction */
	tmp = (uint64_t)r[0] - (carry2 & 1);
	r[0] = tmp & 0xffffffff;
	carry = tmp >> 32;
	for (int32_t i = 1; i < 7; i++) {
		tmp = (uint64_t)r[i] + (int32_t)carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	tmp = (uint64_t)r[7] - (carry2 & 1);
	r[7] = tmp & 0xffffffff;
	carry = tmp >> 32;
	for (int32_t i = 8; i < 14; i++) {
		tmp = (uint64_t)r[i] + (int32_t)carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

#else

	uint64_t t0 = (uint64_t)a[0] - b[0];
	uint64_t t1 = (uint64_t)a[1] - b[1];
	uint64_t t2 = (uint64_t)a[2] - b[2];
	uint64_t t3 = (uint64_t)a[3] - b[3];
	uint64_t t4 = (uint64_t)a[4] - b[4];
	uint64_t t5 = (uint64_t)a[5] - b[5];
	uint64_t t6 = (uint64_t)a[6] - b[6];
	uint64_t t7 = (uint64_t)a[7] - b[7];
	uint64_t t8 = (uint64_t)a[8] - b[8];
	uint64_t t9 = (uint64_t)a[9] - b[9];
	uint64_t t10 = (uint64_t)a[10] - b[10];
	uint64_t t11 = (uint64_t)a[11] - b[11];
	uint64_t t12 = (uint64_t)a[12] - b[12];
	uint64_t t13 = (uint64_t)a[13] - b[13];
	t1 += (int32_t)(t0 >> 32);
	t2 += (int32_t)(t1 >> 32);
	t3 += (int32_t)(t2 >> 32);
	t4 += (int32_t)(t3 >> 32);
	t5 += (int32_t)(t4 >> 32);
	t6 += (int32_t)(t5 >> 32);
	t7 += (int32_t)(t6 >> 32);
	t8 += (int32_t)(t7 >> 32);
	t9 += (int32_t)(t8 >> 32);
	t10 += (int32_t)(t9 >> 32);
	t11 += (int32_t)(t10 >> 32);
	t12 += (int32_t)(t11 >> 32);
	t13 += (int32_t)(t12 >> 32);
	uint32_t carry = t13 >> 32;
	t0 &= 0xffffffff;
	t1 &= 0xffffffff;
	t2 &= 0xffffffff;
	t3 &= 0xffffffff;
	t4 &= 0xffffffff;
	t5 &= 0xffffffff;
	t6 &= 0xffffffff;
	t7 &= 0xffffffff;
	t8 &= 0xffffffff;
	t9 &= 0xffffffff;
	t10 &= 0xffffffff;
	t11 &= 0xffffffff;
	t12 &= 0xffffffff;
	t13 &= 0xffffffff;

	t0 -= (carry & 1);
	t1 += t0 >> 32;
	t2 += t1 >> 32;
	t3 += t2 >> 32;
	t4 += t3 >> 32;
	t5 += t4 >> 32;
	t6 += t5 >> 32;
	t7 -= (carry & 1);
	t8 += t7 >> 32;
	t9 += t8 >> 32;
	t10 += t9 >> 32;
	t11 += t10 >> 32;
	t12 += t11 >> 32;
	t13 += t12 >> 32;
	r[0] = t0 & 0xffffffff;
	r[1] = t1 & 0xffffffff;
	r[2] = t2 & 0xffffffff;
	r[3] = t3 & 0xffffffff;
	r[4] = t4 & 0xffffffff;
	r[5] = t5 & 0xffffffff;
	r[6] = t6 & 0xffffffff;
	r[7] = t7 & 0xffffffff;
	r[8] = t8 & 0xffffffff;
	r[9] = t9 & 0xffffffff;
	r[10] = t10 & 0xffffffff;
	r[11] = t11 & 0xffffffff;
	r[12] = t12 & 0xffffffff;
	r[13] = t13 & 0xffffffff;

#endif
}

/* @func: _fp448_mul (static)
 * #desc:
 *    prime field multiplication.
 *
 * #1: r [out] product
 * #2: a [in]  multiplicand
 * #3: b [in]  multiplier
 */
static void _fp448_mul(uint32_t r[14],
		const uint32_t a[14], const uint32_t b[14])
{
#if 0

	uint32_t rr[28];
	uint32_t carry = 0, carry2 = 0;
	uint64_t tmp = 0;

	/* XXX: karatsuba optimization */

	/* rr = a * b */
	for (int32_t i = 0; i < 28; i++)
		rr[i] = 0;

	for (int32_t i = 0; i < 14; i++) {
		tmp = (uint64_t)a[0] * b[i] + carry2;
		rr[i] = tmp & 0xffffffff;
		carry2 = tmp >> 32;
	}
	rr[14] = carry2;

	for (int32_t i = 1; i < 14; i++) {
		carry2 = 0;
		for (int32_t j = 0; j < 14; j++) {
			tmp = (uint64_t)a[i] * b[j] + rr[i + j] + carry2;
			rr[i + j] = tmp & 0xffffffff;
			carry2 = tmp >> 32;
		}
		rr[i + 14] = carry2;
	}

	/* NOTE: 2^448 == 2^224+1 (mod 2^448-2^224-1) */

	/* r = rr % p modular reduction */
	carry2 = 0;
	for (int32_t i = 0; i < 7; i++) { /* fold */
		tmp = (uint64_t)rr[i] + rr[i + 14] + rr[i + 21] + carry2;
		r[i] = tmp & 0xffffffff;
		carry2 = tmp >> 32;
	}
	for (int32_t i = 7; i < 14; i++) {
		tmp = (uint64_t)rr[i] + rr[i + 14] + rr[i + 14]
			+ rr[i + 7] + carry2;
		r[i] = tmp & 0xffffffff;
		carry2 = tmp >> 32;
	}

	carry += carry2;
	for (int32_t i = 0; i < 7; i++) {
		tmp = (uint64_t)r[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	carry += carry2;
	for (int32_t i = 7; i < 14; i++) {
		tmp = (uint64_t)r[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

#else

	uint64_t a0 = a[0];
	uint64_t a1 = a[1];
	uint64_t a2 = a[2];
	uint64_t a3 = a[3];
	uint64_t a4 = a[4];
	uint64_t a5 = a[5];
	uint64_t a6 = a[6];
	uint64_t a7 = a[7];
	uint64_t a8 = a[8];
	uint64_t a9 = a[9];
	uint64_t a10 = a[10];
	uint64_t a11 = a[11];
	uint64_t a12 = a[12];
	uint64_t a13 = a[13];
	uint64_t b0 = b[0];
	uint64_t b1 = b[1];
	uint64_t b2 = b[2];
	uint64_t b3 = b[3];
	uint64_t b4 = b[4];
	uint64_t b5 = b[5];
	uint64_t b6 = b[6];
	uint64_t b7 = b[7];
	uint64_t b8 = b[8];
	uint64_t b9 = b[9];
	uint64_t b10 = b[10];
	uint64_t b11 = b[11];
	uint64_t b12 = b[12];
	uint64_t b13 = b[13];

	uint64_t t0 = a0 * b0;
	uint64_t t1 = a0 * b1;
	uint64_t t2 = a0 * b2;
	uint64_t t3 = a0 * b3;
	uint64_t t4 = a0 * b4;
	uint64_t t5 = a0 * b5;
	uint64_t t6 = a0 * b6;
	uint64_t t7 = a0 * b7;
	uint64_t t8 = a0 * b8;
	uint64_t t9 = a0 * b9;
	uint64_t t10 = a0 * b10;
	uint64_t t11 = a0 * b11;
	uint64_t t12 = a0 * b12;
	uint64_t t13 = a0 * b13;
	t1 += t0 >> 32;
	t2 += t1 >> 32;
	t3 += t2 >> 32;
	t4 += t3 >> 32;
	t5 += t4 >> 32;
	t6 += t5 >> 32;
	t7 += t6 >> 32;
	t8 += t7 >> 32;
	t9 += t8 >> 32;
	t10 += t9 >> 32;
	t11 += t10 >> 32;
	t12 += t11 >> 32;
	t13 += t12 >> 32;
	uint64_t t14 = t13 >> 32;
	t0 &= 0xffffffff;
	t1 &= 0xffffffff;
	t2 &= 0xffffffff;
	t3 &= 0xffffffff;
	t4 &= 0xffffffff;
	t5 &= 0xffffffff;
	t6 &= 0xffffffff;
	t7 &= 0xffffffff;
	t8 &= 0xffffffff;
	t9 &= 0xffffffff;
	t10 &= 0xffffffff;
	t11 &= 0xffffffff;
	t12 &= 0xffffffff;
	t13 &= 0xffffffff;

	t1 += a1 * b0;
	t2 += a1 * b1;
	t3 += a1 * b2;
	t4 += a1 * b3;
	t5 += a1 * b4;
	t6 += a1 * b5;
	t7 += a1 * b6;
	t8 += a1 * b7;
	t9 += a1 * b8;
	t10 += a1 * b9;
	t11 += a1 * b10;
	t12 += a1 * b11;
	t13 += a1 * b12;
	t14 += a1 * b13;
	t2 += t1 >> 32;
	t3 += t2 >> 32;
	t4 += t3 >> 32;
	t5 += t4 >> 32;
	t6 += t5 >> 32;
	t7 += t6 >> 32;
	t8 += t7 >> 32;
	t9 += t8 >> 32;
	t10 += t9 >> 32;
	t11 += t10 >> 32;
	t12 += t11 >> 32;
	t13 += t12 >> 32;
	t14 += t13 >> 32;
	uint64_t t15 = t14 >> 32;
	t1 &= 0xffffffff;
	t2 &= 0xffffffff;
	t3 &= 0xffffffff;
	t4 &= 0xffffffff;
	t5 &= 0xffffffff;
	t6 &= 0xffffffff;
	t7 &= 0xffffffff;
	t8 &= 0xffffffff;
	t9 &= 0xffffffff;
	t10 &= 0xffffffff;
	t11 &= 0xffffffff;
	t12 &= 0xffffffff;
	t13 &= 0xffffffff;
	t14 &= 0xffffffff;
	t2 += a2 * b0;
	t3 += a2 * b1;
	t4 += a2 * b2;
	t5 += a2 * b3;
	t6 += a2 * b4;
	t7 += a2 * b5;
	t8 += a2 * b6;
	t9 += a2 * b7;
	t10 += a2 * b8;
	t11 += a2 * b9;
	t12 += a2 * b10;
	t13 += a2 * b11;
	t14 += a2 * b12;
	t15 += a2 * b13;
	t3 += t2 >> 32;
	t4 += t3 >> 32;
	t5 += t4 >> 32;
	t6 += t5 >> 32;
	t7 += t6 >> 32;
	t8 += t7 >> 32;
	t9 += t8 >> 32;
	t10 += t9 >> 32;
	t11 += t10 >> 32;
	t12 += t11 >> 32;
	t13 += t12 >> 32;
	t14 += t13 >> 32;
	t15 += t14 >> 32;
	uint64_t t16 = t15 >> 32;
	t2 &= 0xffffffff;
	t3 &= 0xffffffff;
	t4 &= 0xffffffff;
	t5 &= 0xffffffff;
	t6 &= 0xffffffff;
	t7 &= 0xffffffff;
	t8 &= 0xffffffff;
	t9 &= 0xffffffff;
	t10 &= 0xffffffff;
	t11 &= 0xffffffff;
	t12 &= 0xffffffff;
	t13 &= 0xffffffff;
	t14 &= 0xffffffff;
	t15 &= 0xffffffff;
	t3 += a3 * b0;
	t4 += a3 * b1;
	t5 += a3 * b2;
	t6 += a3 * b3;
	t7 += a3 * b4;
	t8 += a3 * b5;
	t9 += a3 * b6;
	t10 += a3 * b7;
	t11 += a3 * b8;
	t12 += a3 * b9;
	t13 += a3 * b10;
	t14 += a3 * b11;
	t15 += a3 * b12;
	t16 += a3 * b13;
	t4 += t3 >> 32;
	t5 += t4 >> 32;
	t6 += t5 >> 32;
	t7 += t6 >> 32;
	t8 += t7 >> 32;
	t9 += t8 >> 32;
	t10 += t9 >> 32;
	t11 += t10 >> 32;
	t12 += t11 >> 32;
	t13 += t12 >> 32;
	t14 += t13 >> 32;
	t15 += t14 >> 32;
	t16 += t15 >> 32;
	uint64_t t17 = t16 >> 32;
	t3 &= 0xffffffff;
	t4 &= 0xffffffff;
	t5 &= 0xffffffff;
	t6 &= 0xffffffff;
	t7 &= 0xffffffff;
	t8 &= 0xffffffff;
	t9 &= 0xffffffff;
	t10 &= 0xffffffff;
	t11 &= 0xffffffff;
	t12 &= 0xffffffff;
	t13 &= 0xffffffff;
	t14 &= 0xffffffff;
	t15 &= 0xffffffff;
	t16 &= 0xffffffff;
	t4 += a4 * b0;
	t5 += a4 * b1;
	t6 += a4 * b2;
	t7 += a4 * b3;
	t8 += a4 * b4;
	t9 += a4 * b5;
	t10 += a4 * b6;
	t11 += a4 * b7;
	t12 += a4 * b8;
	t13 += a4 * b9;
	t14 += a4 * b10;
	t15 += a4 * b11;
	t16 += a4 * b12;
	t17 += a4 * b13;
	t5 += t4 >> 32;
	t6 += t5 >> 32;
	t7 += t6 >> 32;
	t8 += t7 >> 32;
	t9 += t8 >> 32;
	t10 += t9 >> 32;
	t11 += t10 >> 32;
	t12 += t11 >> 32;
	t13 += t12 >> 32;
	t14 += t13 >> 32;
	t15 += t14 >> 32;
	t16 += t15 >> 32;
	t17 += t16 >> 32;
	uint64_t t18 = t17 >> 32;
	t4 &= 0xffffffff;
	t5 &= 0xffffffff;
	t6 &= 0xffffffff;
	t7 &= 0xffffffff;
	t8 &= 0xffffffff;
	t9 &= 0xffffffff;
	t10 &= 0xffffffff;
	t11 &= 0xffffffff;
	t12 &= 0xffffffff;
	t13 &= 0xffffffff;
	t14 &= 0xffffffff;
	t15 &= 0xffffffff;
	t16 &= 0xffffffff;
	t17 &= 0xffffffff;
	t5 += a5 * b0;
	t6 += a5 * b1;
	t7 += a5 * b2;
	t8 += a5 * b3;
	t9 += a5 * b4;
	t10 += a5 * b5;
	t11 += a5 * b6;
	t12 += a5 * b7;
	t13 += a5 * b8;
	t14 += a5 * b9;
	t15 += a5 * b10;
	t16 += a5 * b11;
	t17 += a5 * b12;
	t18 += a5 * b13;
	t6 += t5 >> 32;
	t7 += t6 >> 32;
	t8 += t7 >> 32;
	t9 += t8 >> 32;
	t10 += t9 >> 32;
	t11 += t10 >> 32;
	t12 += t11 >> 32;
	t13 += t12 >> 32;
	t14 += t13 >> 32;
	t15 += t14 >> 32;
	t16 += t15 >> 32;
	t17 += t16 >> 32;
	t18 += t17 >> 32;
	uint64_t t19 = t18 >> 32;
	t5 &= 0xffffffff;
	t6 &= 0xffffffff;
	t7 &= 0xffffffff;
	t8 &= 0xffffffff;
	t9 &= 0xffffffff;
	t10 &= 0xffffffff;
	t11 &= 0xffffffff;
	t12 &= 0xffffffff;
	t13 &= 0xffffffff;
	t14 &= 0xffffffff;
	t15 &= 0xffffffff;
	t16 &= 0xffffffff;
	t17 &= 0xffffffff;
	t18 &= 0xffffffff;
	t6 += a6 * b0;
	t7 += a6 * b1;
	t8 += a6 * b2;
	t9 += a6 * b3;
	t10 += a6 * b4;
	t11 += a6 * b5;
	t12 += a6 * b6;
	t13 += a6 * b7;
	t14 += a6 * b8;
	t15 += a6 * b9;
	t16 += a6 * b10;
	t17 += a6 * b11;
	t18 += a6 * b12;
	t19 += a6 * b13;
	t7 += t6 >> 32;
	t8 += t7 >> 32;
	t9 += t8 >> 32;
	t10 += t9 >> 32;
	t11 += t10 >> 32;
	t12 += t11 >> 32;
	t13 += t12 >> 32;
	t14 += t13 >> 32;
	t15 += t14 >> 32;
	t16 += t15 >> 32;
	t17 += t16 >> 32;
	t18 += t17 >> 32;
	t19 += t18 >> 32;
	uint64_t t20 = t19 >> 32;
	t6 &= 0xffffffff;
	t7 &= 0xffffffff;
	t8 &= 0xffffffff;
	t9 &= 0xffffffff;
	t10 &= 0xffffffff;
	t11 &= 0xffffffff;
	t12 &= 0xffffffff;
	t13 &= 0xffffffff;
	t14 &= 0xffffffff;
	t15 &= 0xffffffff;
	t16 &= 0xffffffff;
	t17 &= 0xffffffff;
	t18 &= 0xffffffff;
	t19 &= 0xffffffff;
	t7 += a7 * b0;
	t8 += a7 * b1;
	t9 += a7 * b2;
	t10 += a7 * b3;
	t11 += a7 * b4;
	t12 += a7 * b5;
	t13 += a7 * b6;
	t14 += a7 * b7;
	t15 += a7 * b8;
	t16 += a7 * b9;
	t17 += a7 * b10;
	t18 += a7 * b11;
	t19 += a7 * b12;
	t20 += a7 * b13;
	t8 += t7 >> 32;
	t9 += t8 >> 32;
	t10 += t9 >> 32;
	t11 += t10 >> 32;
	t12 += t11 >> 32;
	t13 += t12 >> 32;
	t14 += t13 >> 32;
	t15 += t14 >> 32;
	t16 += t15 >> 32;
	t17 += t16 >> 32;
	t18 += t17 >> 32;
	t19 += t18 >> 32;
	t20 += t19 >> 32;
	uint64_t t21 = t20 >> 32;
	t7 &= 0xffffffff;
	t8 &= 0xffffffff;
	t9 &= 0xffffffff;
	t10 &= 0xffffffff;
	t11 &= 0xffffffff;
	t12 &= 0xffffffff;
	t13 &= 0xffffffff;
	t14 &= 0xffffffff;
	t15 &= 0xffffffff;
	t16 &= 0xffffffff;
	t17 &= 0xffffffff;
	t18 &= 0xffffffff;
	t19 &= 0xffffffff;
	t20 &= 0xffffffff;
	t8 += a8 * b0;
	t9 += a8 * b1;
	t10 += a8 * b2;
	t11 += a8 * b3;
	t12 += a8 * b4;
	t13 += a8 * b5;
	t14 += a8 * b6;
	t15 += a8 * b7;
	t16 += a8 * b8;
	t17 += a8 * b9;
	t18 += a8 * b10;
	t19 += a8 * b11;
	t20 += a8 * b12;
	t21 += a8 * b13;
	t9 += t8 >> 32;
	t10 += t9 >> 32;
	t11 += t10 >> 32;
	t12 += t11 >> 32;
	t13 += t12 >> 32;
	t14 += t13 >> 32;
	t15 += t14 >> 32;
	t16 += t15 >> 32;
	t17 += t16 >> 32;
	t18 += t17 >> 32;
	t19 += t18 >> 32;
	t20 += t19 >> 32;
	t21 += t20 >> 32;
	uint64_t t22 = t21 >> 32;
	t8 &= 0xffffffff;
	t9 &= 0xffffffff;
	t10 &= 0xffffffff;
	t11 &= 0xffffffff;
	t12 &= 0xffffffff;
	t13 &= 0xffffffff;
	t14 &= 0xffffffff;
	t15 &= 0xffffffff;
	t16 &= 0xffffffff;
	t17 &= 0xffffffff;
	t18 &= 0xffffffff;
	t19 &= 0xffffffff;
	t20 &= 0xffffffff;
	t21 &= 0xffffffff;
	t9 += a9 * b0;
	t10 += a9 * b1;
	t11 += a9 * b2;
	t12 += a9 * b3;
	t13 += a9 * b4;
	t14 += a9 * b5;
	t15 += a9 * b6;
	t16 += a9 * b7;
	t17 += a9 * b8;
	t18 += a9 * b9;
	t19 += a9 * b10;
	t20 += a9 * b11;
	t21 += a9 * b12;
	t22 += a9 * b13;
	t10 += t9 >> 32;
	t11 += t10 >> 32;
	t12 += t11 >> 32;
	t13 += t12 >> 32;
	t14 += t13 >> 32;
	t15 += t14 >> 32;
	t16 += t15 >> 32;
	t17 += t16 >> 32;
	t18 += t17 >> 32;
	t19 += t18 >> 32;
	t20 += t19 >> 32;
	t21 += t20 >> 32;
	t22 += t21 >> 32;
	uint64_t t23 = t22 >> 32;
	t9 &= 0xffffffff;
	t10 &= 0xffffffff;
	t11 &= 0xffffffff;
	t12 &= 0xffffffff;
	t13 &= 0xffffffff;
	t14 &= 0xffffffff;
	t15 &= 0xffffffff;
	t16 &= 0xffffffff;
	t17 &= 0xffffffff;
	t18 &= 0xffffffff;
	t19 &= 0xffffffff;
	t20 &= 0xffffffff;
	t21 &= 0xffffffff;
	t22 &= 0xffffffff;
	t10 += a10 * b0;
	t11 += a10 * b1;
	t12 += a10 * b2;
	t13 += a10 * b3;
	t14 += a10 * b4;
	t15 += a10 * b5;
	t16 += a10 * b6;
	t17 += a10 * b7;
	t18 += a10 * b8;
	t19 += a10 * b9;
	t20 += a10 * b10;
	t21 += a10 * b11;
	t22 += a10 * b12;
	t23 += a10 * b13;
	t11 += t10 >> 32;
	t12 += t11 >> 32;
	t13 += t12 >> 32;
	t14 += t13 >> 32;
	t15 += t14 >> 32;
	t16 += t15 >> 32;
	t17 += t16 >> 32;
	t18 += t17 >> 32;
	t19 += t18 >> 32;
	t20 += t19 >> 32;
	t21 += t20 >> 32;
	t22 += t21 >> 32;
	t23 += t22 >> 32;
	uint64_t t24 = t23 >> 32;
	t10 &= 0xffffffff;
	t11 &= 0xffffffff;
	t12 &= 0xffffffff;
	t13 &= 0xffffffff;
	t14 &= 0xffffffff;
	t15 &= 0xffffffff;
	t16 &= 0xffffffff;
	t17 &= 0xffffffff;
	t18 &= 0xffffffff;
	t19 &= 0xffffffff;
	t20 &= 0xffffffff;
	t21 &= 0xffffffff;
	t22 &= 0xffffffff;
	t23 &= 0xffffffff;
	t11 += a11 * b0;
	t12 += a11 * b1;
	t13 += a11 * b2;
	t14 += a11 * b3;
	t15 += a11 * b4;
	t16 += a11 * b5;
	t17 += a11 * b6;
	t18 += a11 * b7;
	t19 += a11 * b8;
	t20 += a11 * b9;
	t21 += a11 * b10;
	t22 += a11 * b11;
	t23 += a11 * b12;
	t24 += a11 * b13;
	t12 += t11 >> 32;
	t13 += t12 >> 32;
	t14 += t13 >> 32;
	t15 += t14 >> 32;
	t16 += t15 >> 32;
	t17 += t16 >> 32;
	t18 += t17 >> 32;
	t19 += t18 >> 32;
	t20 += t19 >> 32;
	t21 += t20 >> 32;
	t22 += t21 >> 32;
	t23 += t22 >> 32;
	t24 += t23 >> 32;
	uint64_t t25 = t24 >> 32;
	t11 &= 0xffffffff;
	t12 &= 0xffffffff;
	t13 &= 0xffffffff;
	t14 &= 0xffffffff;
	t15 &= 0xffffffff;
	t16 &= 0xffffffff;
	t17 &= 0xffffffff;
	t18 &= 0xffffffff;
	t19 &= 0xffffffff;
	t20 &= 0xffffffff;
	t21 &= 0xffffffff;
	t22 &= 0xffffffff;
	t23 &= 0xffffffff;
	t24 &= 0xffffffff;
	t12 += a12 * b0;
	t13 += a12 * b1;
	t14 += a12 * b2;
	t15 += a12 * b3;
	t16 += a12 * b4;
	t17 += a12 * b5;
	t18 += a12 * b6;
	t19 += a12 * b7;
	t20 += a12 * b8;
	t21 += a12 * b9;
	t22 += a12 * b10;
	t23 += a12 * b11;
	t24 += a12 * b12;
	t25 += a12 * b13;
	t13 += t12 >> 32;
	t14 += t13 >> 32;
	t15 += t14 >> 32;
	t16 += t15 >> 32;
	t17 += t16 >> 32;
	t18 += t17 >> 32;
	t19 += t18 >> 32;
	t20 += t19 >> 32;
	t21 += t20 >> 32;
	t22 += t21 >> 32;
	t23 += t22 >> 32;
	t24 += t23 >> 32;
	t25 += t24 >> 32;
	uint64_t t26 = t25 >> 32;
	t12 &= 0xffffffff;
	t13 &= 0xffffffff;
	t14 &= 0xffffffff;
	t15 &= 0xffffffff;
	t16 &= 0xffffffff;
	t17 &= 0xffffffff;
	t18 &= 0xffffffff;
	t19 &= 0xffffffff;
	t20 &= 0xffffffff;
	t21 &= 0xffffffff;
	t22 &= 0xffffffff;
	t23 &= 0xffffffff;
	t24 &= 0xffffffff;
	t25 &= 0xffffffff;
	t13 += a13 * b0;
	t14 += a13 * b1;
	t15 += a13 * b2;
	t16 += a13 * b3;
	t17 += a13 * b4;
	t18 += a13 * b5;
	t19 += a13 * b6;
	t20 += a13 * b7;
	t21 += a13 * b8;
	t22 += a13 * b9;
	t23 += a13 * b10;
	t24 += a13 * b11;
	t25 += a13 * b12;
	t26 += a13 * b13;
	t14 += t13 >> 32;
	t15 += t14 >> 32;
	t16 += t15 >> 32;
	t17 += t16 >> 32;
	t18 += t17 >> 32;
	t19 += t18 >> 32;
	t20 += t19 >> 32;
	t21 += t20 >> 32;
	t22 += t21 >> 32;
	t23 += t22 >> 32;
	t24 += t23 >> 32;
	t25 += t24 >> 32;
	t26 += t25 >> 32;
	uint64_t t27 = t26 >> 32;
	t13 &= 0xffffffff;
	t14 &= 0xffffffff;
	t15 &= 0xffffffff;
	t16 &= 0xffffffff;
	t17 &= 0xffffffff;
	t18 &= 0xffffffff;
	t19 &= 0xffffffff;
	t20 &= 0xffffffff;
	t21 &= 0xffffffff;
	t22 &= 0xffffffff;
	t23 &= 0xffffffff;
	t24 &= 0xffffffff;
	t25 &= 0xffffffff;
	t26 &= 0xffffffff;

	t0 += t14 + t21;
	t1 += t15 + t22;
	t2 += t16 + t23;
	t3 += t17 + t24;
	t4 += t18 + t25;
	t5 += t19 + t26;
	t6 += t20 + t27;
	t7 += t21 + t21 + t14;
	t8 += t22 + t22 + t15;
	t9 += t23 + t23 + t16;
	t10 += t24 + t24 + t17;
	t11 += t25 + t25 + t18;
	t12 += t26 + t26 + t19;
	t13 += t27 + t27 + t20;
	t1 += t0 >> 32;
	t2 += t1 >> 32;
	t3 += t2 >> 32;
	t4 += t3 >> 32;
	t5 += t4 >> 32;
	t6 += t5 >> 32;
	t7 += t6 >> 32;
	t8 += t7 >> 32;
	t9 += t8 >> 32;
	t10 += t9 >> 32;
	t11 += t10 >> 32;
	t12 += t11 >> 32;
	t13 += t12 >> 32;
	uint32_t carry = t13 >> 32;
	t0 &= 0xffffffff;
	t1 &= 0xffffffff;
	t2 &= 0xffffffff;
	t3 &= 0xffffffff;
	t4 &= 0xffffffff;
	t5 &= 0xffffffff;
	t6 &= 0xffffffff;
	t7 &= 0xffffffff;
	t8 &= 0xffffffff;
	t9 &= 0xffffffff;
	t10 &= 0xffffffff;
	t11 &= 0xffffffff;
	t12 &= 0xffffffff;
	t13 &= 0xffffffff;

	t0 += carry;
	t1 += t0 >> 32;
	t2 += t1 >> 32;
	t3 += t2 >> 32;
	t4 += t3 >> 32;
	t5 += t4 >> 32;
	t6 += t5 >> 32;
	t7 += carry;
	t7 += t6 >> 32;
	t8 += t7 >> 32;
	t9 += t8 >> 32;
	t10 += t9 >> 32;
	t11 += t10 >> 32;
	t12 += t11 >> 32;
	t13 += t12 >> 32;
	r[0] = t0 & 0xffffffff;
	r[1] = t1 & 0xffffffff;
	r[2] = t2 & 0xffffffff;
	r[3] = t3 & 0xffffffff;
	r[4] = t4 & 0xffffffff;
	r[5] = t5 & 0xffffffff;
	r[6] = t6 & 0xffffffff;
	r[7] = t7 & 0xffffffff;
	r[8] = t8 & 0xffffffff;
	r[9] = t9 & 0xffffffff;
	r[10] = t10 & 0xffffffff;
	r[11] = t11 & 0xffffffff;
	r[12] = t12 & 0xffffffff;
	r[13] = t13 & 0xffffffff;

#endif
}

/* @func: _fp448_mod (static)
 * #desc:
 *    prime field modular reduction (norm).
 *
 * #1: r [in/out] number
 */
static void _fp448_mod(uint32_t r[14])
{
	uint32_t carry = 0, carry2 = 0;
	uint64_t tmp = 0;

	/* NOTE: 2^448 == 2^224+1 (mod 2^448-2^224-1) */

	/* if r < p : adjust carry2 */
	carry2 += 1;
	for (int32_t i = 0; i < 7; i++) {
		tmp = (uint64_t)r[i] + carry2;
		carry2 = tmp >> 32;
	}

	carry2 += 1;
	for (int32_t i = 7; i < 14; i++) {
		tmp = (uint64_t)r[i] + carry2;
		carry2 = tmp >> 32;
	}

	/* r = r % p modular reduction */
	carry += carry2;
	for (int32_t i = 0; i < 7; i++) {
		tmp = (uint64_t)r[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	carry += carry2;
	for (int32_t i = 7; i < 14; i++) {
		tmp = (uint64_t)r[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}
}

/* @func: _fp448_inv (static)
 * #desc:
 *    modular inversion based on fermat's little theorem.
 *
 * #1: r [out] inverse modulus
 * #2: z [in]  number
 */
static void _fp448_inv(uint32_t r[14], const uint32_t z[14])
{
	uint32_t t[14];

	_fp448_mul(t, z, z);
	_fp448_mul(t, t, z);
	for (int32_t i = 0; i < 221; i++) {
		_fp448_mul(t, t, t);
		_fp448_mul(t, t, z);
	}

	_fp448_mul(t, t, t);
	for (int32_t i = 0; i < 222; i++) {
		_fp448_mul(t, t, t);
		_fp448_mul(t, t, z);
	}

	_fp448_mul(t, t, t);
	_fp448_mul(t, t, t);
	_fp448_mul(r, t, z);
}

/* @func: _fp448_mul39081 (static)
 * #desc:
 *    multiplier/39081 and addition (x448).
 *
 * #1: r [out] product
 * #2: a [in]  multiplicand
 * #3: b [in]  addend
 */
static void _fp448_mul39081(uint32_t r[14],
		const uint32_t a[14], const uint32_t b[14])
{
	uint32_t carry = 0, carry2 = 0;
	uint64_t tmp = 0;

	/* r = a * 39081 + b, (156326 - 2) / 4 = 39081 */
	for (int32_t i = 0; i < 14; i++) {
		tmp = (uint64_t)a[i] * 39081 + b[i] + carry2;
		r[i] = tmp & 0xffffffff;
		carry2 = tmp >> 32;
	}

	/* NOTE: 2^448 == 2^224+1 (mod 2^448-2^224-1) */

	/* r = rr % p modular reduction */
	carry += carry2;
	for (int32_t i = 0; i < 7; i++) {
		tmp = (uint64_t)r[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	carry += carry2;
	for (int32_t i = 7; i < 14; i++) {
		tmp = (uint64_t)r[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}
}

/* @func: _fp448_pow (static)
 * #desc:
 *    fast exponentiation in constant-time.
 *
 * #1: r [out] result
 * #2: a [in]  number
 * #3: b [in]  exponential
 */
static void _fp448_pow(uint32_t r[14],
		const uint32_t a[14], const uint32_t b[14])
{
	uint32_t x[14], t[14], e[14];
	for (int32_t i = 0; i < 14; i++) {
		x[i] = a[i];
		e[i] = b[i];
		r[i] = 0;
	}
	r[0] = 1;

	for (int32_t i = 0; i < 448; i++) {
		_fp448_mul(t, r, x);
		_fp448_swap(r, t, e[i / 32] >> (i % 32));
		_fp448_mul(x, x, x);
	}
}

/* @func: _sc448_modw (static)
 * #desc:
 *    scalar high-word modular reduction fold.
 *
 * #1: r [out] result
 * #2: a [in]  number
 * #3: b [in]  high-word
 */
static void _sc448_modw(uint32_t r[14],
		const uint32_t a[14], uint32_t b)
{
	uint32_t rr[14];
	uint32_t carry = 0, carry2 = 0;
	uint64_t tmp = 0;

	/* rr = b * R */
	for (int32_t i = 0; i < 14; i++) {
		tmp = (uint64_t)b * _sc448_R[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	/* r = a + rr */
	for (int32_t i = 0; i < 14; i++) {
		tmp = (uint64_t)a[i] + rr[i] + carry2;
		r[i] = tmp & 0xffffffff;
		carry2 = tmp >> 32;
	}

	/* rr = carry2 * R */
	carry = 0;
	for (int32_t i = 0; i < 14; i++) {
		tmp = (uint64_t)carry2 * _sc448_R[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	/* r = r + rr */
	carry = 0;
	for (int32_t i = 0; i < 14; i++) {
		tmp = (uint64_t)r[i] + rr[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}
}

/* @func: _sc448_mod (static)
 * #desc:
 *    scalar modular reduction (norm).
 *
 * #1: r [in/out] number
 */
static void _sc448_mod(uint32_t r[14])
{
	const uint32_t *p = _sc448_BPO[(r[13] >> 30) + 1]; /* 448 - 446 = 2 */
	uint32_t carry = 0;
	uint64_t tmp = 0;

	/* r = r - p */
	for (int32_t i = 0; i < 14; i++) {
		tmp = (uint64_t)r[i] - p[i] + (int32_t)carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	/* r = r + p */
	p = _sc448_BPO[carry & 1];
	carry = 0;
	for (int32_t i = 0; i < 14; i++) {
		tmp = (uint64_t)r[i] + p[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}
}

/* @func: _sc448_add (static)
 * #desc:
 *    scalar addition.
 *
 * #1: r [out] sum
 * #2: a [in]  addend
 * #3: b [in]  addend
 */
static void _sc448_add(uint32_t r[14],
		const uint32_t a[14], const uint32_t b[14])
{
	uint32_t carry = 0;
	uint64_t tmp = 0;

	/* r = a + b */
	for (int32_t i = 0; i < 14; i++) {
		tmp = (uint64_t)a[i] + b[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	/* r = r % q */
	_sc448_modw(r, r, carry);
}

/* @func: _sc448_mul (static)
 * #desc:
 *    scalar multiplication.
 *
 * #1: r [out] product
 * #2: a [in]  multiplicand
 * #3: b [in]  multiplier
 */
static void _sc448_mul(uint32_t r[14],
		const uint32_t a[14], const uint32_t b[14])
{
	uint32_t rr[28];
	uint32_t carry = 0;
	uint64_t tmp = 0;

	for (int32_t i = 0; i < 28; i++)
		rr[i] = 0;

	/* rr = a * b */
	for (int32_t i = 0; i < 14; i++) {
		tmp = (uint64_t)a[0] * b[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}
	rr[14] = carry;

	for (int32_t i = 1; i < 14; i++) {
		carry = 0;
		for (int32_t j = 0; j < 14; j++) {
			tmp = (uint64_t)a[i] * b[j] + rr[i + j] + carry;
			rr[i + j] = tmp & 0xffffffff;
			carry = tmp >> 32;
		}
		rr[i + 14] = carry;
	}

	/* r = rr % q */
	_sc448_modw(rr + 13, rr + 13, rr[27]);
	_sc448_modw(rr + 12, rr + 12, rr[26]);
	_sc448_modw(rr + 11, rr + 11, rr[25]);
	_sc448_modw(rr + 10, rr + 10, rr[24]);
	_sc448_modw(rr + 9, rr + 9, rr[23]);
	_sc448_modw(rr + 8, rr + 8, rr[22]);
	_sc448_modw(rr + 7, rr + 7, rr[21]);
	_sc448_modw(rr + 6, rr + 6, rr[20]);
	_sc448_modw(rr + 5, rr + 5, rr[19]);
	_sc448_modw(rr + 4, rr + 4, rr[18]);
	_sc448_modw(rr + 3, rr + 3, rr[17]);
	_sc448_modw(rr + 2, rr + 2, rr[16]);
	_sc448_modw(rr + 1, rr + 1, rr[15]);
	_sc448_modw(r, rr, rr[14]);
}

/* @func: _sc448_digest (static)
 * #desc:
 *    shake256 digest modular reduction.
 *
 * #1: dig [in]  shake256 digest
 * #2: r   [out] result
 */
static void _sc448_digest(const uint8_t dig[114], uint32_t r[14])
{
	uint32_t rr[29]; /* (456 * 2) < 928-bits */
	rr[28] = 0; /* mask */

	conch_memcpy(rr, dig, 114),

	/* r = rr % q */
	_sc448_modw(rr + 14, rr + 14, rr[28]);
	_sc448_modw(rr + 13, rr + 13, rr[27]);
	_sc448_modw(rr + 12, rr + 12, rr[26]);
	_sc448_modw(rr + 11, rr + 11, rr[25]);
	_sc448_modw(rr + 10, rr + 10, rr[24]);
	_sc448_modw(rr + 9, rr + 9, rr[23]);
	_sc448_modw(rr + 8, rr + 8, rr[22]);
	_sc448_modw(rr + 7, rr + 7, rr[21]);
	_sc448_modw(rr + 6, rr + 6, rr[20]);
	_sc448_modw(rr + 5, rr + 5, rr[19]);
	_sc448_modw(rr + 4, rr + 4, rr[18]);
	_sc448_modw(rr + 3, rr + 3, rr[17]);
	_sc448_modw(rr + 2, rr + 2, rr[16]);
	_sc448_modw(rr + 1, rr + 1, rr[15]);
	_sc448_modw(r, rr, rr[14]);
}

/* @func: _np448_sub (static)
 * #desc:
 *    nonprime field subtraction.
 *
 * #1: r [out] difference
 * #2: a [in]  minuend
 * #3: b [in]  subtract
 * #r:   [ret] overflow
 */
static uint32_t _np448_sub(uint32_t r[14],
		const uint32_t a[14], const uint32_t b[14])
{
	uint32_t carry = 0;
	uint64_t tmp = 0;

	/* r = a - b */
	for (int32_t i = 0; i < 14; i++) {
		tmp = (uint64_t)a[i] - b[i] + (int32_t)carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	return carry;
}

/* @func: _x448_scalar_mul (static)
 * #desc:
 *    x448 montgomery ladder scalar multiplication.
 *
 * #1: k [in]  input point
 * #2: b [in]  base point
 * #3: r [out] scalar
 */
static void _x448_scalar_mul(const uint32_t k[14], const uint32_t b[14],
		uint32_t r[14])
{
	uint32_t x1[14], z1[14], x2[14], z2[14];
	for (int32_t i = 0; i < 14; i++) {
		x1[i] = 0;
		z1[i] = 0;
		x2[i] = b[i];
		z2[i] = 0;
	}
	x1[0] = 1;
	z2[0] = 1;
	/*
	 * x1, z1 = (1, 0)
	 * x2, z2 = (b, 1)
	 */

	/* montgomery projective coordinates */
	uint32_t A[14], AA[14], B[14], BB[14], E[14], C[14], D[14],
		DA[14], CB[14];
	for (int32_t i = 447; i >= 0; i--) {
		uint32_t k_i = k[i / 32] >> (i % 32);
		_fp448_swap(x1, x2, k_i);
		_fp448_swap(z1, z2, k_i);

		/* A = x1 + z1 */
		_fp448_add(A, x1, z1);
		/* AA = A ^ 2 */
		_fp448_mul(AA, A, A);

		/* B = x1 - z1 */
		_fp448_sub(B, x1, z1);
		/* BB = B ^ 2 */
		_fp448_mul(BB, B, B);

		/* E = AA - BB */
		_fp448_sub(E, AA, BB);
		/* C = x2 + z2 */
		_fp448_add(C, x2, z2);
		/* D = x2 - z2 */
		_fp448_sub(D, x2, z2);

		/* DA = D * A */
		_fp448_mul(DA, D, A);
		/* CB = C * B */
		_fp448_mul(CB, C, B);

		/* x2 = (DA * CB) ^ 2 */
		_fp448_add(x2, DA, CB);
		_fp448_mul(x2, x2, x2);

		/* z2 = b * ((DA - CB) ^ 2) */
		_fp448_sub(z2, DA, CB);
		_fp448_mul(z2, z2, z2);
		_fp448_mul(z2, z2, b);

		/* x1 = AA * BB */
		_fp448_mul(x1, AA, BB);

		/* z1 = E * (AA + 39081 * E) */
		_fp448_mul39081(z1, E, AA);
		_fp448_mul(z1, z1, E);

		_fp448_swap(x1, x2, k_i);
		_fp448_swap(z1, z2, k_i);
	}

	/* r = (inv(z1) * x1) % p */
	_fp448_inv(z1, z1);
	_fp448_mul(r, z1, x1);
	_fp448_mod(r);
}

/* @func: _ed448_point_add (static)
 * #desc:
 *    edwards curve point addition.
 *
 * #1: xyz1 [in]  addend
 * #2: xyz2 [in]  addend
 * #3: xyz3 [out] result
 */
static void _ed448_point_add(const struct ed448_point *xyz1,
		const struct ed448_point *xyz2, struct ed448_point *xyz3)
{
	uint32_t A[14], B[14], A1[14], B1[14], C1[14], D1[14], E1[14],
		F1[14], G1[14], H1[14];
	/*
	 * A1 = z1 * z2
	 * B1 = A1 ^ 2
	 * C1 = x1 * x2
	 * D1 = y1 * y2
	 * E1 = (d * C1) * D1
	 * F1 = B1 - E1
	 * G1 = B1 + E1
	 * H1 = (x1 + y1) * (x2 + y2)
	 * x3 = (A1 * F1) * (H1 - C1 - D1)
	 * y3 = (A1 * G1) * (D1 - C1)
	 * z3 = F1 * G1
	 */

	/* A1 = z1 * z2 */
	_fp448_mul(A1, xyz1->z, xyz2->z);
	/* B1 = A1 ^ 2 */
	_fp448_mul(B1, A1, A1);
	/* c1 = x1 * x2 */
	_fp448_mul(C1, xyz1->x, xyz2->x);
	/* d1 = y1 * y2 */
	_fp448_mul(D1, xyz1->y, xyz2->y);

	/* E1 = (d * C1) * D1 */
	_fp448_mul(E1, C1, _ed448_d);
	_fp448_mul(E1, E1, D1);

	/* F1 = B1 - E1 */
	_fp448_sub(F1, B1, E1);
	/* G1 = B1 + E1 */
	_fp448_add(G1, B1, E1);

	/* H1 = (x1 + y1) * (x2 + y2) */
	_fp448_add(A, xyz1->x, xyz1->y);
	_fp448_add(B, xyz2->x, xyz2->y);
	_fp448_mul(H1, A, B);

	/* x3 = (A1 * F1) * (H1 - C1 - D1) */
	_fp448_mul(A, A1, F1);
	_fp448_sub(B, H1, C1);
	_fp448_sub(B, B, D1);
	_fp448_mul(xyz3->x, A, B);

	/* y3 = (A1 * G1) * (D1 - C1) */
	_fp448_mul(A, A1, G1);
	_fp448_sub(B, D1, C1);
	_fp448_mul(xyz3->y, A, B);

	/* z3 = F1 * G1 */
	_fp448_mul(xyz3->z, F1, G1);
}

/* @func: _ed448_point_double (static)
 * #desc:
 *    edwards curve point doubling.
 *
 * #1: xyz1 [in]  addend
 * #2: xyz3 [out] result
 */
static void _ed448_point_double(const struct ed448_point *xyz1,
		struct ed448_point *xyz3)
{
	uint32_t A[14], B1[14], C1[14], D1[14], E1[14], H1[14], J1[14];
	/*
	 * B1 = (x1 + y1) ^ 2
	 * C1 = x1 ^ 2
	 * D1 = y1 ^ 2
	 * E1 = C1 + D1
	 * H1 = z1 ^ 2
	 * J1 = E1 - (H1 * 2)
	 * x3 = J1 * (B1 - E1)
	 * y3 = E1 * (C1 - D1)
	 * x3 = E1 * J1
	 */

	/* B1 = (x1 + y1) ^ 2 */
	_fp448_add(B1, xyz1->x, xyz1->y);
	_fp448_mul(B1, B1, B1);

	/* C1 = x1 ^ 2 */
	_fp448_mul(C1, xyz1->x, xyz1->x);
	/* D1 = y1 ^ 2 */
	_fp448_mul(D1, xyz1->y, xyz1->y);
	/* E1 = C1 + D1 */
	_fp448_add(E1, C1, D1);
	/* H1 = z1 ^ 2 */
	_fp448_mul(H1, xyz1->z, xyz1->z);

	/* J1 = E1 - (H1 * 2) */
	_fp448_add(J1, H1, H1);
	_fp448_sub(J1, E1, J1);

	/* X3 = J1 * (B1 - E1) */
	_fp448_sub(A, B1, E1);
	_fp448_mul(xyz3->x, J1, A);

	/* y3 = E1 * (C1 - D1) */
	_fp448_sub(A, C1, D1);
	_fp448_mul(xyz3->y, E1, A);

	/* y3 = E1 * J1 */
	_fp448_mul(xyz3->z, E1, J1);
}

/* @func: _ed448_scalar_mul (static)
 * #desc:
 *    ed448 montgomery ladder scalar multiplication.
 *
 * #1: k    [in]  input point
 * #2: xyz1 [in]  base point
 * #3: xyz2 [out] scalar
 */
static void _ed448_scalar_mul(const uint32_t k[14],
		const struct ed448_point *xyz1, struct ed448_point *xyz2)
{
	struct ed448_point _xyz1, _xyz2, _xyz3;
	for (int32_t i = 0; i < 14; i++) {
		_xyz1.x[i] = 0;
		_xyz1.y[i] = 0;
		_xyz1.z[i] = 0;
		_xyz2.x[i] = xyz1->x[i];
		_xyz2.y[i] = xyz1->y[i];
		_xyz2.z[i] = xyz1->z[i];
	}
	_xyz1.y[0] = 1;
	_xyz1.z[0] = 1;
	/*
	 * _xyz1 = (0, 1, 1)
	 * _xyz2 = xyz1
	 */

	/* edwards projective coordinates */
	for (int32_t i = 0; i < 448; i++) {
		_ed448_point_add(&_xyz1, &_xyz2, &_xyz3);

		uint32_t k_i = k[i / 32] >> (i % 32);
		_fp448_swap(_xyz1.x, _xyz3.x, k_i);
		_fp448_swap(_xyz1.y, _xyz3.y, k_i);
		_fp448_swap(_xyz1.z, _xyz3.z, k_i);

		/* _ed448_point_add(&_xyz2, &_xyz2, &_xyz2); */
		_ed448_point_double(&_xyz2, &_xyz2);
	}

	for (int32_t i = 0; i < 14; i++) {
		xyz2->x[i] = _xyz1.x[i];
		xyz2->y[i] = _xyz1.y[i];
		xyz2->z[i] = _xyz1.z[i];
	}
}

/* @func: _ed448_point_equal (static)
 * #desc:
 *    curve point comparison is equal.
 *
 * #1: xyz1 [in]  curve point
 * #2: xyz2 [in]  curve point
 * #r:      [ret] 0: p1 != p2, 1: p1 == p2
 */
static int32_t _ed448_point_equal(const struct ed448_point *xyz1,
		const struct ed448_point *xyz2)
{
	uint32_t a[14], b[14], r = 0;
	/*
	 * if ((x1 * z2) - (x2 * z1))
	 *   return 0
	 * if ((y1 * z2) - (y2 * z1))
	 *   return 0
	 * return 1
	 */

	_fp448_mul(a, xyz1->x, xyz2->z);
	_fp448_mul(b, xyz2->x, xyz1->z);
	_fp448_sub(a, a, b);
	r = _fp448_iszero(a);

	_fp448_mul(a, xyz1->y, xyz2->z);
	_fp448_mul(b, xyz2->y, xyz1->z);
	_fp448_sub(a, a, b);
	r &= _fp448_iszero(a);

	/*
	 * 0 & 1 == 0
	 * 1 & 0 == 0
	 * 1 & 1 == 1
	 */
	return r;
}

/* @func: _ed448_check_point (static)
 * #desc:
 *    curve point legality check.
 *
 * #1: xyz1 [in]  curve point
 * #r:      [ret] 0: no error, -1: error
 */
static int32_t _ed448_check_point(const struct ed448_point *xyz1)
{
	uint32_t a[14], b[14], t1[14], t2[14];
	/*
	 * if ((y * y) + (x * x)) != (1 + (d * (x * x) * (y * y)))
	 *   return -1;
	 * return 0;
	 */

	/* t1 = (y * y) + (x * x) */
	_fp448_mul(a, xyz1->y, xyz1->y);
	_fp448_mul(b, xyz1->x, xyz1->x);
	_fp448_add(t1, a, b);
	_fp448_mod(t1);

	/* t2 = 1 + (d * a * b) */
	_fp448_mul(t2, a, b);
	_fp448_mul(t2, t2, _ed448_d);
	_fp448_add(t2, t2, _ed448_one);
	_fp448_mod(t2);

	_fp448_sub(t1, t1, t2);

	return _fp448_iszero(t1) - 1;
}

/* @func: _ed448_point_recover_x (static)
 * #desc:
 *    calculate the corresponding curve point x.
 *
 * #1: y    [in]  curve point y
 * #2: sign [in]  sign of x
 * #3: r    [out] curve point x
 * #r:      [ret] 0: success, -1: fail
 */
static int32_t _ed448_point_recover_x(const uint32_t y[14], uint32_t sign,
		uint32_t r[14])
{
	uint32_t x1[14], x2[14], y2[14], x[14];
	/*
	 * y2 = y ^ 2
	 * x1 = y2 - 1
	 * x2 = (d * y2) - 1
	 * x1 = (x1 * inv(x2, p)) % p
	 * x = modpow(x1, (p + 1) / 4, p)
	 * if (((x ^ 2) % p) - x1)
	 *   return -1
	 * if (x == 0 && (x & 1) != sign)
	 *   return -1
	 * if ((x & 1) != sign)
	 *   x = p - x
	 * return 0
	 */

	/* y2 = y ^ 2 */
	_fp448_mul(y2, y, y);

	/* x1 = y2 - 1 */
	_fp448_sub(x1, y2, _ed448_one);

	/* x2 = (d * y2) - 1 */
	_fp448_mul(x2, y2, _ed448_d);
	_fp448_sub(x2, x2, _ed448_one);

	/* x1 = (x1 * inv(x2)) % p */
	_fp448_inv(x2, x2);
	_fp448_mul(x1, x1, x2);
	_fp448_mod(x1);

	/* x = modpow(x1, (p + 1) / 4, p) % p */
	_fp448_pow(x, x1, _ed448_p14);
	_fp448_mod(x);

	/* y2 = ((x ^ 2) % p) - x1 */
	_fp448_mul(y2, x, x);
	_fp448_mod(y2);
	_fp448_sub(y2, y2, x1);

	/* if y2 != 0 : fail, no square root */
	if (!_fp448_iszero(y2))
		return -1;

	/* if x == 0 && (x & 1) != sign : fail */
	if (_fp448_iszero(x) & ((x[0] & 1) ^ (sign & 1)))
		return -1;

	/* x1 = p - x */
	_fp448_sub(x1, _fp448_p, x);
	/* x = if x & 1 != sign : swap x1 */
	_fp448_swap(x, x1, (x[0] & 1) ^ (sign & 1));

	/* r = x */
	for (int32_t i = 0; i < 14; i++)
		r[i] = x[i];

	return 0;
}

/* @func: _ed448_point_compress (static)
 * #desc:
 *    curve point compression.
 *
 * #1: xyz1 [in]  curve point
 * #2: r    [out] compress point
 */
static void _ed448_point_compress(const struct ed448_point *xyz1,
		uint32_t r[15])
{
	uint32_t x[14], z[14];
	/*
	 * x = (x1 * inv(z, p)) % p
	 * y = (y1 * inv(z, p)) % p
	 * r = y | ((x & 1) << 455)
	 */

	/* z = inv(z) */
	_fp448_inv(z, xyz1->z);

	/* x = (x1 * z) % p */
	_fp448_mul(x, xyz1->x, z);
	_fp448_mod(x);

	/* r = (y1 * z) % p */
	_fp448_mul(r, xyz1->y, z);
	_fp448_mod(r);

	/* r = y | ((x & 1) << 455) */
	r[14] = (x[0] & 1) << 7; /* x % 2 = 0 (non-negative and even) */
}

/* @func: _ed448_point_decompress (static)
 * #desc:
 *    curve point decompression.
 *
 * #1: k    [in]  compress point
 * #2: xyz1 [out] curve point
 * #r:      [ret] 0: success, -1: fail
 */
static int32_t _ed448_point_decompress(const uint32_t k[15],
		struct ed448_point *xyz1)
{
	uint32_t t[14];
	/*
	 * y1 = k & ((1 << 448) - 1)
	 * x1 = rec_x(y1, (k >> 455) & 1)
	 * z1 = 1
	 */

	/* y1 = k & ((1 << 448) - 1) */
	for (int32_t i = 0; i < 14; i++)
		xyz1->y[i] = k[i];

	/* if y1 < p */
	if (!_np448_sub(t, xyz1->y, _fp448_p))
		return -1;

	/* x1 = rec_x(y1, (k >> 455) & 1) */
	if (_ed448_point_recover_x(xyz1->y, (k[14] >> 7) & 1, xyz1->x))
		return -1;

	/* z1 = 1 */
	for (int32_t i = 0; i < 14; i++)
		xyz1->z[i] = 0;
	xyz1->z[0] = 1;

	return _ed448_check_point(xyz1);
}

/* @func: conch_ecdh_x448_public_key
 * #desc:
 *    x448 public key create function.
 *
 * #1: pri [in]  private key
 * #2: pub [out] public key
 */
void conch_ecdh_x448_public_key(const uint8_t *pri, uint8_t *pub)
{
	uint32_t _pri[14], _pub[14];

	conch_memcpy(_pri, pri, ECDH_X448_PRI_LEN);

	/* key clamping */
	_pri[0] &= ~0x03U;
	_pri[13] |= 0x80000000;

	/* _pub = scalar(_pri, base) */
	_x448_scalar_mul(_pri, _x448_b, _pub);

	conch_memcpy(pub, _pub, ECDH_X448_PUB_LEN);
}

/* @func: conch_ecdh_x448_shared_key
 * #desc:
 *    x448 shared key create function.
 *
 * #1: pri [in]  private key
 * #2: pub [in]  public key
 * #3: key [out] shared key
 * #r:     [ret] 0: success, -1: fail
 */
int32_t conch_ecdh_x448_shared_key(const uint8_t *pri,
		const uint8_t *pub, uint8_t *key)
{
	uint32_t _pri[14], _pub[14], _key[14];

	conch_memcpy(_pri, pri, ECDH_X448_PRI_LEN);
	conch_memcpy(_pub, pub, ECDH_X448_PUB_LEN);

	/* key clamping */
	_pri[0] &= ~0x03U;
	_pri[13] |= 0x80000000;

	/* _key = scalar(_pri, _pub) */
	_x448_scalar_mul(_pri, _pub, _key);

	conch_memcpy(key, _key, ECDH_X448_KEY_LEN);

	return 0 - _fp448_iszero(_key);
}

/* @func: conch_eddsa_ed448_nonce_key
 * #desc:
 *    ed448 private and nonce operation function.
 *
 * #1: inpri [in]  input private key
 * #2: pri   [out] private key
 * #3: ran   [out] nonce
 */
void conch_eddsa_ed448_nonce_key(const uint8_t *inpri, uint8_t *pri,
		uint8_t *ran)
{
	uint32_t _pri[15];
	SHA3_NEW(sha_ctx);

	conch_sha3_init(&sha_ctx, SHA3_SHAKE256_TYPE, 114);
	conch_sha3(&sha_ctx, inpri, EDDSA_ED448_PRI_LEN);

	conch_memcpy(_pri,
		&(SHA3_STATE(&sha_ctx, 0)),
		EDDSA_ED448_PRI_LEN);
	conch_memcpy(ran,
		&(SHA3_STATE(&sha_ctx, EDDSA_ED448_RAN_LEN)),
		EDDSA_ED448_RAN_LEN);

	/* key clamping */
	_pri[0] &= ~0x03U;
	_pri[13] |= 0x80000000;

	_pri[14] = 0; /* mask */

	conch_memcpy(pri, _pri, EDDSA_ED448_PRI_LEN);
}

/* @func: conch_eddsa_ed448_public_key
 * #desc:
 *    ed448 public key create function.
 *
 * #1: pri [in]  private key
 * #2: pub [out] public key
 */
void conch_eddsa_ed448_public_key(const uint8_t *pri, uint8_t *pub)
{
	uint32_t _pri[15], _pub[15], _ran[15];
	struct ed448_point xyz1;

	conch_eddsa_ed448_nonce_key(pri, (uint8_t *)_pri, (uint8_t *)_ran);

	/* _pub = compress(scalar(_pri, base)) */
	_ed448_scalar_mul(_pri, &_ed448_base, &xyz1);
	_ed448_point_compress(&xyz1, _pub);

	conch_memcpy(pub, _pub, EDDSA_ED448_PUB_LEN);
}

/* @func: conch_eddsa_ed448_sign
 * #desc:
 *    ed448 signature function.
 *
 * #1: pri  [in]  private key
 * #2: msg  [in]  input message
 * #3: len  [in]  message length
 * #4: sign [out] signature
 */
void conch_eddsa_ed448_sign(const uint8_t *pri,
		const uint8_t *msg, uint32_t len, uint8_t *sign)
{
	uint32_t _pri[15], _pub[15], _ran[15], r[14], R[15], h[14], s[15];
	struct ed448_point xyz1;
	SHA3_NEW(sha_ctx);

	conch_eddsa_ed448_nonce_key(pri, (uint8_t *)_pri, (uint8_t *)_ran);
	conch_eddsa_ed448_public_key(pri, (uint8_t *)_pub);

	/* r = sha(ctx + _ran + msg) % q */
	conch_sha3_init(&sha_ctx, SHA3_SHAKE256_TYPE, 114);
	conch_sha3_process(&sha_ctx, _ed448_ctx, ED448_CTX_LEN);
	conch_sha3_process(&sha_ctx, (uint8_t *)_ran, EDDSA_ED448_RAN_LEN);
	conch_sha3_process(&sha_ctx, msg, len);
	conch_sha3_finish(&sha_ctx);
	_sc448_digest(&(SHA3_STATE(&sha_ctx, 0)), r);
	_sc448_mod(r);

	/* R = compress(scalar(r, base)) */
	_ed448_scalar_mul(r, &_ed448_base, &xyz1);
	_ed448_point_compress(&xyz1, R);

	/* h = sha(ctx + R + _pub + msg) % q */
	conch_sha3_init(&sha_ctx, SHA3_SHAKE256_TYPE, 114);
	conch_sha3_process(&sha_ctx, _ed448_ctx, ED448_CTX_LEN);
	conch_sha3_process(&sha_ctx, (uint8_t *)R, EDDSA_ED448_LEN);
	conch_sha3_process(&sha_ctx, (uint8_t *)_pub, EDDSA_ED448_PUB_LEN);
	conch_sha3_process(&sha_ctx, msg, len);
	conch_sha3_finish(&sha_ctx);
	_sc448_digest(&(SHA3_STATE(&sha_ctx, 0)), h);

	/* s = ((h * _pri) + r) % q */
	_sc448_mul(s, h, _pri);
	_sc448_add(s, s, r);
	_sc448_mod(s);

	s[14] = 0; /* mask */

	conch_memcpy(sign, R, EDDSA_ED448_LEN);
	conch_memcpy(sign + EDDSA_ED448_LEN, s, EDDSA_ED448_LEN);
}

/* @func: conch_eddsa_ed448_verify
 * #desc:
 *    ed448 signature verification function.
 *
 * #1: pub  [in]  public key
 * #2: sign [in]  signature
 * #3: msg  [in]  input message
 * #4: len  [in]  message length
 * #r:      [ret] 0: success, -1: fail
 */
int32_t conch_eddsa_ed448_verify(const uint8_t *pub,
		const uint8_t *sign, const uint8_t *msg, uint32_t len)
{
	uint32_t _pub[15], r[15], s[15], h[14];
	struct ed448_point xyz1, xyz2, R, A;
	SHA3_NEW(sha_ctx);

	conch_memcpy(_pub, pub, EDDSA_ED448_PUB_LEN);
	conch_memcpy(r, sign, EDDSA_ED448_LEN);
	conch_memcpy(s, sign + EDDSA_ED448_LEN, EDDSA_ED448_LEN);

	/* if s < q */
	if (!_np448_sub(h, s, _sc448_q))
		return -1;

	/* A = decompress(_pub) */
	if (_ed448_point_decompress(_pub, &A))
		return -1;
	/* R = decompress(rs) */
	if (_ed448_point_decompress(r, &R))
		return -1;

	/* h = sha(ctx + rs + _pub + msg) % q */
	conch_sha3_init(&sha_ctx, SHA3_SHAKE256_TYPE, 114);
	conch_sha3_process(&sha_ctx, _ed448_ctx, ED448_CTX_LEN);
	conch_sha3_process(&sha_ctx, (uint8_t *)r, EDDSA_ED448_LEN);
	conch_sha3_process(&sha_ctx, (uint8_t *)_pub, EDDSA_ED448_PUB_LEN);
	conch_sha3_process(&sha_ctx, msg, len);
	conch_sha3_finish(&sha_ctx);
	_sc448_digest(&(SHA3_STATE(&sha_ctx, 0)), h);
	_sc448_mod(h);

	/* xyz1 = scalar(h, A) */
	_ed448_scalar_mul(h, &A, &xyz1);
	/* xyz1 = add(R, xyz1) */
	_ed448_point_add(&R, &xyz1, &xyz1);

	/* xyz2 = scalar(s, base) */
	_ed448_scalar_mul(s, &_ed448_base, &xyz2);

	/* NOTE: cofactor clearing [4]R + [h*4]A == [s*4]B */

	for (int32_t i = 0; i < 2; i++) { /* 4-torsion subgroup, 2^log2(4) */
		_ed448_point_double(&xyz1, &xyz1);
		_ed448_point_double(&xyz2, &xyz2);
	}

	return _ed448_point_equal(&xyz1, &xyz2) - 1;
}
