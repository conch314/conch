/* @file: nist-p384.c
 * #desc:
 *    The implementations of nist p384 ecdh and ecdsa.
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


/* P = 2^384 - 2^128 - 2^96 + 2^32 - 1 */
static const uint32_t _fp384_p[12] = {
	0xffffffff, 0x00000000, 0x00000000, 0xffffffff,
	0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
	};

/* p_R = 2^384 % P */
static const uint32_t _fp384_p_R[12] = {
	1, 0xffffffff, 0xffffffff, 0, 1, 0, 0, 0, 0, 0, 0, 0
	};

/* A = -3 % P */
static const uint32_t _p384_a[12] = {
	0xfffffffc, 0x00000000, 0x00000000, 0xffffffff,
	0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
	};

/*
 * B = 2758019355995970587784901184038904809305690585636156852142870730198
 *     8689241309860865136260764883745107765439761230575
 */
static const uint32_t _p384_b[12] = {
	0xd3ec2aef, 0x2a85c8ed, 0x8a2ed19d, 0xc656398d,
	0x5013875a, 0x0314088f, 0xfe814112, 0x181d9c6e,
	0xe3f82d19, 0x988e056b, 0xe23ee7e4, 0xb3312fa7
	};

/*
 * Q = 3940200619639447921227904010014361380507973927046544666794690527962
 *     7659399113263569398956308152294913554433653942643
 */
static const uint32_t _sc384_q[12] = {
	0xccc52973, 0xecec196a, 0x48b0a77a, 0x581a0db2,
	0xf4372ddf, 0xc7634d81, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
	};

/*
 * Bx = 262470350957996892686231567445669818918529234911092133878156159009
 *      25518854738050089022388053975719786650872476732087
 * By = 832571096148902998554675128952010817928785304886131559470920590248
 *      0503199884419224438643760392947333078086511627871
 */
struct p384_point {
	uint32_t x[12];
	uint32_t y[12];
	uint32_t z[12]; /* jacobian coordinates */
};

static const struct p384_point _p384_base = {
	{
		0x72760ab7, 0x3a545e38, 0xbf55296c, 0x5502f25d,
		0x82542a38, 0x59f741e0, 0x8ba79b98, 0x6e1d3b62,
		0xf320ad74, 0x8eb1c71e, 0xbe8b0537, 0xaa87ca22
	},
	{
		0x90ea0e5f, 0x7a431d7c, 0x1d7e819d, 0x0a60b1ce,
		0xb5f0b8c0, 0xe9da3113, 0x289a147c, 0xf8f41dbd,
		0x9292dc29, 0x5d9e98bf, 0x96262c6f, 0x3617de4a
	},
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	}
	};

/*
 * R = 2^384 % Q
 *   1388124618062372383947042015309946732620727252194336364173
 */
static const uint32_t _sc384_R[12] = {
	0x333ad68d, 0x1313e695, 0xb74f5885, 0xa7e5f24d,
	0x0bc8d220, 0x389cb27e, 0, 0, 0, 0, 0, 0
	};


/* @func: _fp384_swap (static)
 * #desc:
 *    constant-time the numerical swap.
 *
 * #1: a   [in/out] number
 * #2: b   [in/out] number
 * #3: bit [in]     low bit (0: keep, 1: swap)
 */
static void _fp384_swap(uint32_t a[12], uint32_t b[12], uint32_t bit)
{
	bit = (~bit & 1) - 1;
	for (int32_t i = 0; i < 12; i++) {
		uint32_t t = bit & (a[i] ^ b[i]);
		a[i] ^= t;
		b[i] ^= t;
	}
}

/* @func: _fp384_move (static)
 * #desc:
 *    constant-time the move numerical.
 *
 * #1: a   [in/out] number
 * #2: b   [in]     number
 * #3: bit [in]     low bit (0: keep, 1: swap)
 */
static void _fp384_move(uint32_t a[12], const uint32_t b[12], uint32_t bit)
{
	bit = (~bit & 1) - 1;
	for (int32_t i = 0; i < 12; i++) {
		uint32_t t = bit & (a[i] ^ b[i]);
		a[i] ^= t;
	}
}

/* @func: _fp384_iszero (static)
 * #desc:
 *    check if a number is zero in constant-time.
 *
 * #1: a [in]  number
 * #r:   [ret] 0: non-zero, 1: is zero
 */
static uint32_t _fp384_iszero(const uint32_t a[12])
{
	uint64_t r = 0;
	for (int32_t i = 0; i < 12; i++)
		r |= a[i];

	return ((r - 1) >> 32) & 1;
}

/* @func: _fp384_add (static)
 * #desc:
 *    prime field addition.
 *
 * #1: r [out] sum
 * #2: a [in]  addend
 * #3: b [in]  addend
 */
