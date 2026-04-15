/* @file: csoft.c
 * #desc:
 *    The implementations of software mathematical operations in based
 *    on C.  only depends on bitwise operations, addition, subtraction,
 *    and multiplication (division is optional).
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
#include <conch/csoft.h>


/* TODO: The implementation of software floating-point is not yet complete */


/* @func: conch_csf_clz32
 * #desc:
 *    count leading zeros (32-bit).
 *
 * #1: x [in]  unsigned integer
 * #r:   [ret] number of leading zero
 */
int32_t conch_csf_clz32(uint32_t x)
{
	if (!x)
		return 32;

	/* clz32()=4: 0b00001...1 */

	int32_t n = 0;
	if (!(x >> 16)) { /* hig-16 */
		n += 16;
		x <<= 16;
	}
	if (!(x >> 24)) { /* hig-8 */
		n += 8;
		x <<= 8;
	}
	if (!(x >> 28)) { /* hig-4 */
		n += 4;
		x <<= 4;
	}
	if (!(x >> 30)) { /* hig-2 */
		n += 2;
		x <<= 2;
	}
	if (!(x >> 31)) /* hig-1 */
		n += 1;

	return n;
}

/* @func: conch_csf_clz64
 * #desc:
 *    count leading zeros (64-bit).
 *
 * #1: x [in]  unsigned integer
 * #r:   [ret] number of leading zero
 */
int32_t conch_csf_clz64(uint64_t x)
{
	if (!x)
		return 64;

	/* clz64()=4: 0b00001..10..0 */

	int32_t n = 0;
	if (!(x >> 32)) { /* hig-32 */
		n += 32;
		x <<= 32;
	}
	if (!(x >> 48)) { /* hig-16 */
		n += 16;
		x <<= 16;
	}
	if (!(x >> 56)) { /* hig-8 */
		n += 8;
		x <<= 8;
	}
	if (!(x >> 60)) { /* hig-4 */
		n += 4;
		x <<= 4;
	}
	if (!(x >> 62)) { /* hig-2 */
		n += 2;
		x <<= 2;
	}
	if (!(x >> 63)) /* hig-1 */
		n += 1;

	return n;
}

/* @func: conch_csf_ctz32
 * #desc:
 *    count trailing zeros (32-bit).
 *
 * #1: x [in]  unsigned integer
 * #r:   [ret] number of trailing zero
 */
int32_t conch_csf_ctz32(uint32_t x)
{
	if (!x)
		return 32;

	/* ctz32()=4: 0b01..10..10000 (32-bit) */

	int32_t n = 0;
	if (!(x & 0xffff)) { /* low-16 */
		n += 16;
		x >>= 16;
	}
	if (!(x & 0xff)) { /* low-8 */
		n += 8;
		x >>= 8;
	}
	if (!(x & 0xf)) { /* low-4 */
		n += 4;
		x >>= 4;
	}
	if (!(x & 0x3)) { /* low-2 */
		n += 2;
		x >>= 2;
	}
	if (!(x & 0x1)) /* low-1 */
		n += 1;

	return n;
}

/* @func: conch_csf_ctz64
 * #desc:
 *    count trailing zeros (64-bit).
 *
 * #1: x [in]  unsigned integer
 * #r:   [ret] number of trailing zero
 */
int32_t conch_csf_ctz64(uint64_t x)
{
	if (!x)
		return 64;

	/* ctz64()=4: 0b01..10..10000 */

	int32_t n = 0;
	if (!(x & 0xffffffff)) { /* low-32 */
		n += 32;
		x >>= 32;
	}
	if (!(x & 0xffff)) { /* low-16 */
		n += 16;
		x >>= 16;
	}
	if (!(x & 0xff)) { /* low-8 */
		n += 8;
		x >>= 8;
	}
	if (!(x & 0xf)) { /* low-4 */
		n += 4;
		x >>= 4;
	}
	if (!(x & 0x3)) { /* low-2 */
		n += 2;
		x >>= 2;
	}
	if (!(x & 0x1)) /* low-1 */
		n += 1;

	return n;
}

/* @func: conch_csf_udivmod32
 * #desc:
 *    unsigned integer division and remainder (32-bit).
 *
 * #1: a   [in]  dividend
 * #2: b   [in]  divisor
 * #3: rem [out] result remainder
 * #r:     [ret] result quotient
 */
