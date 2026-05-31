/* @file: nist-p521.c
 * #desc:
 *    The implementations of nist p521 ecdh and ecdsa.
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
#include <conch/ecc.h>


/* P = 2^521 - 1 */
static const uint32_t _fp521_p[17] = {
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0x000001ff
	};

/* A = -3 % P */
static const uint32_t _p521_a[17] = {
	0xfffffffc, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0x000001ff
	};

/*
 * B = 1093849038073734274511112390766805569936207598951683748994586394495
 *     9531161507350160137087375737596232485921322967063133094384525315910
 *     12912142327488478985984
 */
static const uint32_t _p521_b[17] = {
	0x6b503f00, 0xef451fd4, 0x3d2c34f1, 0x3573df88,
	0x3bb1bf07, 0x1652c0bd, 0xec7e937b, 0x56193951,
	0x8ef109e1, 0xb8b48991, 0x99b315f3, 0xa2da725b,
	0xb68540ee, 0x929a21a0, 0x8e1c9a1f, 0x953eb961,
	0x00000051
	};

/*
 * Q = 6864797660130609714981900799081393217269435300143305409394463459185
 *     5431833976553942450577463332171975329639963713633211138647686124403
 *     80340372808892707005449
 */
static const uint32_t _sc521_q[17] = {
	0x91386409, 0xbb6fb71e, 0x899c47ae, 0x3bb5c9b8,
	0xf709a5d0, 0x7fcc0148, 0xbf2f966b, 0x51868783,
	0xfffffffa, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0x000001ff
	};

/*
 * Bx = 266174080205021706322876871672336096072985916875697314770667136841
 *      880294499642780849154508062777190235209424122506555866215711354557
 *      0916814161637315895999846
 * By = 375718002577002046354550722449118360359445513476976248669456777961
 *      554447744055631669123440501294553956214444453728942852258566672919
 *      6580810124344277578376784
 */
struct p521_point {
	uint32_t x[17];
	uint32_t y[17];
	uint32_t z[17]; /* jacobian coordinates */
};

static const struct p521_point _p521_base = {
	{
		0xc2e5bd66, 0xf97e7e31, 0x856a429b, 0x3348b3c1,
		0xa2ffa8de, 0xfe1dc127, 0xefe75928, 0xa14b5e77,
		0x6b4d3dba, 0xf828af60, 0x053fb521, 0x9c648139,
		0x2395b442, 0x9e3ecb66, 0x0404e9cd, 0x858e06b7,
		0x000000c6
	},
	{
		0x9fd16650, 0x88be9476, 0xa272c240, 0x353c7086,
		0x3fad0761, 0xc550b901, 0x5ef42640, 0x97ee7299,
		0x273e662c, 0x17afbd17, 0x579b4468, 0x98f54449,
		0x2c7d1bd9, 0x5c8a5fb4, 0x9a3bc004, 0x39296a78,
		0x00000118
	},
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	}
	};

/*
 * R = 2^544 % Q
 *   551867647541077700652255638561243627813117473089660986076197301492665
 *   5240969780199424
 */
static const uint32_t _sc521_R[17] = {
	0xfb800000, 0x70b763cd, 0x28a24824, 0x23bb31dc,
	0x17e2251b, 0x5b847b2d, 0xca4019ff, 0x3e206834,
	0x02d73cbc, 0, 0, 0, 0, 0, 0, 0, 0
	};


/* @func: _fp521_swap (static)
 * #desc:
 *    constant-time the numerical swap.
 *
 * #1: a   [in/out] number
 * #2: b   [in/out] number
 * #3: bit [in]     low bit (0: keep, 1: swap)
 */
static void _fp521_swap(uint32_t a[17], uint32_t b[17], uint32_t bit)
{
	bit = (~bit & 1) - 1;
	for (int32_t i = 0; i < 17; i++) {
		uint32_t t = bit & (a[i] ^ b[i]);
		a[i] ^= t;
		b[i] ^= t;
	}
}

/* @func: _fp521_move (static)
 * #desc:
 *    constant-time the move numerical.
 *
 * #1: a   [in/out] number
 * #2: b   [in]     number
 * #3: bit [in]     low bit (0: keep, 1: swap)
 */
static void _fp521_move(uint32_t a[17], const uint32_t b[17], uint32_t bit)
{
	bit = (~bit & 1) - 1;
	for (int32_t i = 0; i < 17; i++) {
		uint32_t t = bit & (a[i] ^ b[i]);
		a[i] ^= t;
	}
}

/* @func: _fp521_iszero (static)
 * #desc:
 *    check if a number is zero in constant-time.
 *
 * #1: a [in]  number
 * #r:   [ret] 0: non-zero, 1: is zero
 */
static uint32_t _fp521_iszero(const uint32_t a[17])
{
	uint64_t r = 0;
	for (int32_t i = 0; i < 17; i++)
		r |= a[i];

	return ((r - 1) >> 32) & 1;
}

/* @func: _fp521_add (static)
 * #desc:
 *    prime field addition.
 *
 * #1: r [out] sum
 * #2: a [in]  addend
 * #3: b [in]  addend
 */