static void _fp384_add(uint32_t r[12],
		const uint32_t a[12], const uint32_t b[12])
{
#if 0

	uint32_t carry = 0, carry2 = 0;
	uint64_t tmp = 0;

	/* r = a + b */
	for (int32_t i = 0; i < 12; i++) {
		tmp = (uint64_t)a[i] + b[i] + carry2;
		r[i] = tmp & 0xffffffff;
		carry2 = tmp >> 32;
	}

	/* r = r % p modular reduction */
	carry2 = ~(((uint64_t)carry2 - 1) >> 32);
	for (int32_t i = 0; i < 12; i++) {
		tmp = (uint64_t)r[i] - (_fp384_p[i] & carry2) + (int32_t)carry;
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
	uint32_t mask = ~(((t11 >> 32) - 1) >> 32);
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

	t0 -= _fp384_p[0] & mask;
	t1 -= _fp384_p[1] & mask;
	t2 -= _fp384_p[2] & mask;
	t3 -= _fp384_p[3] & mask;
	t4 -= _fp384_p[4] & mask;
	t5 -= _fp384_p[5] & mask;
	t6 -= _fp384_p[6] & mask;
	t7 -= _fp384_p[7] & mask;
	t8 -= _fp384_p[8] & mask;
	t9 -= _fp384_p[9] & mask;
	t10 -= _fp384_p[10] & mask;
	t11 -= _fp384_p[11] & mask;
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

#endif
}

/* @func: _fp384_sub (static)
 * #desc:
 *    prime field subtraction.
 *
 * #1: r [out] difference
 * #2: a [in]  minuend
 * #3: b [in]  subtract
 */
static void _fp384_sub(uint32_t r[12],
		const uint32_t a[12], const uint32_t b[12])
{
#if 0

	uint32_t carry = 0, carry2 = 0;
	uint64_t tmp = 0;

	/* r = a - b */
	for (int32_t i = 0; i < 12; i++) {
		tmp = (uint64_t)a[i] - b[i] + (int32_t)carry2;
		r[i] = tmp & 0xffffffff;
		carry2 = tmp >> 32;
	}

	/* r = r % p modular reduction */
	carry2 = ~(((uint64_t)carry2 - 1) >> 32);
	for (int32_t i = 0; i < 12; i++) {
		tmp = (uint64_t)r[i] + (_fp384_p[i] & carry2) + carry;
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
	uint32_t mask = ~(((t11 >> 32) - 1) >> 32);
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

	t0 += _fp384_p[0] & mask;
	t1 += _fp384_p[1] & mask;
	t2 += _fp384_p[2] & mask;
	t3 += _fp384_p[3] & mask;
	t4 += _fp384_p[4] & mask;
	t5 += _fp384_p[5] & mask;
	t6 += _fp384_p[6] & mask;
	t7 += _fp384_p[7] & mask;
	t8 += _fp384_p[8] & mask;
	t9 += _fp384_p[9] & mask;
	t10 += _fp384_p[10] & mask;
	t11 += _fp384_p[11] & mask;
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

#endif
}

/* @func: _fp384_mul (static)
 * #desc:
 *    prime field multiplication.
 *
 * #1: r [out] product
 * #2: a [in]  multiplicand
 * #3: b [in]  multiplier
 */
static void _fp384_mul(uint32_t r[12],
		const uint32_t a[12], const uint32_t b[12])
{
#if 0

	uint32_t rr[24], t[12];
	uint32_t carry = 0;
	uint64_t tmp = 0;

	for (int32_t i = 0; i < 24; i++)
		rr[i] = 0;

	/* rr = a * b */
	for (int32_t i = 0; i < 12; i++) {
		tmp = (uint64_t)a[0] * b[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}
	rr[12] = carry;

	for (int32_t i = 1; i < 12; i++) {
		carry = 0;
		for (int32_t j = 0; j < 12; j++) {
			tmp = (uint64_t)a[i] * b[j] + rr[i + j] + carry;
			rr[i + j] = tmp & 0xffffffff;
			carry = tmp >> 32;
		}
		rr[i + 12] = carry;
	}

	/* NOTE: NIST.FIPS.186-4 -- D.2.4 */

	/* r = rr % p modular reduction fold */
	for (int32_t i = 0; i < 12; i++)
		r[i] = rr[i];

	/* B = T + 2S1 + S2 + S3 + S4 + S5 + S6 - D1 - D2 - D3 */

	/* S1 = 0  |0  |0  |0  |A21|A22|A23|0  |0  |0  |0  |0 */
	t[0] = t[1] = t[2] = t[3] = 0;
	t[4] = rr[21]; t[5] = rr[22]; t[6] = rr[23];
	t[7] = t[8] = t[9] = t[10] = t[11] = 0;

	_fp384_add(t, t, t);
	_fp384_add(r, r, t);

	/* S2 = A12|A13|A14|A15|A16|A17|A18|A19|A20|A21|A22|A23 */
	for (int32_t i = 0; i < 12; i++)
		t[i] = rr[i + 12];

	_fp384_add(r, r, t);

	/* S3 = A21|A22|A23|A12|A13|A14|A15|A16|A17|A18|A19|A20 */
	t[0] = rr[21]; t[1] = rr[22]; t[2] = rr[23];
	for (int32_t i = 3; i < 12; i++)
		t[i] = rr[i + 9];

	_fp384_add(r, r, t);

	/* S4 = 0  |A23|0  |A20|A12|A13|A14|A15|A16|A17|A18|A19 */
	t[0] = 0;
	t[1] = rr[23];
	t[2] = 0;
	t[3] = rr[20];
	for (int32_t i = 4; i < 12; i++)
		t[i] = rr[i + 8];

	_fp384_add(r, r, t);

	/* S5 = 0  |0  |0  |0  |A20|A21|A22|A23|0  |0  |0  |0 */
	t[0] = t[1] = t[2] = t[3] = 0;
	t[4] = rr[20]; t[5] = rr[21]; t[6] = rr[22]; t[7] = rr[23];
	t[8] = t[9] = t[10] = t[11] = 0;

	_fp384_add(r, r, t);

	/* S6 = A20|0  |0  |A21|A22|A23|0  |0  |0  |0  |0  |0 */
	t[0] = rr[20];
	t[1] = t[2] = 0;
	t[3] = rr[21]; t[4] = rr[22]; t[5] = rr[23];
	t[6] = t[7] = t[8] = t[9] = t[10] = t[11] = 0;

	_fp384_add(r, r, t);

	/* D1 = A23|A12|A13|A14|A15|A16|A17|A18|A19|A20|A21|A22 */
	t[0] = rr[23];
	for (int32_t i = 1; i < 12; i++)
		t[i] = rr[i + 11];

	_fp384_sub(r, r, t);

	/* D2 = 0  |A20|A21|A22|A23|0  |0  |0  |0  |0  |0  |0 */
	t[0] = 0;
	t[1] = rr[20]; t[2] = rr[21]; t[3] = rr[22]; t[4] = rr[23];
	t[5] = t[6] = t[7] = t[8] = t[9] = t[10] = t[11] = 0;

	_fp384_sub(r, r, t);

	/* D3 = 0  |0  |0  |A23|A23|0  |0  |0  |0  |0  |0  |0 */
	t[0] = t[1] = t[2] = 0;
	t[3] = t[4] = rr[23];
	t[5] = t[6] = t[7] = t[8] = t[9] = t[10] = t[11] = 0;

	_fp384_sub(r, r, t);

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
	uint64_t t12 = t11 >> 32;
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
	uint64_t t13 = t12 >> 32;
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

	uint32_t T[12];

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

	T[0] = T[1] = T[2] = T[3] = 0;
	T[4] = t21 & 0xffffffff;
	T[5] = t22 & 0xffffffff;
	T[6] = t23 & 0xffffffff;
	T[7] = T[8] = T[9] = T[10] = T[11] = 0;

	_fp384_add(T, T, T);
	_fp384_add(r, r, T);

	T[0] = t12 & 0xffffffff;
	T[1] = t13 & 0xffffffff;
	T[2] = t14 & 0xffffffff;
	T[3] = t15 & 0xffffffff;
	T[4] = t16 & 0xffffffff;
	T[5] = t17 & 0xffffffff;
	T[6] = t18 & 0xffffffff;
	T[7] = t19 & 0xffffffff;
	T[8] = t20 & 0xffffffff;
	T[9] = t21 & 0xffffffff;
	T[10] = t22 & 0xffffffff;
	T[11] = t23 & 0xffffffff;

	_fp384_add(r, r, T);

	T[0] = t21 & 0xffffffff;
	T[1] = t22 & 0xffffffff;
	T[2] = t23 & 0xffffffff;
	T[3] = t12 & 0xffffffff;
	T[4] = t13 & 0xffffffff;
	T[5] = t14 & 0xffffffff;
	T[6] = t15 & 0xffffffff;
	T[7] = t16 & 0xffffffff;
	T[8] = t17 & 0xffffffff;
	T[9] = t18 & 0xffffffff;
	T[10] = t19 & 0xffffffff;
	T[11] = t20 & 0xffffffff;

	_fp384_add(r, r, T);

	T[0] = 0;
	T[1] = t23 & 0xffffffff;
	T[2] = 0;
	T[3] = t20 & 0xffffffff;
	T[4] = t12 & 0xffffffff;
	T[5] = t13 & 0xffffffff;
	T[6] = t14 & 0xffffffff;
	T[7] = t15 & 0xffffffff;
	T[8] = t16 & 0xffffffff;
	T[9] = t17 & 0xffffffff;
	T[10] = t18 & 0xffffffff;
	T[11] = t19 & 0xffffffff;

	_fp384_add(r, r, T);

	T[0] = T[1] = T[2] = T[3] = 0;
	T[4] = t20 & 0xffffffff;
	T[5] = t21 & 0xffffffff;
	T[6] = t22 & 0xffffffff;
	T[7] = t23 & 0xffffffff;
	T[8] = T[9] = T[10] = T[11] = 0;

	_fp384_add(r, r, T);

	T[0] = t20 & 0xffffffff;
	T[1] = T[2] = 0;
	T[3] = t21 & 0xffffffff;
	T[4] = t22 & 0xffffffff;
	T[5] = t23 & 0xffffffff;
	T[6] = T[7] = T[8] = T[9] = T[10] = T[11] = 0;

	_fp384_add(r, r, T);

	T[0] = t23 & 0xffffffff;
	T[1] = t12 & 0xffffffff;
	T[2] = t13 & 0xffffffff;
	T[3] = t14 & 0xffffffff;
	T[4] = t15 & 0xffffffff;
	T[5] = t16 & 0xffffffff;
	T[6] = t17 & 0xffffffff;
	T[7] = t18 & 0xffffffff;
	T[8] = t19 & 0xffffffff;
	T[9] = t20 & 0xffffffff;
	T[10] = t21 & 0xffffffff;
	T[11] = t22 & 0xffffffff;

	_fp384_sub(r, r, T);

	T[0] = 0;
	T[1] = t20 & 0xffffffff;
	T[2] = t21 & 0xffffffff;
	T[3] = t22 & 0xffffffff;
	T[4] = t23 & 0xffffffff;
	T[5] = T[6] = T[7] = T[8] = T[9] = T[10] = T[11] = 0;

	_fp384_sub(r, r, T);

	T[0] = T[1] = T[2] = 0;
	T[3] = T[4] = t23 & 0xffffffff;
	T[5] = T[6] = T[7] = T[8] = T[9] = T[10] = T[11] = 0;

	_fp384_sub(r, r, T);

#endif
}

/* @func: _fp384_mod (static)
 * #desc:
 *    prime field modular reduction (norm).
 *
 * #1: r [in/out] number
 */
static void _fp384_mod(uint32_t r[12])
{
	uint32_t carry = 0, carry2 = 0;
	uint64_t tmp = 0;

	/* if r < p : adjust carry2 */
	for (int32_t i = 0; i < 12; i++) {
		tmp = (uint64_t)r[i] + _fp384_p_R[i] + carry2;
		carry2 = tmp >> 32;
	}

	/* r = r % p modular reduction */
	carry2 = ~(((uint64_t)carry2 - 1) >> 32);
	for (int32_t i = 0; i < 12; i++) {
		tmp = (uint64_t)r[i] - (_fp384_p[i] & carry2) + (int32_t)carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}
}

/* @func: _fp384_inv (static)
 * #desc:
 *    modular inversion based on fermat's little theorem.
 *
 * #1: r [out] inverse modulus
 * #2: z [in]  number
 */
static void _fp384_inv(uint32_t r[12], const uint32_t z[12])
{
	uint32_t x[12], e[12];
	for (int32_t i = 0; i < 12; i++) {
		x[i] = z[i];
		e[i] = _fp384_p[i];
		r[i] = 0;
	}
	e[0] -= 2;
	r[0] = 1;

	for (int32_t i = 0; i < 384; i++) {
		uint32_t k_i = e[i / 32] >> (i % 32);
		if (k_i & 1)
			_fp384_mul(r, r, x);
		_fp384_mul(x, x, x);
	}
}

/* @func: _sc384_modw (static)
 * #desc:
 *    scalar high-word modular reduction fold.
 *
 * #1: r [out] result
 * #2: a [in]  number
 * #3: b [in]  high-word
 */
static void _sc384_modw(uint32_t r[12],
		const uint32_t a[12], uint32_t b)
{
#if 0

	uint32_t rr[12];
	uint32_t carry = 0, carry2 = 0;
	uint64_t tmp = 0;

	/* rr = b * R */
	for (int32_t i = 0; i < 12; i++) {
		tmp = (uint64_t)b * _sc384_R[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	/* r = a + rr */
	for (int32_t i = 0; i < 12; i++) {
		tmp = (uint64_t)a[i] + rr[i] + carry2;
		r[i] = tmp & 0xffffffff;
		carry2 = tmp >> 32;
	}

	/* rr = carry2 * R */
	carry = 0;
	for (int32_t i = 0; i < 12; i++) {
		tmp = (uint64_t)carry2 * _sc384_R[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	/* r = r + rr */
	carry = 0;
	for (int32_t i = 0; i < 12; i++) {
		tmp = (uint64_t)r[i] + rr[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

#else

	uint64_t tt0 = (uint64_t)b * _sc384_R[0];
	uint64_t tt1 = (uint64_t)b * _sc384_R[1];
	uint64_t tt2 = (uint64_t)b * _sc384_R[2];
	uint64_t tt3 = (uint64_t)b * _sc384_R[3];
	uint64_t tt4 = (uint64_t)b * _sc384_R[4];
	uint64_t tt5 = (uint64_t)b * _sc384_R[5];
	uint64_t tt6 = (uint64_t)b * _sc384_R[6];
	uint64_t tt7 = (uint64_t)b * _sc384_R[7];
	uint64_t tt8 = (uint64_t)b * _sc384_R[8];
	uint64_t tt9 = (uint64_t)b * _sc384_R[9];
	uint64_t tt10 = (uint64_t)b * _sc384_R[10];
	uint64_t tt11 = (uint64_t)b * _sc384_R[11];
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
	uint32_t carry = t11 >> 32;
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

	tt0 = (uint64_t)carry * _sc384_R[0];
	tt1 = (uint64_t)carry * _sc384_R[1];
	tt2 = (uint64_t)carry * _sc384_R[2];
	tt3 = (uint64_t)carry * _sc384_R[3];
	tt4 = (uint64_t)carry * _sc384_R[4];
	tt5 = (uint64_t)carry * _sc384_R[5];
	tt6 = (uint64_t)carry * _sc384_R[6];
	tt7 = (uint64_t)carry * _sc384_R[7];
	tt8 = (uint64_t)carry * _sc384_R[8];
	tt9 = (uint64_t)carry * _sc384_R[9];
	tt10 = (uint64_t)carry * _sc384_R[10];
	tt11 = (uint64_t)carry * _sc384_R[11];
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

#endif
}

/* @func: _sc384_mod (static)
 * #desc:
 *    scalar modular reduction (norm).
 *
 * #1: r [in/out] number
 */
static void _sc384_mod(uint32_t r[12])
{
	uint32_t carry = 0, carry2 = 0;
	uint64_t tmp = 0;

	/* r = r - q */
	for (int32_t i = 0; i < 12; i++) {
		tmp = (uint64_t)r[i] - _sc384_q[i] + (int32_t)carry2;
		r[i] = tmp & 0xffffffff;
		carry2 = tmp >> 32;
	}

	/* r = if r < q : adjust r */
	carry2 = ~(((uint64_t)carry2 - 1) >> 32);
	for (int32_t i = 0; i < 12; i++) {
		tmp = (uint64_t)r[i] + (_sc384_q[i] & carry2) + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}
}

/* @func: _sc384_add (static)
 * #desc:
 *    scalar addition.
 *
 * #1: r [out] sum
 * #2: a [in]  addend
 * #3: b [in]  addend
 */
static void _sc384_add(uint32_t r[12],
		const uint32_t a[12], const uint32_t b[12])
{
	uint32_t carry = 0;
	uint64_t tmp = 0;

	/* r = a + b */
	for (int32_t i = 0; i < 12; i++) {
		tmp = (uint64_t)a[i] + b[i] + carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	/* r = r % q */
	_sc384_modw(r, r, carry);
}

/* @func: _sc384_mul (static)
 * #desc:
 *    scalar multiplication.
 *
 * #1: r [out] product
 * #2: a [in]  multiplicand
 * #3: b [in]  multiplier
 */
static void _sc384_mul(uint32_t r[12],
		const uint32_t a[12], const uint32_t b[12])
{
#if 0

	uint32_t rr[24];
	uint32_t carry = 0;
	uint64_t tmp = 0;

	for (int32_t i = 0; i < 24; i++)
		rr[i] = 0;

	/* rr = a * b */
	for (int32_t i = 0; i < 12; i++) {
		tmp = (uint64_t)a[0] * b[i] + carry;
		rr[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}
	rr[12] = carry;

	for (int32_t i = 1; i < 12; i++) {
		carry = 0;
		for (int32_t j = 0; j < 12; j++) {
			tmp = (uint64_t)a[i] * b[j] + rr[i + j] + carry;
			rr[i + j] = tmp & 0xffffffff;
			carry = tmp >> 32;
		}
		rr[i + 12] = carry;
	}

	/* r = rr % q */
	_sc384_modw(rr + 11, rr + 11, rr[23]);
	_sc384_modw(rr + 10, rr + 10, rr[22]);
	_sc384_modw(rr + 9, rr + 9, rr[21]);
	_sc384_modw(rr + 8, rr + 8, rr[20]);
	_sc384_modw(rr + 7, rr + 7, rr[19]);
	_sc384_modw(rr + 6, rr + 6, rr[18]);
	_sc384_modw(rr + 5, rr + 5, rr[17]);
	_sc384_modw(rr + 4, rr + 4, rr[16]);
	_sc384_modw(rr + 3, rr + 3, rr[15]);
	_sc384_modw(rr + 2, rr + 2, rr[14]);
	_sc384_modw(rr + 1, rr + 1, rr[13]);
	_sc384_modw(r, rr, rr[12]);

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
	uint64_t t12 = t11 >> 32;
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
	uint64_t t13 = t12 >> 32;
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

	uint32_t T[24];

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

	_sc384_modw(T + 11, T + 11, T[23]);
	_sc384_modw(T + 10, T + 10, T[22]);
	_sc384_modw(T + 9, T + 9, T[21]);
	_sc384_modw(T + 8, T + 8, T[20]);
	_sc384_modw(T + 7, T + 7, T[19]);
	_sc384_modw(T + 6, T + 6, T[18]);
	_sc384_modw(T + 5, T + 5, T[17]);
	_sc384_modw(T + 4, T + 4, T[16]);
	_sc384_modw(T + 3, T + 3, T[15]);
	_sc384_modw(T + 2, T + 2, T[14]);
	_sc384_modw(T + 1, T + 1, T[13]);
	_sc384_modw(r, T, T[12]);

#endif
}

/* @func: _sc384_inv (static)
 * #desc:
 *    modular inversion based on fermat's little theorem.
 *
 * #1: r [out] inverse modulus
 * #2: z [in]  number
 */
static void _sc384_inv(uint32_t r[12], const uint32_t z[12])
{
	uint32_t x[12], e[12];
	for (int32_t i = 0; i < 12; i++) {
		x[i] = z[i];
		e[i] = _sc384_q[i];
		r[i] = 0;
	}
	e[0] -= 2;
	r[0] = 1;

	for (int32_t i = 0; i < 384; i++) {
		uint32_t k_i = e[i / 32] >> (i % 32);
		if (k_i & 1)
			_sc384_mul(r, r, x);
		_sc384_mul(x, x, x);
	}
}

/* @func: _sc384_digest (static)
 * #desc:
 *    sha384 digest modular reduction.
 *
 * #1: dig [in]  sha384 digest
 * #2: r   [out] result
 */
static void _sc384_digest(const uint8_t dig[48], uint32_t r[12])
{
	/* big endian */
	for (int32_t i = 0; i < 48; i++)
		((uint8_t *)r)[47 - i] = dig[i];

	/* conch_memcpy(r, dig, 48); */

	/* r = r % q */
	_sc384_mod(r);
}

/* @func: _p384_sub (static)
 * #desc:
 *    nonprime field subtraction.
 *
 * #1: r [out] difference
 * #2: a [in]  minuend
 * #3: b [in]  subtract
 * #r:   [ret] overflow
 */
static uint32_t _p384_sub(uint32_t r[12],
		const uint32_t a[12], const uint32_t b[12])
{
	uint32_t carry = 0;
	uint64_t tmp = 0;

	/* r = a - b */
	for (int32_t i = 0; i < 12; i++) {
		tmp = (uint64_t)a[i] - b[i] + (int32_t)carry;
		r[i] = tmp & 0xffffffff;
		carry = tmp >> 32;
	}

	return carry;
}

/* @func: _p384_point_add (static)
 * #desc:
 *    jacobian-affine curve point addition.
 *
 * #1: xyz1 [in]  addend
 * #2: xyz2 [in]  addend
 * #3: xyz3 [out] result
 */
static void _p384_point_add(const struct p384_point *xyz1,
		const struct p384_point *xyz2, struct p384_point *xyz3)
{
	uint32_t H1[12], R1[12], H2[12], H3[12], C1[12], T1[12], T2[12];
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
	_fp384_mul(T1, xyz1->z, xyz1->z);
	_fp384_mul(H1, T1, xyz2->x);
	_fp384_sub(H1, H1, xyz1->x);

	/* R1 = (y2 * (z1 ^ 3)) - y1 */
	_fp384_mul(R1, T1, xyz1->z);
	_fp384_mul(R1, R1, xyz2->y);
	_fp384_sub(R1, R1, xyz1->y);

	/* H2 = H1 ^ 2 */
	_fp384_mul(H2, H1, H1);
	/* H3 = H1 ^ 3 */
	_fp384_mul(H3, H2, H1);
	/* C1 = x1 * H2 */
	_fp384_mul(C1, xyz1->x, H2);

	/* x3 = (R1 ^ 2) - (C1 * 2) - H3 */
	_fp384_mul(T1, R1, R1);
	_fp384_sub(T1, T1, C1);
	_fp384_sub(T1, T1, C1);
	_fp384_sub(xyz3->x, T1, H3);

	/* y3 = (R1 * (C1 - x3)) - (y1 * H3) */
	_fp384_sub(T1, C1, xyz3->x);
	_fp384_mul(T1, T1, R1);
	_fp384_mul(T2, xyz1->y, H3);
	_fp384_sub(xyz3->y, T1, T2);

	/* z3 = z1 * H1 */
	_fp384_mul(xyz3->z, xyz1->z, H1);
}

/* @func: _p384_point_double (static)
 * #desc:
 *    jacobian curve point doubling.
 *
 * #1: xyz1 [in]  addend
 * #2: xyz3 [out] result
 */
static void _p384_point_double(const struct p384_point *xyz1,
		struct p384_point *xyz3)
{
	uint32_t M1[12], S1[12], U1[12], T1[12];
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
	_fp384_mul(T1, xyz1->x, xyz1->x);
	_fp384_add(M1, T1, T1);
	_fp384_add(M1, M1, T1);
	_fp384_mul(T1, xyz1->z, xyz1->z);
	_fp384_mul(T1, T1, T1);
	_fp384_mul(T1, T1, _p384_a);
	_fp384_add(M1, M1, T1);

	/* S1 = (x1 * 4) * (y1 ^ 2) */
	_fp384_add(S1, xyz1->x, xyz1->x);
	_fp384_add(S1, S1, S1);
	_fp384_mul(T1, xyz1->y, xyz1->y);
	_fp384_mul(S1, S1, T1);

	/* U1 = (y1 ^ 4) * 8 */
	_fp384_mul(T1, T1, T1);
	_fp384_add(T1, T1, T1);
	_fp384_add(T1, T1, T1);
	_fp384_add(U1, T1, T1);

	/* z3 = (y1 * z1) * 2 */
	_fp384_mul(T1, xyz1->y, xyz1->z);
	_fp384_add(xyz3->z, T1, T1);

	/* x3 = (M1 ^ 2) - (S1 * 2) */
	_fp384_mul(T1, M1, M1);
	_fp384_sub(T1, T1, S1);
	_fp384_sub(xyz3->x, T1, S1);

	/* y3 = (M1 * (S1 - x3)) - U1 */
	_fp384_sub(T1, S1, xyz3->x);
	_fp384_mul(T1, T1, M1);
	_fp384_sub(xyz3->y, T1, U1);
}

/* @func: _p384_point_to_affine (static)
 * #desc:
 *    jacobian to affine coordinates conversion.
 *
 * #1: xyz1 [in]  jacobian point
 * #2: xy2  [out] affine point
 */
static void _p384_point_to_affine(const struct p384_point *xyz1,
		struct p384_point *xy2)
{
	uint32_t z[12], t[12];

	/* z = inv(z1) */
	_fp384_inv(z, xyz1->z);

	/* x2 = (x1 * (z ^ 2)) % p */
	_fp384_mul(t, z, z);
	_fp384_mul(xy2->x, xyz1->x, t);
	_fp384_mod(xy2->x);

	/* y2 = (y1 * (z ^ 3)) % p */
	_fp384_mul(t, t, z);
	_fp384_mul(xy2->y, xyz1->y, t);
	_fp384_mod(xy2->y);
}

/* @func: _p384_scalar_mul (static)
 * #desc:
 *    p384 signed binary ladder scalar multiplication.
 *
 * #1: k   [in]  input point
 * #2: xy1 [in]  base point
 * #3: xy2 [out] scalar
 */
static void _p384_scalar_mul(const uint32_t k[12],
		const struct p384_point *xy1, struct p384_point *xy2)
{
	struct p384_point _xyz1, _xyz2;
	uint32_t t[12], y_neg[12], neg = 0;
	for (int32_t i = 0; i < 12; i++) {
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
	_p384_sub(t, _sc384_q, k);
	neg = ~k[0] & 1;
	_fp384_move(t, k, ~neg);

	/* y_neg = -y1 */
	_fp384_sub(y_neg, y_neg, xy1->y);
	_fp384_move(_xyz1.y, y_neg, neg);

	/* signed binary ladder and jacobian-affine coordinates */
	for (int32_t i = 383; i > 0; i--) {
		uint32_t k_i = t[i / 32] >> (i % 32);
		_fp384_move(_xyz2.y, xy1->y, k_i ^ neg);
		_fp384_move(_xyz2.y, y_neg, ~k_i ^ neg);
		/* R = (R * 2) +- P */

		_p384_point_double(&_xyz1, &_xyz1);
		_p384_point_add(&_xyz1, &_xyz2, &_xyz1);
	}

	_p384_point_to_affine(&_xyz1, xy2);
}

/* @func: _p384_check_key (static)
 * #desc:
 *    input point legality check.
 *
 * #1: k [in]  input point
 * #r:   [ret] 0: no error, -1: error
 */
static int32_t _p384_check_key(const uint32_t k[12])
{
	uint32_t t[12];

	/* if k > 0 && k < q */
	if (_fp384_iszero(k) || !_p384_sub(t, k, _sc384_q))
		return -1;

	return 0;
}

/* @func: _p384_check_point (static)
 * #desc:
 *    curve point legality check.
 *
 * #1: xy1 [in]  curve point
 * #r:     [ret] 0: no error, -1: error
 */
static int32_t _p384_check_point(const struct p384_point *xy1)
{
	uint32_t t1[12], t2[12];

	if (!_p384_sub(t1, xy1->x, _fp384_p))
		return -1;
	if (!_p384_sub(t1, xy1->y, _fp384_p))
		return -1;

	/* t1 = ((x1 ^ 3) - (x1 * 3) + b) % p */
	_fp384_mul(t1, xy1->x, xy1->x);
	_fp384_mul(t1, t1, xy1->x);
	_fp384_sub(t1, t1, xy1->x);
	_fp384_sub(t1, t1, xy1->x);
	_fp384_sub(t1, t1, xy1->x);
	_fp384_add(t1, t1, _p384_b);
	_fp384_mod(t1);

	/* t2 = (y1 ^ 2) % p */
	_fp384_mul(t2, xy1->y, xy1->y);
	_fp384_mod(t2);

	_p384_sub(t1, t1, t2);

	return _fp384_iszero(t1) - 1;
}

/* @func: _p384_affine_add (static)
 * #desc:
 *    affine curve point addition.
 *
 * #1: xy1 [in]  addend
 * #2: xy2 [in]  addend
 * #3: xy3 [out] result
 */
static void _p384_affine_add(const struct p384_point *xy1,
		const struct p384_point *xy2, struct p384_point *xy3)
{
	struct p384_point _xyz1, _xyz2, _xyz3, _xy4;
	uint32_t t[12], iswap1, iswap2;
	for (int32_t i = 0; i < 12; i++) {
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

	_p384_sub(t, xy1->x, xy2->x);
	iswap1 = _fp384_iszero(t);
	_p384_sub(t, xy1->y, xy2->y);
	iswap2 = _fp384_iszero(t);

	/* P != +- Q */
	_p384_point_add(&_xyz1, &_xyz2, &_xyz1);
	/* P == Q */
	_p384_point_double(&_xyz1, &_xyz3);

	/* xyz1 = if x1 == x2 && y1 == y2 : swap xyz3 */
	_fp384_swap(_xyz1.x, _xyz3.x, iswap1 & iswap2);
	_fp384_swap(_xyz1.y, _xyz3.y, iswap1 & iswap2);
	_fp384_swap(_xyz1.z, _xyz3.z, iswap1 & iswap2);

	_p384_point_to_affine(&_xyz1, xy3);

	/* xy3 = if x1 == x2 && y1 != y2 : adjust zero */
	_fp384_swap(xy3->x, _xy4.x, iswap1 & ~iswap2);
	_fp384_swap(xy3->y, _xy4.y, iswap1 & ~iswap2);
}

/* @func: conch_ecdh_p384_public_key
 * #desc:
 *    p384 ecdh public key create function.
 *
 * #1: pri [in]  private key
 * #2: pub [out] public key
 * #r:     [ret] 0: success, -1: fail
 */
int32_t conch_ecdh_p384_public_key(const uint8_t *pri, uint8_t *pub)
{
	uint32_t _pri[12];
	struct p384_point xy1;

	conch_memcpy(_pri, pri, ECDH_P384_PRI_LEN);
	if (_p384_check_key(_pri))
		return -1;

	/* xy1 = scalar(_pri, base) */
	_p384_scalar_mul(_pri, &_p384_base, &xy1);

	conch_memcpy(pub, xy1.x, ECDH_P384_LEN);
	conch_memcpy(pub + ECDH_P384_LEN, xy1.y, ECDH_P384_LEN);

	return 0;
}

/* @func: conch_ecdh_p384_shared_key
 * #desc:
 *    p384 ecdh shared key create function.
 *
 * #1: pri [in]  private key
 * #2: pub [in]  public key
 * #3: key [in]  shared key
 * #r:     [ret] 0: success, -1: fail
 */
int32_t conch_ecdh_p384_shared_key(const uint8_t *pri,
		const uint8_t *pub, uint8_t *key)
{
	uint32_t _pri[12];
	struct p384_point xy1, xy2;

	conch_memcpy(_pri, pri, ECDH_P384_PRI_LEN);
	if (_p384_check_key(_pri))
		return -1;

	conch_memcpy(xy1.x, pub, ECDH_P384_LEN);
	conch_memcpy(xy1.y, pub + ECDH_P384_LEN, ECDH_P384_LEN);
	if (_p384_check_point(&xy1))
		return -1;

	/* xy2 = scalar(_pri, xy1) */
	_p384_scalar_mul(_pri, &xy1, &xy2);

	conch_memcpy(key, xy2.x, ECDH_P384_KEY_LEN);

	return 0 - _fp384_iszero(xy2.x);
}

/* @func: conch_ecdsa_p384_public_key
 * #desc:
 *    p384 ecdsa public key create function.
 *
 * #1: pri [in]  private key
 * #2: pub [out] public key
 * #r:     [ret] 0: success, -1: fail
 */
int32_t conch_ecdsa_p384_public_key(const uint8_t *pri, uint8_t *pub)
{
	uint32_t _pri[12];
	struct p384_point xy1;

	conch_memcpy(_pri, pri, ECDSA_P384_PRI_LEN);
	if (_p384_check_key(_pri))
		return -1;

	/* xy1 = scalar(_pri, base) */
	_p384_scalar_mul(_pri, &_p384_base, &xy1);

	conch_memcpy(pub, xy1.x, ECDSA_P384_LEN);
	conch_memcpy(pub + ECDSA_P384_LEN, xy1.y, ECDSA_P384_LEN);

	return 0;
}

/* @func: conch_ecdsa_p384_sign
 * #desc:
 *    p384 ecdsa signature function.
 *
 * #1: pri  [in]  private key
 * #2: ran  [in]  nonce
 * #3: msg  [in]  input message
 * #4: len  [in]  message length
 * #5: sign [out] signature
 * #r:      [ret] 0: success, -1: fail
 */
int32_t conch_ecdsa_p384_sign(const uint8_t *pri, const uint8_t *ran,
		const uint8_t *msg, uint32_t len, uint8_t *sign)
{
	uint32_t _pri[12], _ran[12], z[12], s[12];
	struct p384_point xy1;
	SHA384_NEW(ctx);

	conch_memcpy(_pri, pri, ECDSA_P384_PRI_LEN);
	conch_memcpy(_ran, ran, ECDSA_P384_RAN_LEN);
	if (_p384_check_key(_pri) || _p384_check_key(_ran))
		return -1;

	/* x1 = scalar(_ran, base) */
	_p384_scalar_mul(_ran, &_p384_base, &xy1);
	_sc384_mod(xy1.x);

	if (_fp384_iszero(xy1.x))
		return -1;

	/* z = sha(msg) % q */
	conch_sha384_init(&ctx);
	conch_sha512(&ctx, msg, len);
	_sc384_digest(&(SHA384_STATE(&ctx, 0)), z);

	/* s = (x1 * _pri) + z */
	_sc384_mul(s, xy1.x, _pri);
	_sc384_add(s, s, z);

	/* s = (s * inv(_ran)) % q */
	_sc384_inv(z, _ran);
	_sc384_mul(s, s, z);
	_sc384_mod(s);

	if (_fp384_iszero(s))
		return -1;

	conch_memcpy(sign, xy1.x, ECDSA_P384_LEN);
	conch_memcpy(sign + ECDSA_P384_LEN, s, ECDSA_P384_LEN);

	return 0;
}

/* @func: conch_ecdsa_p384_verify
 * #desc:
 *    p384 ecdsa signature verification function.
 *
 * #1: pub  [in]  public key
 * #2: sign [in]  signature
 * #3: msg  [in]  input message
 * #4: len  [in]  message length
 * #r:      [ret] 0: success, -1: fail
 */
int32_t conch_ecdsa_p384_verify(const uint8_t *pub,
		const uint8_t *sign, const uint8_t *msg, uint32_t len)
{
	uint32_t z[12], t[12], u1[12], u2[12];
	struct p384_point xy1, xy2, xy3;
	SHA384_NEW(ctx);

	/* xy1 = sign */
	conch_memcpy(xy1.x, sign, ECDSA_P384_LEN);
	conch_memcpy(xy1.y, sign + ECDSA_P384_LEN, ECDSA_P384_LEN);
	if (_p384_check_key(xy1.x) || _p384_check_key(xy1.y))
		return -1;

	/* xy2 = pub */
	conch_memcpy(xy2.x, pub, ECDSA_P384_LEN);
	conch_memcpy(xy2.y, pub + ECDSA_P384_LEN, ECDSA_P384_LEN);
	if (_p384_check_point(&xy2))
		return -1;

	/* z = sha(msg) % q */
	conch_sha384_init(&ctx);
	conch_sha512(&ctx, msg, len);
	_sc384_digest(&(SHA384_STATE(&ctx, 0)), z);

	/* t = inv(y1) */
	_sc384_inv(t, xy1.y);

	/* u1 = (z * t) % q */
	_sc384_mul(u1, z, t);
	_sc384_mod(u1);

	/* u2 = (x1 * t) % q */
	_sc384_mul(u2, xy1.x, t);
	_sc384_mod(u2);

	/* xy2 = scalar(u2, xy2) */
	_p384_scalar_mul(u2, &xy2, &xy2);
	/* xy3 = scalar(u1, base) */
	_p384_scalar_mul(u1, &_p384_base, &xy3);

	/* x2 = add(xy3, xy2) % q */
	_p384_affine_add(&xy3, &xy2, &xy2);
	_sc384_mod(xy2.x);

	_p384_sub(t, xy1.x, xy2.x);

	return _fp384_iszero(t) - 1;
}