uint32_t conch_csf_udivmod32(uint32_t a, uint32_t b, uint32_t *rem)
{
	if (!a || !b) {
		if (rem)
			*rem = 0;
		return 0;
	}
	if (a < b) {
		if (rem)
			*rem = a;
		return 0;
	}

	int32_t sh = conch_csf_clz32(b) - conch_csf_clz32(a);
	b <<= sh;

	uint32_t q = 0;
	for (; sh >= 0; sh--) {
		q <<= 1;
		if (a >= b) {
			a -= b;
			q |= 1;
		}
		b >>= 1;
	}

	if (rem)
		*rem = a;

	return q;
}

/* @func: conch_csf_idivmod32
 * #desc:
 *    signed integer division and remainder (32-bit).
 *
 * #1: a   [in]  dividend
 * #2: b   [in]  divisor
 * #3: rem [out] result remainder
 * #r:     [ret] result quotient
 */
int32_t conch_csf_idivmod32(int32_t a, int32_t b, int32_t *rem)
{
	if (!a || !b) {
		if (rem)
			*rem = 0;
		return 0;
	}

	int32_t q_neg = (a < 0) ^ (b < 0);
	int32_t r_neg = (a < 0);
	uint32_t q, r;

	if (a < 0)
		a = -a;
	if (b < 0)
		b = -b;

	q = conch_csf_udivmod32((uint32_t)a, (uint32_t)b, &r);

	if (q_neg)
		q = -q;
	if (r_neg)
		r = -r;
	if (rem)
		*rem = (int32_t)r;

	return (int32_t)q;
}

/* @func: conch_csf_udivmod64
 * #desc:
 *    unsigned integer division and remainder (64-bit).
 *
 * #1: a   [in]  dividend
 * #2: b   [in]  divisor
 * #3: rem [out] result remainder
 * #r:     [ret] result quotient
 */
uint64_t conch_csf_udivmod64(uint64_t a, uint64_t b, uint64_t *rem)
{
	if (!a || !b) {
		if (rem)
			*rem = 0;
		return 0;
	}
	if (a < b) {
		if (rem)
			*rem = a;
		return 0;
	}

	int64_t sh = conch_csf_clz64(b) - conch_csf_clz64(a);
	b <<= sh;

	uint64_t q = 0;
	for (; sh >= 0; sh--) {
		q <<= 1;
		if (a >= b) {
			a -= b;
			q |= 1;
		}
		b >>= 1;
	}

	if (rem)
		*rem = a;

	return q;
}

/* @func: conch_csf_idivmod64
 * #desc:
 *    signed integer division and remainder (64-bit).
 *
 * #1: a   [in]  dividend
 * #2: b   [in]  divisor
 * #3: rem [out] result remainder
 * #r:     [ret] result quotient
 */
int64_t conch_csf_idivmod64(int64_t a, int64_t b, int64_t *rem)
{
	if (!a || !b) {
		if (rem)
			*rem = 0;
		return 0;
	}

	int32_t q_neg = (a < 0) ^ (b < 0);
	int32_t r_neg = (a < 0);
	uint64_t q, r;

	if (a < 0)
		a = -a;
	if (b < 0)
		b = -b;

	q = conch_csf_udivmod64((uint64_t)a, (uint64_t)b, &r);

	if (q_neg)
		q = -q;
	if (r_neg)
		r = -r;
	if (rem)
		*rem = (int64_t)r;

	return (int64_t)q;
}

/* @func: conch_csf_udiv32
 * #desc:
 *    unsigned integer division (32-bit).
 *
 * #1: a [in]  dividend
 * #2: b [in]  divisor
 * #r:   [ret] result quotient
 */
uint32_t conch_csf_udiv32(uint32_t a, uint32_t b)
{
	return conch_csf_udivmod32(a, b, NULL);
}

/* @func: conch_csf_umod32
 * #desc:
 *    unsigned integer division remainder (32-bit).
 *
 * #1: a [in]  dividend
 * #2: b [in]  divisor
 * #r:   [ret] result remainder
 */
uint32_t conch_csf_umod32(uint32_t a, uint32_t b)
{
	uint32_t r;
	conch_csf_udivmod32(a, b, &r);

	return r;
}

/* @func: conch_csf_idiv32
 * #desc:
 *    signed integer division (32-bit).
 *
 * #1: a [in]  dividend
 * #2: b [in]  divisor
 * #r:   [ret] result quotient
 */
int32_t conch_csf_idiv32(int32_t a, int32_t b)
{
	return conch_csf_idivmod32(a, b, NULL);
}

/* @func: conch_csf_imod32
 * #desc:
 *    signed integer division remainder (32-bit).
 *
 * #1: a [in]  dividend
 * #2: b [in]  divisor
 * #r:   [ret] result remainder
 */
int32_t conch_csf_imod32(int32_t a, int32_t b)
{
	int32_t r;
	conch_csf_idivmod32(a, b, &r);

	return r;
}