static void _fp521_add(uint32_t r[17],
		const uint32_t a[17], const uint32_t b[17])
{
#if 0

	uint32_t carry = 0;
	uint64_t tmp = 0;

	/* r = a + b */
	for (int32_t i = 0; i < 17; i++) {
		tmp = (uint64_t)a[i] + b[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	/* NOTE: 2^544 == 8388608 (mod 2^521-1) */

	/* r = r % p modular reduction */
	carry *= 8388608;
	for (int32_t i = 0; i < 17; i++) {
		tmp = (uint64_t)r[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	carry *= 8388608;
	for (int32_t i = 0; i < 17; i++) {
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
	uint64_t t14 = (uint64_t)a[14] + b[14];
	uint64_t t15 = (uint64_t)a[15] + b[15];
	uint64_t t16 = (uint64_t)a[16] + b[16];
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
	t14 += t13 >> 32;
	t15 += t14 >> 32;
	t16 += t15 >> 32;
	uint32_t carry = t16 >> 32;
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
	t14 &= 0xffffffff;
	t15 &= 0xffffffff;
	t16 &= 0xffffffff;

	t0 += carry * 8388608;
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
	t14 += t13 >> 32;
	t15 += t14 >> 32;
	t16 += t15 >> 32;
	carry = t16 >> 32;
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
	t14 &= 0xffffffff;
	t15 &= 0xffffffff;
	t16 &= 0xffffffff;

	t0 += carry * 8388608;
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
	t14 += t13 >> 32;
	t15 += t14 >> 32;
	t16 += t15 >> 32;
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
	r[14] = t14 & 0xffffffff;
	r[15] = t15 & 0xffffffff;
	r[16] = t16 & 0xffffffff;

#endif
}

/* @func: _fp521_sub (static)
 * #desc:
 *    prime field subtraction.
 *
 * #1: r [out] difference
 * #2: a [in]  minuend
 * #3: b [in]  subtract
 */
static void _fp521_sub(uint32_t r[17],
		const uint32_t a[17], const uint32_t b[17])
{
#if 0

	uint32_t carry = 0;
	uint64_t tmp = 0;

	/* r = a - b */
	for (int32_t i = 0; i < 17; i++) {
		tmp = (uint64_t)a[i] - b[i] + (int32_t)carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	/* NOTE: 2^544 == 8388608 (mod 2^521-1) */

	/* r = r % p modular reduction */
	tmp = (uint64_t)r[0] - (carry & 8388608);
	r[0] = tmp & 0xffffffff;
	carry = tmp >> 32;
	for (int32_t i = 1; i < 17; i++) {
		tmp = (uint64_t)r[i] + (int32_t)carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	tmp = (uint64_t)r[0] - (carry & 8388608);
	r[0] = tmp & 0xffffffff;
	carry = tmp >> 32;
	for (int32_t i = 1; i < 17; i++) {
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
	uint64_t t14 = (uint64_t)a[14] - b[14];
	uint64_t t15 = (uint64_t)a[15] - b[15];
	uint64_t t16 = (uint64_t)a[16] - b[16];
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
	t14 += (int32_t)(t13 >> 32);
	t15 += (int32_t)(t14 >> 32);
	t16 += (int32_t)(t15 >> 32);
	uint32_t carry = t16 >> 32;
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
	t14 &= 0xffffffff;
	t15 &= 0xffffffff;
	t16 &= 0xffffffff;

	t0 -= carry & 8388608;
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
	t14 += (int32_t)(t13 >> 32);
	t15 += (int32_t)(t14 >> 32);
	t16 += (int32_t)(t15 >> 32);
	carry = t16 >> 32;
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
	t14 &= 0xffffffff;
	t15 &= 0xffffffff;
	t16 &= 0xffffffff;

	t0 -= carry & 8388608;
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
	t14 += (int32_t)(t13 >> 32);
	t15 += (int32_t)(t14 >> 32);
	t16 += (int32_t)(t15 >> 32);
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
	r[14] = t14 & 0xffffffff;
	r[15] = t15 & 0xffffffff;
	r[16] = t16 & 0xffffffff;

#endif
}

/* @func: _fp521_mul (static)
 * #desc:
 *    prime field multiplication.
 *
 * #1: r [out] product
 * #2: a [in]  multiplicand
 * #3: b [in]  multiplier
 */
static void _fp521_mul(uint32_t r[17],
		const uint32_t a[17], const uint32_t b[17])
{
#if 0

	uint32_t rr[34];
	uint32_t carry = 0;
	uint64_t tmp = 0;

	for (int32_t i = 0; i < 34; i++)
		rr[i] = 0;

	/* rr = a * b */
	for (int32_t i = 0; i < 17; i++) {
		tmp = (uint64_t)a[0] * b[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}
	rr[17] = carry;

	for (int32_t i = 1; i < 17; i++) {
		carry = 0;
		for (int32_t j = 0; j < 17; j++) {
			tmp = (uint64_t)a[i] * b[j] + rr[i + j] + carry;
			rr[i + j] = tmp & 0xffffffff;
			carry = tmp >> 32;
		}
		rr[i + 17] = carry;
	}

	/* NOTE: 2^544 == 8388608 (mod 2^521-1) */

	/* r = rr % p modular reduction */
	carry = 0;
	for (int32_t i = 0; i < 17; i++) { /* fold */
		tmp = (uint64_t)rr[i + 17] * 8388608 + rr[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	tmp = (uint64_t)carry * 8388608 + r[0];
	r[0] = tmp & 0xffffffff;
	carry = tmp >> 32;
	for (int32_t i = 1; i < 17; i++) {
		tmp = (uint64_t)r[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	tmp = (uint64_t)carry * 8388608 + r[0];
	r[0] = tmp & 0xffffffff;
	carry = tmp >> 32;
	for (int32_t i = 1; i < 17; i++) {
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
	uint64_t a14 = a[14];
	uint64_t a15 = a[15];
	uint64_t a16 = a[16];
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
	uint64_t b14 = b[14];
	uint64_t b15 = b[15];
	uint64_t b16 = b[16];

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
	uint64_t t14 = a0 * b14;
	uint64_t t15 = a0 * b15;
	uint64_t t16 = a0 * b16;
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
	t14 += t13 >> 32;
	t15 += t14 >> 32;
	t16 += t15 >> 32;
	uint64_t t17 = t16 >> 32;
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
	t14 &= 0xffffffff;
	t15 &= 0xffffffff;
	t16 &= 0xffffffff;

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
	t15 += a1 * b14;
	t16 += a1 * b15;
	t17 += a1 * b16;
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
	t15 += t14 >> 32;
	t16 += t15 >> 32;
	t17 += t16 >> 32;
	uint64_t t18 = t17 >> 32;
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
	t15 &= 0xffffffff;
	t16 &= 0xffffffff;
	t17 &= 0xffffffff;
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
	t16 += a2 * b14;
	t17 += a2 * b15;
	t18 += a2 * b16;
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
	t16 += t15 >> 32;
	t17 += t16 >> 32;
	t18 += t17 >> 32;
	uint64_t t19 = t18 >> 32;
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
	t16 &= 0xffffffff;
	t17 &= 0xffffffff;
	t18 &= 0xffffffff;
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
	t17 += a3 * b14;
	t18 += a3 * b15;
	t19 += a3 * b16;
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
	t17 += t16 >> 32;
	t18 += t17 >> 32;
	t19 += t18 >> 32;
	uint64_t t20 = t19 >> 32;
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
	t17 &= 0xffffffff;
	t18 &= 0xffffffff;
	t19 &= 0xffffffff;
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
	t18 += a4 * b14;
	t19 += a4 * b15;
	t20 += a4 * b16;
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
	t18 += t17 >> 32;
	t19 += t18 >> 32;
	t20 += t19 >> 32;
	uint64_t t21 = t20 >> 32;
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
	t18 &= 0xffffffff;
	t19 &= 0xffffffff;
	t20 &= 0xffffffff;
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
	t19 += a5 * b14;
	t20 += a5 * b15;
	t21 += a5 * b16;
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
	t19 += t18 >> 32;
	t20 += t19 >> 32;
	t21 += t20 >> 32;
	uint64_t t22 = t21 >> 32;
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
	t19 &= 0xffffffff;
	t20 &= 0xffffffff;
	t21 &= 0xffffffff;
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
	t20 += a6 * b14;
	t21 += a6 * b15;
	t22 += a6 * b16;
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
	t20 += t19 >> 32;
	t21 += t20 >> 32;
	t22 += t21 >> 32;
	uint64_t t23 = t22 >> 32;
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
	t20 &= 0xffffffff;
	t21 &= 0xffffffff;
	t22 &= 0xffffffff;
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
	t21 += a7 * b14;
	t22 += a7 * b15;
	t23 += a7 * b16;
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
	t21 += t20 >> 32;
	t22 += t21 >> 32;
	t23 += t22 >> 32;
	uint64_t t24 = t23 >> 32;
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
	t21 &= 0xffffffff;
	t22 &= 0xffffffff;
	t23 &= 0xffffffff;
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
	t22 += a8 * b14;
	t23 += a8 * b15;
	t24 += a8 * b16;
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
	t22 += t21 >> 32;
	t23 += t22 >> 32;
	t24 += t23 >> 32;
	uint64_t t25 = t24 >> 32;
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
	t22 &= 0xffffffff;
	t23 &= 0xffffffff;
	t24 &= 0xffffffff;
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
	t23 += a9 * b14;
	t24 += a9 * b15;
	t25 += a9 * b16;
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
	t23 += t22 >> 32;
	t24 += t23 >> 32;
	t25 += t24 >> 32;
	uint64_t t26 = t25 >> 32;
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
	t23 &= 0xffffffff;
	t24 &= 0xffffffff;
	t25 &= 0xffffffff;
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
	t24 += a10 * b14;
	t25 += a10 * b15;
	t26 += a10 * b16;
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
	t24 += t23 >> 32;
	t25 += t24 >> 32;
	t26 += t25 >> 32;
	uint64_t t27 = t26 >> 32;
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
	t24 &= 0xffffffff;
	t25 &= 0xffffffff;
	t26 &= 0xffffffff;
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
	t25 += a11 * b14;
	t26 += a11 * b15;
	t27 += a11 * b16;
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
	t25 += t24 >> 32;
	t26 += t25 >> 32;
	t27 += t26 >> 32;
	uint64_t t28 = t27 >> 32;
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
	t25 &= 0xffffffff;
	t26 &= 0xffffffff;
	t27 &= 0xffffffff;
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
	t26 += a12 * b14;
	t27 += a12 * b15;
	t28 += a12 * b16;
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
	t26 += t25 >> 32;
	t27 += t26 >> 32;
	t28 += t27 >> 32;
	uint64_t t29 = t28 >> 32;
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
	t26 &= 0xffffffff;
	t27 &= 0xffffffff;
	t28 &= 0xffffffff;
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
	t27 += a13 * b14;
	t28 += a13 * b15;
	t29 += a13 * b16;
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
	t27 += t26 >> 32;
	t28 += t27 >> 32;
	t29 += t28 >> 32;
	uint64_t t30 = t29 >> 32;
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
	t27 &= 0xffffffff;
	t28 &= 0xffffffff;
	t29 &= 0xffffffff;
	t14 += a14 * b0;
	t15 += a14 * b1;
	t16 += a14 * b2;
	t17 += a14 * b3;
	t18 += a14 * b4;
	t19 += a14 * b5;
	t20 += a14 * b6;
	t21 += a14 * b7;
	t22 += a14 * b8;
	t23 += a14 * b9;
	t24 += a14 * b10;
	t25 += a14 * b11;
	t26 += a14 * b12;
	t27 += a14 * b13;
	t28 += a14 * b14;
	t29 += a14 * b15;
	t30 += a14 * b16;
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
	t27 += t26 >> 32;
	t28 += t27 >> 32;
	t29 += t28 >> 32;
	t30 += t29 >> 32;
	uint64_t t31 = t30 >> 32;
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
	t27 &= 0xffffffff;
	t28 &= 0xffffffff;
	t29 &= 0xffffffff;
	t30 &= 0xffffffff;
	t15 += a15 * b0;
	t16 += a15 * b1;
	t17 += a15 * b2;
	t18 += a15 * b3;
	t19 += a15 * b4;
	t20 += a15 * b5;
	t21 += a15 * b6;
	t22 += a15 * b7;
	t23 += a15 * b8;
	t24 += a15 * b9;
	t25 += a15 * b10;
	t26 += a15 * b11;
	t27 += a15 * b12;
	t28 += a15 * b13;
	t29 += a15 * b14;
	t30 += a15 * b15;
	t31 += a15 * b16;
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
	t27 += t26 >> 32;
	t28 += t27 >> 32;
	t29 += t28 >> 32;
	t30 += t29 >> 32;
	t31 += t30 >> 32;
	uint64_t t32 = t31 >> 32;
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
	t27 &= 0xffffffff;
	t28 &= 0xffffffff;
	t29 &= 0xffffffff;
	t30 &= 0xffffffff;
	t31 &= 0xffffffff;
	t16 += a16 * b0;
	t17 += a16 * b1;
	t18 += a16 * b2;
	t19 += a16 * b3;
	t20 += a16 * b4;
	t21 += a16 * b5;
	t22 += a16 * b6;
	t23 += a16 * b7;
	t24 += a16 * b8;
	t25 += a16 * b9;
	t26 += a16 * b10;
	t27 += a16 * b11;
	t28 += a16 * b12;
	t29 += a16 * b13;
	t30 += a16 * b14;
	t31 += a16 * b15;
	t32 += a16 * b16;
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
	t27 += t26 >> 32;
	t28 += t27 >> 32;
	t29 += t28 >> 32;
	t30 += t29 >> 32;
	t31 += t30 >> 32;
	t32 += t31 >> 32;
	uint64_t t33 = t32 >> 32;
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
	t27 &= 0xffffffff;
	t28 &= 0xffffffff;
	t29 &= 0xffffffff;
	t30 &= 0xffffffff;
	t31 &= 0xffffffff;
	t32 &= 0xffffffff;

	t0 += t17 * 8388608;
	t1 += t18 * 8388608;
	t2 += t19 * 8388608;
	t3 += t20 * 8388608;
	t4 += t21 * 8388608;
	t5 += t22 * 8388608;
	t6 += t23 * 8388608;
	t7 += t24 * 8388608;
	t8 += t25 * 8388608;
	t9 += t26 * 8388608;
	t10 += t27 * 8388608;
	t11 += t28 * 8388608;
	t12 += t29 * 8388608;
	t13 += t30 * 8388608;
	t14 += t31 * 8388608;
	t15 += t32 * 8388608;
	t16 += t33 * 8388608;
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
	t14 += t13 >> 32;
	t15 += t14 >> 32;
	t16 += t15 >> 32;
	uint64_t carry = t16 >> 32;
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
	t14 &= 0xffffffff;
	t15 &= 0xffffffff;
	t16 &= 0xffffffff;

	t0 += (uint64_t)carry * 8388608;
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
	t14 += t13 >> 32;
	t15 += t14 >> 32;
	t16 += t15 >> 32;
	carry = t16 >> 32;
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
	t14 &= 0xffffffff;
	t15 &= 0xffffffff;
	t16 &= 0xffffffff;

	t0 += (uint64_t)carry * 8388608;
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
	t14 += t13 >> 32;
	t15 += t14 >> 32;
	t16 += t15 >> 32;
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
	r[14] = t14 & 0xffffffff;
	r[15] = t15 & 0xffffffff;
	r[16] = t16 & 0xffffffff;

#endif
}

/* @func: _fp521_mod (static)
 * #desc:
 *    prime field modular reduction (norm).
 *
 * #1: r [in/out] number
 */
static void _fp521_mod(uint32_t r[17])
{
	uint32_t carry = 0, carry2 = 0;
	uint64_t tmp = 0;

	/* NOTE: 2^544 == 8388608 (mod 2^521-1) */

	carry = r[16] >> 9; /* fold */
	r[16] &= 0x1ff;
	for (int32_t i = 0; i < 17; i++) {
		tmp = (uint64_t)r[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	/* r = r - p */
	for (int32_t i = 0; i < 17; i++) {
		tmp = (uint64_t)r[i] - _fp521_p[i] + (int32_t)carry2;
		r[i] = tmp & 0xffffffff;
		carry2 = tmp >> 32;
	}

	/* r = if r < p : adjust r */
	carry2 = ~(((uint64_t)carry2 - 1) >> 32);
	carry = 0;
	for (int32_t i = 0; i < 17; i++) {
		tmp = (uint64_t)r[i] + (_fp521_p[i] & carry2) + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}
}

/* @func: _fp521_inv (static)
 * #desc:
 *    modular inversion based on fermat's little theorem.
 *
 * #1: r [out] inverse modulus
 * #2: z [in]  number
 */
static void _fp521_inv(uint32_t r[17], const uint32_t z[17])
{
	uint32_t x[17], e[17];
	for (int32_t i = 0; i < 17; i++) {
		x[i] = z[i];
		e[i] = _fp521_p[i];
		r[i] = 0;
	}
	e[0] -= 2;
	r[0] = 1;

	for (int32_t i = 0; i < 521; i++) {
		uint32_t k_i = e[i / 32] >> (i % 32);
		if (k_i & 1)
			_fp521_mul(r, r, x);
		_fp521_mul(x, x, x);
	}
}

/* @func: _sc521_modw (static)
 * #desc:
 *    scalar high-word modular reduction fold.
 *
 * #1: r [out] result
 * #2: a [in]  number
 * #3: b [in]  high-word
 */
static void _sc521_modw(uint32_t r[17],
		const uint32_t a[17], uint32_t b)
{
#if 0

	uint32_t rr[17];
	uint32_t carry = 0, carry2 = 0;
	uint64_t tmp = 0;

	/* rr = b * R */
	for (int32_t i = 0; i < 17; i++) {
		tmp = (uint64_t)b * _sc521_R[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	/* r = a + rr */
	for (int32_t i = 0; i < 17; i++) {
		tmp = (uint64_t)a[i] + rr[i] + carry2;
		r[i] = tmp & 0xffffffff;
		carry2 = tmp >> 32;
	}

	/* rr = carry2 * R */
	carry = 0;
	for (int32_t i = 0; i < 17; i++) {
		tmp = (uint64_t)carry2 * _sc521_R[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	/* r = r + rr */
	carry = 0;
	for (int32_t i = 0; i < 17; i++) {
		tmp = (uint64_t)r[i] + rr[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

#else

	uint64_t tt0 = (uint64_t)b * _sc521_R[0];
	uint64_t tt1 = (uint64_t)b * _sc521_R[1];
	uint64_t tt2 = (uint64_t)b * _sc521_R[2];
	uint64_t tt3 = (uint64_t)b * _sc521_R[3];
	uint64_t tt4 = (uint64_t)b * _sc521_R[4];
	uint64_t tt5 = (uint64_t)b * _sc521_R[5];
	uint64_t tt6 = (uint64_t)b * _sc521_R[6];
	uint64_t tt7 = (uint64_t)b * _sc521_R[7];
	uint64_t tt8 = (uint64_t)b * _sc521_R[8];
	uint64_t tt9 = (uint64_t)b * _sc521_R[9];
	uint64_t tt10 = (uint64_t)b * _sc521_R[10];
	uint64_t tt11 = (uint64_t)b * _sc521_R[11];
	uint64_t tt12 = (uint64_t)b * _sc521_R[12];
	uint64_t tt13 = (uint64_t)b * _sc521_R[13];
	uint64_t tt14 = (uint64_t)b * _sc521_R[14];
	uint64_t tt15 = (uint64_t)b * _sc521_R[15];
	uint64_t tt16 = (uint64_t)b * _sc521_R[16];
	tt1 += tt0 >> 32;
	tt2 += tt1 >> 32;
	tt3 += tt2 >> 32;
	tt4 += tt3 >> 32;
	tt5 += tt4 >> 32;
	tt6 += tt5 >> 32;
	tt7 += tt6 >> 32;
	tt8 += tt7 >> 32;
	tt9 += tt8 >> 32;
	tt10 += tt9 >> 32;
	tt11 += tt10 >> 32;
	tt12 += tt11 >> 32;
	tt13 += tt12 >> 32;
	tt14 += tt13 >> 32;
	tt15 += tt14 >> 32;
	tt16 += tt15 >> 32;
	tt0 &= 0xffffffff;
	tt1 &= 0xffffffff;
	tt2 &= 0xffffffff;
	tt3 &= 0xffffffff;
	tt4 &= 0xffffffff;
	tt5 &= 0xffffffff;
	tt6 &= 0xffffffff;
	tt7 &= 0xffffffff;
	tt8 &= 0xffffffff;
	tt9 &= 0xffffffff;
	tt10 &= 0xffffffff;
	tt11 &= 0xffffffff;
	tt12 &= 0xffffffff;
	tt13 &= 0xffffffff;
	tt14 &= 0xffffffff;
	tt15 &= 0xffffffff;
	tt16 &= 0xffffffff;

	uint64_t t0 = (uint64_t)a[0] + tt0;
	uint64_t t1 = (uint64_t)a[1] + tt1;
	uint64_t t2 = (uint64_t)a[2] + tt2;
	uint64_t t3 = (uint64_t)a[3] + tt3;
	uint64_t t4 = (uint64_t)a[4] + tt4;
	uint64_t t5 = (uint64_t)a[5] + tt5;
	uint64_t t6 = (uint64_t)a[6] + tt6;
	uint64_t t7 = (uint64_t)a[7] + tt7;
	uint64_t t8 = (uint64_t)a[8] + tt8;
	uint64_t t9 = (uint64_t)a[9] + tt9;
	uint64_t t10 = (uint64_t)a[10] + tt10;
	uint64_t t11 = (uint64_t)a[11] + tt11;
	uint64_t t12 = (uint64_t)a[12] + tt12;
	uint64_t t13 = (uint64_t)a[13] + tt13;
	uint64_t t14 = (uint64_t)a[14] + tt14;
	uint64_t t15 = (uint64_t)a[15] + tt15;
	uint64_t t16 = (uint64_t)a[16] + tt16;
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
	t14 += t13 >> 32;
	t15 += t14 >> 32;
	t16 += t15 >> 32;
	uint32_t carry = t16 >> 32;
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
	t14 &= 0xffffffff;
	t15 &= 0xffffffff;
	t16 &= 0xffffffff;

	tt0 = (uint64_t)carry * _sc521_R[0];
	tt1 = (uint64_t)carry * _sc521_R[1];
	tt2 = (uint64_t)carry * _sc521_R[2];
	tt3 = (uint64_t)carry * _sc521_R[3];
	tt4 = (uint64_t)carry * _sc521_R[4];
	tt5 = (uint64_t)carry * _sc521_R[5];
	tt6 = (uint64_t)carry * _sc521_R[6];
	tt7 = (uint64_t)carry * _sc521_R[7];
	tt8 = (uint64_t)carry * _sc521_R[8];
	tt9 = (uint64_t)carry * _sc521_R[9];
	tt10 = (uint64_t)carry * _sc521_R[10];
	tt11 = (uint64_t)carry * _sc521_R[11];
	tt12 = (uint64_t)carry * _sc521_R[12];
	tt13 = (uint64_t)carry * _sc521_R[13];
	tt14 = (uint64_t)carry * _sc521_R[14];
	tt15 = (uint64_t)carry * _sc521_R[15];
	tt16 = (uint64_t)carry * _sc521_R[16];
	tt1 += tt0 >> 32;
	tt2 += tt1 >> 32;
	tt3 += tt2 >> 32;
	tt4 += tt3 >> 32;
	tt5 += tt4 >> 32;
	tt6 += tt5 >> 32;
	tt7 += tt6 >> 32;
	tt8 += tt7 >> 32;
	tt9 += tt8 >> 32;
	tt10 += tt9 >> 32;
	tt11 += tt10 >> 32;
	tt12 += tt11 >> 32;
	tt13 += tt12 >> 32;
	tt14 += tt13 >> 32;
	tt15 += tt14 >> 32;
	tt16 += tt15 >> 32;
	tt0 &= 0xffffffff;
	tt1 &= 0xffffffff;
	tt2 &= 0xffffffff;
	tt3 &= 0xffffffff;
	tt4 &= 0xffffffff;
	tt5 &= 0xffffffff;
	tt6 &= 0xffffffff;
	tt7 &= 0xffffffff;
	tt8 &= 0xffffffff;
	tt9 &= 0xffffffff;
	tt10 &= 0xffffffff;
	tt11 &= 0xffffffff;
	tt12 &= 0xffffffff;
	tt13 &= 0xffffffff;
	tt14 &= 0xffffffff;
	tt15 &= 0xffffffff;
	tt16 &= 0xffffffff;

	t0 += tt0;
	t1 += tt1;
	t2 += tt2;
	t3 += tt3;
	t4 += tt4;
	t5 += tt5;
	t6 += tt6;
	t7 += tt7;
	t8 += tt8;
	t9 += tt9;
	t10 += tt10;
	t11 += tt11;
	t12 += tt12;
	t13 += tt13;
	t14 += tt14;
	t15 += tt15;
	t16 += tt16;
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
	t14 += t13 >> 32;
	t15 += t14 >> 32;
	t16 += t15 >> 32;
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
	r[14] = t14 & 0xffffffff;
	r[15] = t15 & 0xffffffff;
	r[16] = t16 & 0xffffffff;

#endif
}

/* @func: _sc521_mod (static)
 * #desc:
 *    scalar modular reduction (norm).
 *
 * #1: r [in/out] number
 */
static void _sc521_mod(uint32_t r[17])
{
	uint32_t rr[17];
	uint32_t carry = 0, carry2 = 0;
	uint64_t tmp = 0;

	/* rr = ((r >> 521) + 1) * q */
	carry2 = (r[16] >> 9) + 1;
	for (int32_t i = 0; i < 17; i++) {
		tmp = (uint64_t)carry2 * _sc521_q[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	/* r = r - rr */
	carry2 = 0;
	for (int32_t i = 0; i < 17; i++) {
		tmp = (uint64_t)r[i] - rr[i] + (int32_t)carry2;
		r[i] = tmp & 0xffffffff;
		carry2 = tmp >> 32;
	}

	/* r = if r < q : adjust r */
	carry2 = ~(((uint64_t)carry2 - 1) >> 32);
	carry = 0;
	for (int32_t i = 0; i < 17; i++) {
		tmp = (uint64_t)r[i] + (_sc521_q[i] & carry2) + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}
}

/* @func: _sc521_add (static)
 * #desc:
 *    scalar addition.
 *
 * #1: r [out] sum
 * #2: a [in]  addend
 * #3: b [in]  addend
 */
static void _sc521_add(uint32_t r[17],
		const uint32_t a[17], const uint32_t b[17])
{
	uint32_t carry = 0;
	uint64_t tmp = 0;

	/* r = a + b */
	for (int32_t i = 0; i < 17; i++) {
		tmp = (uint64_t)a[i] + b[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	/* r = r % q */
	_sc521_modw(r, r, carry);
}

/* @func: _sc521_mul (static)
 * #desc:
 *    scalar multiplication.
 *
 * #1: r [out] product
 * #2: a [in]  multiplicand
 * #3: b [in]  multiplier
 */
static void _sc521_mul(uint32_t r[17],
		const uint32_t a[17], const uint32_t b[17])
{
#if 0

	uint32_t rr[34];
	uint32_t carry = 0;
	uint64_t tmp = 0;

	for (int32_t i = 0; i < 34; i++)
		rr[i] = 0;

	/* rr = a * b */
	for (int32_t i = 0; i < 17; i++) {
		tmp = (uint64_t)a[0] * b[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}
	rr[17] = carry;

	for (int32_t i = 1; i < 17; i++) {
		carry = 0;
		for (int32_t j = 0; j < 17; j++) {
			tmp = (uint64_t)a[i] * b[j] + rr[i + j] + carry;
			rr[i + j] = tmp & 0xffffffff;
			carry = tmp >> 32;
		}
		rr[i + 17] = carry;
	}

	/* r = rr % q */
	_sc521_modw(rr + 16, rr + 16, rr[33]);
	_sc521_modw(rr + 15, rr + 15, rr[32]);
	_sc521_modw(rr + 14, rr + 14, rr[31]);
	_sc521_modw(rr + 13, rr + 13, rr[30]);
	_sc521_modw(rr + 12, rr + 12, rr[29]);
	_sc521_modw(rr + 11, rr + 11, rr[28]);
	_sc521_modw(rr + 10, rr + 10, rr[27]);
	_sc521_modw(rr + 9, rr + 9, rr[26]);
	_sc521_modw(rr + 8, rr + 8, rr[25]);
	_sc521_modw(rr + 7, rr + 7, rr[24]);
	_sc521_modw(rr + 6, rr + 6, rr[23]);
	_sc521_modw(rr + 5, rr + 5, rr[22]);
	_sc521_modw(rr + 4, rr + 4, rr[21]);
	_sc521_modw(rr + 3, rr + 3, rr[20]);
	_sc521_modw(rr + 2, rr + 2, rr[19]);
	_sc521_modw(rr + 1, rr + 1, rr[18]);
	_sc521_modw(r, rr, rr[17]);

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
	uint64_t a14 = a[14];
	uint64_t a15 = a[15];
	uint64_t a16 = a[16];
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
	uint64_t b14 = b[14];
	uint64_t b15 = b[15];
	uint64_t b16 = b[16];

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
	uint64_t t14 = a0 * b14;
	uint64_t t15 = a0 * b15;
	uint64_t t16 = a0 * b16;
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
	t14 += t13 >> 32;
	t15 += t14 >> 32;
	t16 += t15 >> 32;
	uint64_t t17 = t16 >> 32;
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
	t14 &= 0xffffffff;
	t15 &= 0xffffffff;
	t16 &= 0xffffffff;

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
	t15 += a1 * b14;
	t16 += a1 * b15;
	t17 += a1 * b16;
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
	t15 += t14 >> 32;
	t16 += t15 >> 32;
	t17 += t16 >> 32;
	uint64_t t18 = t17 >> 32;
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
	t15 &= 0xffffffff;
	t16 &= 0xffffffff;
	t17 &= 0xffffffff;
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
	t16 += a2 * b14;
	t17 += a2 * b15;
	t18 += a2 * b16;
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
	t16 += t15 >> 32;
	t17 += t16 >> 32;
	t18 += t17 >> 32;
	uint64_t t19 = t18 >> 32;
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
	t16 &= 0xffffffff;
	t17 &= 0xffffffff;
	t18 &= 0xffffffff;
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
	t17 += a3 * b14;
	t18 += a3 * b15;
	t19 += a3 * b16;
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
	t17 += t16 >> 32;
	t18 += t17 >> 32;
	t19 += t18 >> 32;
	uint64_t t20 = t19 >> 32;
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
	t17 &= 0xffffffff;
	t18 &= 0xffffffff;
	t19 &= 0xffffffff;
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
	t18 += a4 * b14;
	t19 += a4 * b15;
	t20 += a4 * b16;
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
	t18 += t17 >> 32;
	t19 += t18 >> 32;
	t20 += t19 >> 32;
	uint64_t t21 = t20 >> 32;
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
	t18 &= 0xffffffff;
	t19 &= 0xffffffff;
	t20 &= 0xffffffff;
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
	t19 += a5 * b14;
	t20 += a5 * b15;
	t21 += a5 * b16;
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
	t19 += t18 >> 32;
	t20 += t19 >> 32;
	t21 += t20 >> 32;
	uint64_t t22 = t21 >> 32;
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
	t19 &= 0xffffffff;
	t20 &= 0xffffffff;
	t21 &= 0xffffffff;
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
	t20 += a6 * b14;
	t21 += a6 * b15;
	t22 += a6 * b16;
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
	t20 += t19 >> 32;
	t21 += t20 >> 32;
	t22 += t21 >> 32;
	uint64_t t23 = t22 >> 32;
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
	t20 &= 0xffffffff;
	t21 &= 0xffffffff;
	t22 &= 0xffffffff;
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
	t21 += a7 * b14;
	t22 += a7 * b15;
	t23 += a7 * b16;
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
	t21 += t20 >> 32;
	t22 += t21 >> 32;
	t23 += t22 >> 32;
	uint64_t t24 = t23 >> 32;
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
	t21 &= 0xffffffff;
	t22 &= 0xffffffff;
	t23 &= 0xffffffff;
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
	t22 += a8 * b14;
	t23 += a8 * b15;
	t24 += a8 * b16;
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
	t22 += t21 >> 32;
	t23 += t22 >> 32;
	t24 += t23 >> 32;
	uint64_t t25 = t24 >> 32;
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
	t22 &= 0xffffffff;
	t23 &= 0xffffffff;
	t24 &= 0xffffffff;
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
	t23 += a9 * b14;
	t24 += a9 * b15;
	t25 += a9 * b16;
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
	t23 += t22 >> 32;
	t24 += t23 >> 32;
	t25 += t24 >> 32;
	uint64_t t26 = t25 >> 32;
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
	t23 &= 0xffffffff;
	t24 &= 0xffffffff;
	t25 &= 0xffffffff;
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
	t24 += a10 * b14;
	t25 += a10 * b15;
	t26 += a10 * b16;
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
	t24 += t23 >> 32;
	t25 += t24 >> 32;
	t26 += t25 >> 32;
	uint64_t t27 = t26 >> 32;
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
	t24 &= 0xffffffff;
	t25 &= 0xffffffff;
	t26 &= 0xffffffff;
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
	t25 += a11 * b14;
	t26 += a11 * b15;
	t27 += a11 * b16;
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
	t25 += t24 >> 32;
	t26 += t25 >> 32;
	t27 += t26 >> 32;
	uint64_t t28 = t27 >> 32;
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
	t25 &= 0xffffffff;
	t26 &= 0xffffffff;
	t27 &= 0xffffffff;
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
	t26 += a12 * b14;
	t27 += a12 * b15;
	t28 += a12 * b16;
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
	t26 += t25 >> 32;
	t27 += t26 >> 32;
	t28 += t27 >> 32;
	uint64_t t29 = t28 >> 32;
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
	t26 &= 0xffffffff;
	t27 &= 0xffffffff;
	t28 &= 0xffffffff;
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
	t27 += a13 * b14;
	t28 += a13 * b15;
	t29 += a13 * b16;
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
	t27 += t26 >> 32;
	t28 += t27 >> 32;
	t29 += t28 >> 32;
	uint64_t t30 = t29 >> 32;
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
	t27 &= 0xffffffff;
	t28 &= 0xffffffff;
	t29 &= 0xffffffff;
	t14 += a14 * b0;
	t15 += a14 * b1;
	t16 += a14 * b2;
	t17 += a14 * b3;
	t18 += a14 * b4;
	t19 += a14 * b5;
	t20 += a14 * b6;
	t21 += a14 * b7;
	t22 += a14 * b8;
	t23 += a14 * b9;
	t24 += a14 * b10;
	t25 += a14 * b11;
	t26 += a14 * b12;
	t27 += a14 * b13;
	t28 += a14 * b14;
	t29 += a14 * b15;
	t30 += a14 * b16;
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
	t27 += t26 >> 32;
	t28 += t27 >> 32;
	t29 += t28 >> 32;
	t30 += t29 >> 32;
	uint64_t t31 = t30 >> 32;
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
	t27 &= 0xffffffff;
	t28 &= 0xffffffff;
	t29 &= 0xffffffff;
	t30 &= 0xffffffff;
	t15 += a15 * b0;
	t16 += a15 * b1;
	t17 += a15 * b2;
	t18 += a15 * b3;
	t19 += a15 * b4;
	t20 += a15 * b5;
	t21 += a15 * b6;
	t22 += a15 * b7;
	t23 += a15 * b8;
	t24 += a15 * b9;
	t25 += a15 * b10;
	t26 += a15 * b11;
	t27 += a15 * b12;
	t28 += a15 * b13;
	t29 += a15 * b14;
	t30 += a15 * b15;
	t31 += a15 * b16;
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
	t27 += t26 >> 32;
	t28 += t27 >> 32;
	t29 += t28 >> 32;
	t30 += t29 >> 32;
	t31 += t30 >> 32;
	uint64_t t32 = t31 >> 32;
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
	t27 &= 0xffffffff;
	t28 &= 0xffffffff;
	t29 &= 0xffffffff;
	t30 &= 0xffffffff;
	t31 &= 0xffffffff;
	t16 += a16 * b0;
	t17 += a16 * b1;
	t18 += a16 * b2;
	t19 += a16 * b3;
	t20 += a16 * b4;
	t21 += a16 * b5;
	t22 += a16 * b6;
	t23 += a16 * b7;
	t24 += a16 * b8;
	t25 += a16 * b9;
	t26 += a16 * b10;
	t27 += a16 * b11;
	t28 += a16 * b12;
	t29 += a16 * b13;
	t30 += a16 * b14;
	t31 += a16 * b15;
	t32 += a16 * b16;
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
	t27 += t26 >> 32;
	t28 += t27 >> 32;
	t29 += t28 >> 32;
	t30 += t29 >> 32;
	t31 += t30 >> 32;
	t32 += t31 >> 32;
	uint64_t t33 = t32 >> 32;
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
	t27 &= 0xffffffff;
	t28 &= 0xffffffff;
	t29 &= 0xffffffff;
	t30 &= 0xffffffff;
	t31 &= 0xffffffff;
	t32 &= 0xffffffff;

	uint32_t T[34];

	T[0] = t0 & 0xffffffff;
	T[1] = t1 & 0xffffffff;
	T[2] = t2 & 0xffffffff;
	T[3] = t3 & 0xffffffff;
	T[4] = t4 & 0xffffffff;
	T[5] = t5 & 0xffffffff;
	T[6] = t6 & 0xffffffff;
	T[7] = t7 & 0xffffffff;
	T[8] = t8 & 0xffffffff;
	T[9] = t9 & 0xffffffff;
	T[10] = t10 & 0xffffffff;
	T[11] = t11 & 0xffffffff;
	T[12] = t12 & 0xffffffff;
	T[13] = t13 & 0xffffffff;
	T[14] = t14 & 0xffffffff;
	T[15] = t15 & 0xffffffff;
	T[16] = t16 & 0xffffffff;
	T[17] = t17 & 0xffffffff;
	T[18] = t18 & 0xffffffff;
	T[19] = t19 & 0xffffffff;
	T[20] = t20 & 0xffffffff;
	T[21] = t21 & 0xffffffff;
	T[22] = t22 & 0xffffffff;
	T[23] = t23 & 0xffffffff;
	T[24] = t24 & 0xffffffff;
	T[25] = t25 & 0xffffffff;
	T[26] = t26 & 0xffffffff;
	T[27] = t27 & 0xffffffff;
	T[28] = t28 & 0xffffffff;
	T[29] = t29 & 0xffffffff;
	T[30] = t30 & 0xffffffff;
	T[31] = t31 & 0xffffffff;
	T[32] = t32 & 0xffffffff;
	T[33] = t33 & 0xffffffff;

	_sc521_modw(T + 16, T + 16, T[33]);
	_sc521_modw(T + 15, T + 15, T[32]);
	_sc521_modw(T + 14, T + 14, T[31]);
	_sc521_modw(T + 13, T + 13, T[30]);
	_sc521_modw(T + 12, T + 12, T[29]);
	_sc521_modw(T + 11, T + 11, T[28]);
	_sc521_modw(T + 10, T + 10, T[27]);
	_sc521_modw(T + 9, T + 9, T[26]);
	_sc521_modw(T + 8, T + 8, T[25]);
	_sc521_modw(T + 7, T + 7, T[24]);
	_sc521_modw(T + 6, T + 6, T[23]);
	_sc521_modw(T + 5, T + 5, T[22]);
	_sc521_modw(T + 4, T + 4, T[21]);
	_sc521_modw(T + 3, T + 3, T[20]);
	_sc521_modw(T + 2, T + 2, T[19]);
	_sc521_modw(T + 1, T + 1, T[18]);
	_sc521_modw(r, T, T[17]);

#endif
}

/* @func: _sc521_inv (static)
 * #desc:
 *    modular inversion based on fermat's little theorem.
 *
 * #1: r [out] inverse modulus
 * #2: z [in]  number
 */
static void _sc521_inv(uint32_t r[17], const uint32_t z[17])
{
	uint32_t x[17], e[17];
	for (int32_t i = 0; i < 17; i++) {
		x[i] = z[i];
		e[i] = _sc521_q[i];
		r[i] = 0;
	}
	e[0] -= 2;
	r[0] = 1;

	for (int32_t i = 0; i < 521; i++) {
		uint32_t k_i = e[i / 32] >> (i % 32);
		if (k_i & 1)
			_sc521_mul(r, r, x);
		_sc521_mul(x, x, x);
	}
}

/* @func: _sc521_digest (static)
 * #desc:
 *    sha512 digest modular reduction.
 *
 * #1: dig [in]  sha521 digest
 * #2: r   [out] result
 */
static void _sc521_digest(const uint8_t dig[64], uint32_t r[17])
{
	r[16] = 0; /* mask */

	/* big endian */
	for (int32_t i = 0; i < 64; i++)
		((uint8_t *)r)[63 - i] = dig[i];

	/* conch_memcpy(r, dig, 64); */

	/* r = r % q */
	_sc521_mod(r);
}

/* @func: _p521_sub (static)
 * #desc:
 *    nonprime field subtraction.
 *
 * #1: r [out] difference
 * #2: a [in]  minuend
 * #3: b [in]  subtract
 * #r:   [ret] overflow
 */
static uint32_t _p521_sub(uint32_t r[17],
		const uint32_t a[17], const uint32_t b[17])
{
	uint32_t carry = 0;
	uint64_t tmp = 0;

	/* r = a - b */
	for (int32_t i = 0; i < 17; i++) {
		tmp = (uint64_t)a[i] - b[i] + (int32_t)carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	return carry;
}

/* @func: _p521_point_add (static)
 * #desc:
 *    jacobian-affine curve point addition.
 *
 * #1: xyz1 [in]  addend
 * #2: xyz2 [in]  addend
 * #3: xyz3 [out] result
 */
static void _p521_point_add(const struct p521_point *xyz1,
		const struct p521_point *xyz2, struct p521_point *xyz3)
{
	uint32_t H1[17], R1[17], H2[17], H3[17], C1[17], T1[17], T2[17];
	/*
	 * H1 = (x2 * (z1 ^ 2)) - x1
	 * R1 = (y2 * (z1 ^ 3)) - y1
	 * H2 = H1 ^ 2
	 * H3 = H1 ^ 3
	 * C1 = x1 * H2
	 * x3 = (R1 ^ 2) - (C1 * 2) - H3
	 * y3 = (R1 * (C1 - x3)) - (y1 * H3)
	 * z3 = z1 * H1
	 */

	/* NOTE: jacobian-affine point addition in CMO98 */

	/* H1 = (x2 * (z1 ^ 2)) - x1 */
	_fp521_mul(T1, xyz1->z, xyz1->z);
	_fp521_mul(H1, T1, xyz2->x);
	_fp521_sub(H1, H1, xyz1->x);

	/* R1 = (y2 * (z1 ^ 3)) - y1 */
	_fp521_mul(R1, T1, xyz1->z);
	_fp521_mul(R1, R1, xyz2->y);
	_fp521_sub(R1, R1, xyz1->y);

	/* H2 = H1 ^ 2 */
	_fp521_mul(H2, H1, H1);
	/* H3 = H1 ^ 3 */
	_fp521_mul(H3, H2, H1);
	/* C1 = x1 * H2 */
	_fp521_mul(C1, xyz1->x, H2);

	/* x3 = (R1 ^ 2) - (C1 * 2) - H3 */
	_fp521_mul(T1, R1, R1);
	_fp521_sub(T1, T1, C1);
	_fp521_sub(T1, T1, C1);
	_fp521_sub(xyz3->x, T1, H3);

	/* y3 = (R1 * (C1 - x3)) - (y1 * H3) */
	_fp521_sub(T1, C1, xyz3->x);
	_fp521_mul(T1, T1, R1);
	_fp521_mul(T2, xyz1->y, H3);
	_fp521_sub(xyz3->y, T1, T2);

	/* z3 = z1 * H1 */
	_fp521_mul(xyz3->z, xyz1->z, H1);
}

/* @func: _p521_point_double (static)
 * #desc:
 *    jacobian curve point doubling.
 *
 * #1: xyz1 [in]  addend
 * #2: xyz3 [out] result
 */
static void _p521_point_double(const struct p521_point *xyz1,
		struct p521_point *xyz3)
{
	uint32_t M1[17], S1[17], U1[17], T1[17];
	/*
	 * M1 = ((x1 ^ 2) * 3) + (a * (z1 ^ 4))
	 * S1 = (x1 * 4) * (y1 ^ 2)
	 * U1 = (y1 ^ 4) * 8
	 * z3 = (y1 * z1) * 2
	 * x3 = (M1 ^ 2) - (S1 * 2)
	 * y3 = (M1 * (S1 - x3)) - U1
	 */

	/* NOTE: jacobian point doubling in CMO98 */

	/* M1 = ((x1 ^ 2) * 3) + (a * (z1 ^ 4)) */
	_fp521_mul(T1, xyz1->x, xyz1->x);
	_fp521_add(M1, T1, T1);
	_fp521_add(M1, M1, T1);
	_fp521_mul(T1, xyz1->z, xyz1->z);
	_fp521_mul(T1, T1, T1);
	_fp521_mul(T1, T1, _p521_a);
	_fp521_add(M1, M1, T1);

	/* S1 = (x1 * 4) * (y1 ^ 2) */
	_fp521_add(S1, xyz1->x, xyz1->x);
	_fp521_add(S1, S1, S1);
	_fp521_mul(T1, xyz1->y, xyz1->y);
	_fp521_mul(S1, S1, T1);

	/* U1 = (y1 ^ 4) * 8 */
	_fp521_mul(T1, T1, T1);
	_fp521_add(T1, T1, T1);
	_fp521_add(T1, T1, T1);
	_fp521_add(U1, T1, T1);

	/* z3 = (y1 * z1) * 2 */
	_fp521_mul(T1, xyz1->y, xyz1->z);
	_fp521_add(xyz3->z, T1, T1);

	/* x3 = (M1 ^ 2) - (S1 * 2) */
	_fp521_mul(T1, M1, M1);
	_fp521_sub(T1, T1, S1);
	_fp521_sub(xyz3->x, T1, S1);

	/* y3 = (M1 * (S1 - x3)) - U1 */
	_fp521_sub(T1, S1, xyz3->x);
	_fp521_mul(T1, T1, M1);
	_fp521_sub(xyz3->y, T1, U1);
}

/* @func: _p521_point_to_affine (static)
 * #desc:
 *    jacobian to affine coordinates conversion.
 *
 * #1: xyz1 [in]  jacobian point
 * #2: xy2  [out] affine point
 */
static void _p521_point_to_affine(const struct p521_point *xyz1,
		struct p521_point *xy2)
{
	uint32_t z[17], t[17];

	/* z = inv(z1) */
	_fp521_inv(z, xyz1->z);

	/* x2 = (x1 * (z ^ 2)) % p */
	_fp521_mul(t, z, z);
	_fp521_mul(xy2->x, xyz1->x, t);
	_fp521_mod(xy2->x);

	/* y2 = (y1 * (z ^ 3)) % p */
	_fp521_mul(t, t, z);
	_fp521_mul(xy2->y, xyz1->y, t);
	_fp521_mod(xy2->y);
}

/* @func: _p521_scalar_mul (static)
 * #desc:
 *    p521 signed binary ladder scalar multiplication.
 *
 * #1: k   [in]  input point
 * #2: xy1 [in]  base point
 * #3: xy2 [out] scalar
 */
static void _p521_scalar_mul(const uint32_t k[17],
		const struct p521_point *xy1, struct p521_point *xy2)
{
	struct p521_point _xyz1, _xyz2;
	uint32_t t[17], y_neg[17], neg = 0;
	for (int32_t i = 0; i < 17; i++) {
		_xyz1.x[i] = xy1->x[i];
		_xyz1.y[i] = xy1->y[i];
		_xyz1.z[i] = 0;
		_xyz2.x[i] = xy1->x[i];
		y_neg[i] = 0;
	}
	_xyz1.z[0] = 1;
	/*
	 * _xyz1 = (x1, y1, 1)
	 * _xyz2 = (x1, y1 ? y_neg)
	 */

	/* t = if k & 1 : move k */
	_p521_sub(t, _sc521_q, k);
	neg = ~k[0] & 1;
	_fp521_move(t, k, ~neg);

	/* y_neg = -y1 */
	_fp521_sub(y_neg, y_neg, xy1->y);
	_fp521_move(_xyz1.y, y_neg, neg);

	/* signed binary ladder and jacobian-affine coordinates */
	for (int32_t i = 520; i > 0; i--) {
		uint32_t k_i = t[i / 32] >> (i % 32);
		_fp521_move(_xyz2.y, xy1->y, k_i ^ neg);
		_fp521_move(_xyz2.y, y_neg, ~k_i ^ neg);
		/* R = (R * 2) +- P */

		_p521_point_double(&_xyz1, &_xyz1);
		_p521_point_add(&_xyz1, &_xyz2, &_xyz1);
	}

	_p521_point_to_affine(&_xyz1, xy2);
}

/* @func: _p521_check_key (static)
 * #desc:
 *    input point legality check.
 *
 * #1: k [in]  input point
 * #r:   [ret] 0: no error, -1: error
 */
static int32_t _p521_check_key(const uint32_t k[17])
{
	uint32_t t[17];

	/* if k > 0 && k < q */
	if (_fp521_iszero(k) || !_p521_sub(t, k, _sc521_q))
		return -1;

	return 0;
}

/* @func: _p521_check_point (static)
 * #desc:
 *    curve point legality check.
 *
 * #1: xy1 [in]  curve point
 * #r:     [ret] 0: no error, -1: error
 */
static int32_t _p521_check_point(const struct p521_point *xy1)
{
	uint32_t t1[17], t2[17];

	if (!_p521_sub(t1, xy1->x, _fp521_p))
		return -1;
	if (!_p521_sub(t1, xy1->y, _fp521_p))
		return -1;

	/* t1 = ((x1 ^ 3) - (x1 * 3) + b) % p */
	_fp521_mul(t1, xy1->x, xy1->x);
	_fp521_mul(t1, t1, xy1->x);
	_fp521_sub(t1, t1, xy1->x);
	_fp521_sub(t1, t1, xy1->x);
	_fp521_sub(t1, t1, xy1->x);
	_fp521_add(t1, t1, _p521_b);
	_fp521_mod(t1);

	/* t2 = (y1 ^ 2) % p */
	_fp521_mul(t2, xy1->y, xy1->y);
	_fp521_mod(t2);

	_p521_sub(t1, t1, t2);

	return _fp521_iszero(t1) - 1;
}

/* @func: _p521_affine_add (static)
 * #desc:
 *    affine curve point addition.
 *
 * #1: xy1 [in]  addend
 * #2: xy2 [in]  addend
 * #3: xy3 [out] result
 */
static void _p521_affine_add(const struct p521_point *xy1,
		const struct p521_point *xy2, struct p521_point *xy3)
{
	struct p521_point _xyz1, _xyz2, _xyz3, _xy4;
	uint32_t t[17], iswap1, iswap2;
	for (int32_t i = 0; i < 17; i++) {
		_xyz1.x[i] = xy1->x[i];
		_xyz1.y[i] = xy1->y[i];
		_xyz1.z[i] = 0;
		_xyz2.x[i] = xy2->x[i];
		_xyz2.y[i] = xy2->y[i];
		_xyz2.z[i] = 0;
		_xy4.x[i] = 0;
		_xy4.y[i] = 0;
	}
	_xyz1.z[0] = 1;
	_xyz2.z[0] = 1;

	_p521_sub(t, xy1->x, xy2->x);
	iswap1 = _fp521_iszero(t);
	_p521_sub(t, xy1->y, xy2->y);
	iswap2 = _fp521_iszero(t);

	/* P != +- Q */
	_p521_point_add(&_xyz1, &_xyz2, &_xyz1);
	/* P == Q */
	_p521_point_double(&_xyz1, &_xyz3);

	/* xyz1 = if x1 == x2 && y1 == y2 : swap xyz3 */
	_fp521_swap(_xyz1.x, _xyz3.x, iswap1 & iswap2);
	_fp521_swap(_xyz1.y, _xyz3.y, iswap1 & iswap2);
	_fp521_swap(_xyz1.z, _xyz3.z, iswap1 & iswap2);

	_p521_point_to_affine(&_xyz1, xy3);

	/* xy3 = if x1 == x2 && y1 != y2 : adjust zero */
	_fp521_swap(xy3->x, _xy4.x, iswap1 & ~iswap2);
	_fp521_swap(xy3->y, _xy4.y, iswap1 & ~iswap2);
}

/* @func: conch_ecdh_p521_public_key
 * #desc:
 *    p521 ecdh public key create function.
 *
 * #1: pri [in]  private key
 * #2: pub [out] public key
 * #r:     [ret] 0: success, -1: fail
 */
int32_t conch_ecdh_p521_public_key(const uint8_t *pri, uint8_t *pub)
{
	uint32_t _pri[17];
	struct p521_point xy1;
	_pri[16] = 0; /* mask */

	conch_memcpy(_pri, pri, ECDH_P521_PRI_LEN);
	if (_p521_check_key(_pri))
		return -1;

	/* xy1 = scalar(_pri, base) */
	_p521_scalar_mul(_pri, &_p521_base, &xy1);

	conch_memcpy(pub, xy1.x, ECDH_P521_LEN);
	conch_memcpy(pub + ECDH_P521_LEN, xy1.y, ECDH_P521_LEN);

	return 0;
}

/* @func: conch_ecdh_p521_shared_key
 * #desc:
 *    p521 ecdh shared key create function.
 *
 * #1: pri [in]  private key
 * #2: pub [in]  public key
 * #3: key [in]  shared key
 * #r:     [ret] 0: success, -1: fail
 */
int32_t conch_ecdh_p521_shared_key(const uint8_t *pri,
		const uint8_t *pub, uint8_t *key)
{
	uint32_t _pri[17];
	struct p521_point xy1, xy2;
	_pri[16] = 0; /* mask */
	xy1.x[16] = xy1.y[16] = 0;

	conch_memcpy(_pri, pri, ECDH_P521_PRI_LEN);
	if (_p521_check_key(_pri))
		return -1;

	conch_memcpy(xy1.x, pub, ECDH_P521_LEN);
	conch_memcpy(xy1.y, pub + ECDH_P521_LEN, ECDH_P521_LEN);
	if (_p521_check_point(&xy1))
		return -1;

	/* xy2 = scalar(_pri, xy1) */
	_p521_scalar_mul(_pri, &xy1, &xy2);

	conch_memcpy(key, xy2.x, ECDH_P521_KEY_LEN);

	return 0 - _fp521_iszero(xy2.x);
}

/* @func: conch_ecdsa_p521_public_key
 * #desc:
 *    p521 ecdsa public key create function.
 *
 * #1: pri [in]  private key
 * #2: pub [out] public key
 * #r:     [ret] 0: success, -1: fail
 */
int32_t conch_ecdsa_p521_public_key(const uint8_t *pri, uint8_t *pub)
{
	uint32_t _pri[17];
	struct p521_point xy1;
	_pri[16] = 0; /* mask */

	conch_memcpy(_pri, pri, ECDSA_P521_PRI_LEN);
	if (_p521_check_key(_pri))
		return -1;

	/* xy1 = scalar(_pri, base) */
	_p521_scalar_mul(_pri, &_p521_base, &xy1);

	conch_memcpy(pub, xy1.x, ECDSA_P521_LEN);
	conch_memcpy(pub + ECDSA_P521_LEN, xy1.y, ECDSA_P521_LEN);

	return 0;
}

/* @func: conch_ecdsa_p521_sign
 * #desc:
 *    p521 ecdsa signature function.
 *
 * #1: pri  [in]  private key
 * #2: ran  [in]  nonce
 * #3: msg  [in]  input message
 * #4: len  [in]  message length
 * #5: sign [out] signature
 * #r:      [ret] 0: success, -1: fail
 */
int32_t conch_ecdsa_p521_sign(const uint8_t *pri, const uint8_t *ran,
		const uint8_t *msg, uint32_t len, uint8_t *sign)
{
	uint32_t _pri[17], _ran[17], z[17], s[17];
	struct p521_point xy1;
	SHA512_NEW(ctx);
	_pri[16] = 0; /* mask */
	_ran[16] = 0;

	conch_memcpy(_pri, pri, ECDSA_P521_PRI_LEN);
	conch_memcpy(_ran, ran, ECDSA_P521_RAN_LEN);
	if (_p521_check_key(_pri) || _p521_check_key(_ran))
		return -1;

	/* x1 = scalar(_ran, base) */
	_p521_scalar_mul(_ran, &_p521_base, &xy1);
	_sc521_mod(xy1.x);

	if (_fp521_iszero(xy1.x))
		return -1;

	/* z = sha(msg) % q */
	conch_sha512_init(&ctx);
	conch_sha512(&ctx, msg, len);
	_sc521_digest(&(SHA512_STATE(&ctx, 0)), z);

	/* s = (x1 * _pri) + z */
	_sc521_mul(s, xy1.x, _pri);
	_sc521_add(s, s, z);

	/* s = (s * inv(_ran)) % q */
	_sc521_inv(z, _ran);
	_sc521_mul(s, s, z);
	_sc521_mod(s);

	if (_fp521_iszero(s))
		return -1;

	conch_memcpy(sign, xy1.x, ECDSA_P521_LEN);
	conch_memcpy(sign + ECDSA_P521_LEN, s, ECDSA_P521_LEN);

	return 0;
}

/* @func: conch_ecdsa_p521_verify
 * #desc:
 *    p521 ecdsa signature verification function.
 *
 * #1: pub  [in]  public key
 * #2: sign [in]  signature
 * #3: msg  [in]  input message
 * #4: len  [in]  message length
 * #r:      [ret] 0: success, -1: fail
 */
int32_t conch_ecdsa_p521_verify(const uint8_t *pub,
		const uint8_t *sign, const uint8_t *msg, uint32_t len)
{
	uint32_t z[17], t[17], u1[17], u2[17];
	struct p521_point xy1, xy2, xy3;
	SHA512_NEW(ctx);
	xy1.x[16] = xy1.y[16] = 0; /* mask */
	xy2.x[16] = xy2.y[16] = 0;

	/* xy1 = sign */
	conch_memcpy(xy1.x, sign, ECDSA_P521_LEN);
	conch_memcpy(xy1.y, sign + ECDSA_P521_LEN, ECDSA_P521_LEN);
	if (_p521_check_key(xy1.x) || _p521_check_key(xy1.y))
		return -1;

	/* xy2 = pub */
	conch_memcpy(xy2.x, pub, ECDSA_P521_LEN);
	conch_memcpy(xy2.y, pub + ECDSA_P521_LEN, ECDSA_P521_LEN);
	if (_p521_check_point(&xy2))
		return -1;

	/* z = sha(msg) % q */
	conch_sha512_init(&ctx);
	conch_sha512(&ctx, msg, len);
	_sc521_digest(&(SHA512_STATE(&ctx, 0)), z);

	/* t = inv(y1) */
	_sc521_inv(t, xy1.y);

	/* u1 = (z * t) % q */
	_sc521_mul(u1, z, t);
	_sc521_mod(u1);

	/* u2 = (x1 * t) % q */
	_sc521_mul(u2, xy1.x, t);
	_sc521_mod(u2);

	/* xy2 = scalar(u2, xy2) */
	_p521_scalar_mul(u2, &xy2, &xy2);
	/* xy3 = scalar(u1, base) */
	_p521_scalar_mul(u1, &_p521_base, &xy3);

	/* x2 = add(xy3, xy2) % q */
	_p521_affine_add(&xy3, &xy2, &xy2);
	_sc521_mod(xy2.x);

	_p521_sub(t, xy1.x, xy2.x);

	return _fp521_iszero(t) - 1;
}