/* @func: conch_csf_udiv64
 * #desc:
 *    unsigned integer division (64-bit).
 *
 * #1: a [in]  dividend
 * #2: b [in]  divisor
 * #r:   [ret] result quotient
 */
uint64_t conch_csf_udiv64(uint64_t a, uint64_t b)
{
	return conch_csf_udivmod64(a, b, NULL);
}

/* @func: conch_csf_umod64
 * #desc:
 *    unsigned integer division remainder (64-bit).
 *
 * #1: a [in]  dividend
 * #2: b [in]  divisor
 * #r:   [ret] result remainder
 */
uint64_t conch_csf_umod64(uint64_t a, uint64_t b)
{
	uint64_t r;
	conch_csf_udivmod64(a, b, &r);

	return r;
}

/* @func: conch_csf_idiv64
 * #desc:
 *    signed integer division (64-bit).
 *
 * #1: a [in]  dividend
 * #2: b [in]  divisor
 * #r:   [ret] result quotient
 */
int64_t conch_csf_idiv64(int64_t a, int64_t b)
{
	return conch_csf_idivmod64(a, b, NULL);
}

/* @func: conch_csf_imod64
 * #desc:
 *    signed integer division remainder (64-bit).
 *
 * #1: a [in]  dividend
 * #2: b [in]  divisor
 * #r:   [ret] result remainder
 */
int64_t conch_csf_imod64(int64_t a, int64_t b)
{
	int64_t r;
	conch_csf_idivmod64(a, b, &r);

	return r;
}

/* @func: conch_csf_umul64
 * #desc:
 *    unsigned multiplication (64-bit).
 *
 * #1: a [in]  multiplicand
 * #2: b [in]  multiplier
 * #r:   [ret] product
 */
uint64_t conch_csf_umul64(uint64_t a, uint64_t b)
{
	uint32_t _a[2], _b[2];
	uint64_t r[3];

	_a[0] = a & 0xffffffff;
	_a[1] = a >> 32;
	_b[0] = b & 0xffffffff;
	_b[1] = b >> 32;

	/*
	 * t = a0 * b0
	 * r0 = t & 0xffffffff
	 * c = t >> 32
	 * t = a0 * b1 + c
	 * r1 = t & 0xffffffff
	 * t = a1 * b0 + r1
	 * r1 = t & 0xffffffff
	 */
	r[0] = (uint64_t)_a[0] * _b[0];
	r[1] = (uint64_t)_a[0] * _b[1];
	r[2] = (uint64_t)_a[1] * _b[0];
	r[1] += r[2];
	r[0] += r[1] << 32;

	return r[0];
}

/* @func: conch_csf_smul64
 * #desc:
 *    signed multiplication (64-bit).
 *
 * #1: a [in]  multiplicand
 * #2: b [in]  multiplier
 * #r:   [ret] product
 */
int64_t conch_csf_smul64(int64_t a, int64_t b)
{
	int32_t neg = (a < 0) ^ (b < 0);

	if (a < 0)
		a = -a;
	if (b < 0)
		b = -b;

	a = (int64_t)conch_csf_umul64((uint64_t)a, (uint64_t)b);

	return neg ? -a : a;
}

/* @func: conch_csf_umul64_128
 * #desc:
 *    in-place unsigned multiplication (64-bit => 128-bit).
 *
 * #1: a [in/out] multiplicand (product low)
 * #2: b [in/out] multiplier (product hig)
 */
void conch_csf_umul64_128(uint64_t *a, uint64_t *b)
{
	uint32_t r[4], _a[2], _b[2];
	uint64_t tmp;

	tmp = *a;
	_a[0] = tmp & 0xffffffff;
	_a[1] = tmp >> 32;
	tmp = *b;
	_b[0] = tmp & 0xffffffff;
	_b[1] = tmp >> 32;

	tmp = (uint64_t)_a[0] * _b[0];
	r[0] = tmp & 0xffffffff;
	tmp = (uint64_t)_a[0] * _b[1] + (tmp >> 32);
	r[1] = tmp & 0xffffffff;
	r[2] = tmp >> 32;

	tmp = (uint64_t)_a[1] * _b[0] + r[1];
	r[1] = tmp & 0xffffffff;
	tmp = (uint64_t)_a[1] * _b[1] + r[2] + (tmp >> 32);
	r[2] = tmp & 0xffffffff;
	r[3] = tmp >> 32;

	*a = (uint64_t)r[0] | (uint64_t)r[1] << 32;
	*b = (uint64_t)r[2] | (uint64_t)r[3] << 32;
}
