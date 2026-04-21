/* @file: csoft.c
 * #desc:
 *    The implementations of software mathematical operations in based
 *    on C.  only depends on bitwise operations, addition, subtraction,
 *    and multiplication.
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
 *    unsigned integer modular (32-bit).
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
 *    signed integer modular (32-bit).
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
 *    unsigned integer modular (64-bit).
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
 *    unsigned integer modular (64-bit).
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
 *    signed integer modular (64-bit).
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

/*
 * Floating-point input processing:
 *    NaN op any     =>  NaN
 *    any op NaN     =>  NaN
 *
 *      x +  NaN     =>  NaN
 *    Inf +  Inf     =>  Inf
 *    Inf + -Inf     =>  NaN
 *    Inf +  finite  =>  Inf
 * finite +  finite  =>  Inf
 *
 *    Inf -  Inf     =>  NaN
 *    Inf - -Inf     =>  Inf
 * finite -  Inf     => -Inf
 *
 *      x * NaN      =>  NaN
 *    Inf * 0        =>  NaN (invalid)
 *    Inf * finite   =>  Inf
 *    Inf * Inf      =>  Inf
 *
 *      x / NaN      =>  NaN
 *    NaN / x        =>  NaN
 * finite / 0        =>  Inf
 *      0 / 0        =>  NaN (invalid)
 *    Inf / Inf      =>  NaN (invalid)
 *    Inf / finite   =>  Inf
 * finite / Inf      =>  0
 *
 *   NaN == x        =>  false
 *   NaN <  x        =>  false
 *   NaN >  x        =>  false
 *   NaN != x        =>  true
 *
 *   Inf >   any     =>  true
 *  -Inf <   any     =>  true
 *   Inf ==  Inf     =>  true
 *   Inf >  -Inf     =>  true
 *
 * NOTE:
 *   The software floating-point reference
 *   from: https://www.jhauser.us/arithmetic/index.html
 *
 * IEEE-754:
 *   float 32-bit:
 *     | sign 1-bit | exponent 8-bit | significand 23-bit  |
 *                                   ^
 *                                hidden bit
 *   double 64-bit:
 *     | sign 1-bit | exponent 11-bit | significand 52-bit |
 *                                    ^
 *                                 hidden bit
 * sign:
 *   0 => positive
 *   1 => negative
 *
 * exponent of float:
 *   0-255      => 127 bias, E-e
 *
 *   1-126      => negative exponent
 *   127-254    => positive exponent
 *   0          => subnormal
 *   255        => infinity and NaN (not a number)
 *
 *   E:255 M: 0 => Inf
 *   E:255 M:!0 => NaN
 *
 * exponent of double:
 *   0-2047      => 1023 bias, E-e
 *
 *   1-1022      => negative exponent
 *   1023-2046   => positive exponent
 *   0           => subnormal
 *   2047        => infinity and NaN (not a number)
 *
 *   E:2047 M: 0 => Inf
 *   E:2047 M:!0 => NaN
 *
 * example:
 *   input: 10.25
 *
 *   10             => 1010
 *   0.25*2 => '0.5 => '0
 *   0.50*2 => '1.0 => '1
 *
 *   1010.01 => 1.01001 (<< 3)
 *              ^
 *           hidden bit
 *
 *   binary: 1.01001*(2^3)
 */

/* @func: _float_approx_recip32_1 (static)
 * #desc:
 *    generate reciprocal approximations.
 *
 * #1: a [in]  number
 * #r:   [ret] reciprocal of x
 */
static uint32_t _float_approx_recip32_1(uint32_t a)
{
	static const uint16_t approx_recip_1k0s[16] = {
		0xffc4, 0xf0be, 0xe363, 0xd76f,
		0xccad, 0xc2f0, 0xba16, 0xb201,
		0xaa97, 0xa3c6, 0x9d7a, 0x97a6,
		0x923c, 0x8d32, 0x887e, 0x8417
		};
	static const uint16_t approx_recip_1k1s[16] = {
		0xf0f1, 0xd62c, 0xbfa1, 0xac77,
		0x9c0a, 0x8ddb, 0x8185, 0x76ba,
		0x6d3b, 0x64d4, 0x5d5c, 0x56b1,
		0x50b6, 0x4b55, 0x4679, 0x4211
		};

	uint16_t index, eps, r0;
	uint32_t m0, m1, r;

	index = (a >> 27) & 0xf;
	eps = (a >> 11) & 0xffff;
	r0 = approx_recip_1k0s[index];
	r0 -= (approx_recip_1k1s[index] * (uint32_t)eps) >> 20;
	r = (uint32_t)r0 << 16;

	m0 = ~(uint32_t)(((uint64_t)r0 * a) >> 7);
	r += ((uint64_t)r0 * m0) >> 24;

	m1 = ((uint64_t)m0 * m0) >> 32;
	r += ((uint64_t)r * m1) >> 48;

	return r;
}

/* @func: _float_pack64 (static)
 * #desc:
 *    floating-point binary package (64-bit).
 *
 * #1: sign [in]  sign
 * #2: exp  [in]  exponent
 * #3: sig  [in]  significand (with hidden bit or exponent bit)
 * #r:      [ret] floating-point
 */
static uint64_t _float_pack64(int32_t sign, int32_t exp, uint64_t sig)
{
	sig += (uint64_t)sign << 63;
	sig += (uint64_t)exp << 52;

	return sig;
}

/* @func: _float_rshift_jam64 (static)
 * #desc:
 *    right shift and add sticky to the low bit (64-bit).
 *
 * #1: a [in]  number
 * #2: b [in]  shift distance
 * #r:   [ret] value after right shift
 */
static uint64_t _float_rshift_jam64(uint64_t a, int32_t b)
{
	if (b < 63) { /* shift + sticky */
		a = (a >> b) | !!(a << (64 - b));
	} else {
		a = !!a; /* sticky */
	}

	return a;
}

/* @func: _float_round_pack64 (static)
 * #desc:
 *    rounding the grs bits and returns the floating-point (64-bit).
 *
 * #1: sign [in]  sign
 * #2: exp  [in]  exponent
 * #3: sig  [in]  significand (with grs bits)
 * #r:      [ret] floating-point
 */
static uint64_t _float_round_pack64(int32_t sign, int32_t exp, uint64_t sig)
{
	int32_t round_bits = sig & 0x3ff; /* guard + round + sticky */
	/*
	 * |G R S              carry|
	 * +========================+
	 * |0 x x | < 0.5     | no  |
	 * |1 0 0 | = 0.5 tie | lsb |
	 * |1 0 1 | > 0.5     | yes |
	 * |1 1 x | > 0.5     | yes |
	 */

	if (exp < 0) { /* subnormal */
		sig = _float_rshift_jam64(sig, -exp);
		round_bits = sig & 0x3ff;
		exp = 0;
	} else if ((exp > (1023 + 1022)) || ((exp == (1023 + 1022))
			&& ((sig + 0x200) >= 0x8000000000000000ULL))) {
		return _float_pack64(sign, 0x7ff, 0); /* inf */
	}

	sig = (sig + 0x200) >> 10; /* rounding */
	if (round_bits == 0x200) /* round-to-even */
		sig &= ~1ULL;

	if (!sig) /* zero */
		exp = 0;

	return _float_pack64(sign, exp, sig);
}

/* @func: _float_norm_round_pack64 (static)
 * #desc:
 *    normalization and rounding, then return a floating-point (64-bit).
 *
 * #1: sign [in]  sign
 * #2: exp  [in]  exponent
 * #3: sig  [in]  significand (with grs bits)
 * #r:      [ret] floating-point
 */
static uint64_t _float_norm_round_pack64(int32_t sign, int32_t exp,
		uint64_t sig)
{
	int32_t sh = conch_csf_clz64(sig) - 1;
	exp -= sh;

	if (sh >= 10 && exp < (1023 + 1022))
		return _float_pack64(sign, sig ? exp : 0, sig << (sh - 10));

	return _float_round_pack64(sign, exp, sig << sh);
}

/* @func: _float_add_mags64 (static)
 * #desc:
 *    floating-point magnitudes addition (64-bit).
 *
 * #1: a      [in]  addend
 * #2: b      [in]  addend
 * #3: z_sign [in]  sign
 * #r:        [ret] sum
 */
static uint64_t _float_add_mags64(uint64_t a, uint64_t b, int32_t z_sign)
{
	int32_t a_exp, b_exp, z_exp, sh;
	uint64_t a_sig, b_sig, z_sig;

	a_exp = (a >> 52) & 0x7ff;
	a_sig = a & 0xfffffffffffffULL;
	b_exp = (b >> 52) & 0x7ff;
	b_sig = b & 0xfffffffffffffULL;
	/*
	 *   x + NaN => NaN
	 * Inf + Inf => Inf
	 */

	sh = a_exp - b_exp;
	if (!sh) { /* a == b */
		if (a_exp == 0x7ff) { /* inf, nan */
			if (b_exp == 0x7ff && b_sig)
				return b;
			return a;
		}

		z_exp = a_exp;
		if (!z_exp) /* subnormal */
			return a + b_sig;

		/* hidden bit of a and b */
		z_sig = 0x20000000000000ULL + a_sig + b_sig;
		z_sig <<= 9; /* grs */
		return _float_round_pack64(z_sign, z_exp, z_sig);
	}

	a_sig <<= 9; /* grs */
	b_sig <<= 9;
	if (sh < 0) { /* a < b */
		if (b_exp == 0x7ff) { /* inf, nan */
			if (b_sig)
				return a;
			return _float_pack64(z_sign, 0x7ff, 0);
		}

		z_exp = b_exp;
		if (a_exp) { /* hidden bit */
			a_sig += 0x2000000000000000ULL;
		} else { /* subnormal */
			a_sig <<= 1;
		}
		a_sig = _float_rshift_jam64(a_sig, -sh);
	} else { /* a > b */
		if (a_exp == 0x7ff) /* inf, nan */
			return a;

		z_exp = a_exp;
		if (b_exp) {
			b_sig += 0x2000000000000000ULL;
		} else {
			b_sig <<= 1;
		}
		b_sig = _float_rshift_jam64(b_sig, sh);
	}

	/* hidden bit */
	z_sig = 0x2000000000000000ULL + a_sig + b_sig;
	if (z_sig < 0x4000000000000000ULL) { /* normalization */
		z_exp--;
		z_sig <<= 1;
	}

	return _float_round_pack64(z_sign, z_exp, z_sig);
}

/* @func: _float_sub_mags64 (static)
 * #desc:
 *    floating-point magnitudes subtraction (64-bit).
 *
 * #1: a      [in]  minuend
 * #2: b      [in]  subtract
 * #3: z_sign [in]  sign
 * #r:        [ret] difference
 */
static uint64_t _float_sub_mags64(uint64_t a, uint64_t b, int32_t z_sign)
{
	int32_t a_exp, b_exp, z_exp, sh;
	uint64_t a_sig, b_sig, z_sig;

	a_exp = (a >> 52) & 0x7ff;
	a_sig = a & 0xfffffffffffffULL;
	b_exp = (b >> 52) & 0x7ff;
	b_sig = b & 0xfffffffffffffULL;
	/*
	 *    Inf -  Inf =>  NaN
	 *    Inf - -Inf =>  Inf
	 * finite -  Inf => -Inf
	 */

	sh = a_exp - b_exp;
	if (!sh) { /* a == b */
		if (a_exp == 0x7ff) { /* inf, nan */
			if (b_exp == 0x7ff && b_sig)
				return b;
			return _float_pack64(0, 0x7ff,
				0x8000000000000ULL);
		}

		z_sig = a_sig - b_sig;
		if (!z_sig) /* zero */
			return _float_pack64(0, 0, 0);

		if (a_exp)
			a_exp--;
		if (z_sig < 0) { /* a < b */
			z_sign = !z_sign;
			z_sig = -z_sig;
		}

		/* z_sig is aligned to 53 bit */
		sh = conch_csf_clz64(z_sig) - 11;
		z_exp = a_exp - sh;
		if (z_exp < 0) { /* subnormal */
			sh = a_exp;
			z_exp = 0;
		}

		return _float_pack64(z_sign, z_exp, z_sig << sh);
	}

	a_sig <<= 10; /* grs */
	b_sig <<= 10;
	if (sh < 0) { /* a < b */
		z_sign = !z_sign;
		if (b_exp == 0x7ff) { /* inf, nan */
			if (b_sig)
				return b;
			return _float_pack64(z_sign, 0x7ff, 0);
		}

		if (a_exp) { /* hidden bit */
			a_sig += 0x4000000000000000ULL;
		} else { /* subnormal */
			a_sig <<= 1;
		}
		a_sig = _float_rshift_jam64(a_sig, -sh);

		z_exp = b_exp; /* hidden bit */
		z_sig = 0x4000000000000000ULL + b_sig - a_sig;
	} else { /* a > b */
		if (a_exp == 0x7ff) /* inf, nan */
			return a;

		if (b_exp) {
			b_sig += 0x4000000000000000ULL;
		} else {
			b_sig <<= 1;
		}
		b_sig = _float_rshift_jam64(b_sig, sh);

		z_exp = a_exp;
		z_sig = 0x4000000000000000ULL + a_sig - b_sig;
	}

	return _float_norm_round_pack64(z_sign, z_exp - 1, z_sig);
}

/* @func: conch_csf_fadd64
 * #desc:
 *    floating-point addition (64-bit).
 *
 * #1: a [in]  addend
 * #2: b [in]  addend
 * #r:   [ret] sum
 */
uint64_t conch_csf_fadd64(uint64_t a, uint64_t b)
{
	int32_t a_sign = a >> 63;
	int32_t b_sign = b >> 63;

	if (a_sign == b_sign) {
		return _float_add_mags64(a, b, a_sign);
	} else {
		return _float_sub_mags64(a, b, a_sign);
	}
}

/* @func: conch_csf_fsub64
 * #desc:
 *    floating-point subtraction (64-bit).
 *
 * #1: a [in]  minuend
 * #2: b [in]  subtract
 * #r:   [ret] difference
 */
uint64_t conch_csf_fsub64(uint64_t a, uint64_t b)
{
	int32_t a_sign = a >> 63;
	int32_t b_sign = b >> 63;

	if (a_sign == b_sign) {
		return _float_sub_mags64(a, b, a_sign);
	} else {
		return _float_add_mags64(a, b, a_sign);
	}
}

/* @func: conch_csf_fmul64
 * #desc:
 *    floating-point multiplication (64-bit).
 *
 * #1: a [in]  multiplicand
 * #2: b [in]  multiplier
 * #r:   [ret] product
 */
uint64_t conch_csf_fmul64(uint64_t a, uint64_t b)
{
	int32_t a_sign, b_sign, z_sign, a_exp, b_exp, z_exp, sh;
	uint64_t a_sig, b_sig, z_sig;

	a_sign = a >> 63;
	a_exp = (a >> 52) & 0x7ff;
	a_sig = a & 0xfffffffffffffULL;
	b_sign = b >> 63;
	b_exp = (b >> 52) & 0x7ff;
	b_sig = b & 0xfffffffffffffULL;
	z_sign = a_sign ^ b_sign;
	/*
	 *   x * NaN      =>  NaN
	 * Inf * 0        =>  NaN (invalid)
	 * Inf * finite   =>  Inf
	 * Inf * Inf      =>  Inf
	 */

	if (a_exp == 0x7ff) { /* inf, nan, zero */
		if (a_sig)
			return a;
		if (b_exp == 0x7ff && b_sig)
			return b;
		if (b_exp || b_sig) {
			return _float_pack64(0, 0x7ff,
				0x8000000000000ULL);
		}
		return _float_pack64(z_sign, 0, 0);
	}
	if (b_exp == 0x7ff) { /* nan, zero */
		if (a_exp || a_sig) {
			return _float_pack64(0, 0x7ff,
				0x8000000000000ULL);
		}
		return _float_pack64(z_sign, 0, 0);
	}

	/* subnormal */
	if (!a_exp) {
		if (!a_sig)
			return _float_pack64(z_sign, 0, 0);
		/* aligned */
		sh = conch_csf_clz64(a_sig) - 11;
		a_exp = 1 - sh;
		a_sig = a_sig << sh;
	}
	if (!b_exp) {
		if (!b_sig)
			return _float_pack64(z_sign, 0, 0);
		/* aligned */
		sh = conch_csf_clz64(b_sig) - 11;
		b_exp = 1 - sh;
		b_sig = b_sig << sh;
	}

	z_exp = a_exp + b_exp - 1023;
	a_sig += 0x10000000000000ULL; /* hidden bit */
	b_sig += 0x10000000000000ULL;
	a_sig <<= 10; /* aligned and grs */
	b_sig <<= 11;

	conch_csf_umul64_128(&a_sig, &b_sig);
	z_sig = b_sig | !!a_sig;

	if (z_sig < 0x4000000000000000ULL) { /* normalization */
		z_exp--;
		z_sig <<= 1;
	}

	return _float_round_pack64(z_sign, z_exp, z_sig);
}

/* @func: conch_csf_fdiv64
 * #desc:
 *    floating-point division (64-bit).
 *
 * #1: a [in]  dividend
 * #2: b [in]  divisor
 * #r:   [ret] quotient
 */
uint64_t conch_csf_fdiv64(uint64_t a, uint64_t b)
{
	int32_t a_sign, b_sign, z_sign, a_exp, b_exp, z_exp, sh;
	uint64_t a_sig, b_sig, z_sig;

	a_sign = a >> 63;
	a_exp = (a >> 52) & 0x7ff;
	a_sig = a & 0xfffffffffffffULL;
	b_sign = b >> 63;
	b_exp = (b >> 52) & 0x7ff;
	b_sig = b & 0xfffffffffffffULL;
	z_sign = a_sign ^ b_sign;
	/*
	 *      x / NaN    => NaN
	 *    NaN / x      => NaN
	 * finite / 0      => Inf
	 *      0 / 0      => NaN (invalid)
	 *    Inf / Inf    => NaN (invalid)
	 *    Inf / finite => Inf
	 * finite / Inf    => 0
	 */

	if (a_exp == 0x7ff) { /* inf, nan */
		if (a_sig)
			return a;
		if (b_exp == 0x7ff) {
			if (b_sig)
				return b;
			return _float_pack64(0, 0x7ff,
				0x8000000000000ULL);
		}
		return _float_pack64(z_sign, 0x7ff, 0);
	}
	if (b_exp == 0x7ff) { /* nan, zero */
		if (b_sig)
			return b;
		return _float_pack64(z_sign, 0, 0);
	}

	/* subnormal */
	if (!b_exp) {
		if (!b_sig) { /* inf, nan */
			if (!(a_exp || a_sig)) {
				return _float_pack64(0, 0x7ff,
					0x8000000000000ULL);
			}
			return _float_pack64(z_sign, 0x7ff, 0);
		}
		/* aligned */
		sh = conch_csf_clz64(b_sig) - 11;
		b_exp = 1 - sh;
		b_sig = b_sig << sh;
	}
	if (!a_exp) {
		if (!a_sig) /* zero */
			return _float_pack64(z_sign, 0, 0);
		/* aligned */
		sh = conch_csf_clz64(a_sig) - 11;
		a_exp = 1 - sh;
		a_sig = a_sig << sh;
	}

	z_exp = a_exp - b_exp + 1022;
	a_sig += 0x10000000000000ULL; /* hidden bit */
	b_sig += 0x10000000000000ULL;
	if (a_sig < b_sig) { /* aligned and grs */
		z_exp--;
		a_sig <<= 11;
	} else {
		a_sig <<= 10;
	}
	b_sig <<= 11;

	/* recip32 = approx(2^32 / b_sig_hi) */
	uint32_t recip32 = _float_approx_recip32_1(b_sig >> 32) - 2;
	/* q0 = a_sig_hi / b_sig_hi hig-bit quotient */
	z_sig = ((uint64_t)(a_sig >> 32) * recip32) >> 32;

	/* r = A - 2q0 * B */
	uint64_t rem = a_sig, double_term = z_sig << 1;
	rem -= (uint64_t)double_term * (b_sig >> 32);
	rem <<= 28;
	rem -= (uint64_t)double_term * ((uint32_t)b_sig >> 4);

	/* q = r_hi / b_sig_hi quotient refinement */
	uint64_t q = ((uint64_t)(rem >> 32) * recip32) >> 32;
	q += 4;

	z_sig = (z_sig << 32) + (q << 4);

	if ((z_sig & 0x1ff) < (4 << 4)) {
		q &= ~7ULL; /* mask low bits */
		z_sig &= ~0x7fULL; /* mask grs */

		/* r = r - 2q * B */
		double_term = q << 1;
		rem -= (uint64_t)double_term * (b_sig >> 32);
		rem <<= 28;
		rem -= (uint64_t)double_term * ((uint32_t)b_sig >> 4);

		if (rem & 0x8000000000000000ULL) { /* r < 0 */
			z_sig -= 1 << 7;
		} else if (rem) { /* sticky */
			z_sig |= 1;
		}
	}

	return _float_round_pack64(z_sign, z_exp, z_sig);
}

/* @func: conch_csf_f64_eq
 * #desc:
 *    floating-point compare is equal (64-bit).
 *
 * #1: a [in]  floating-point
 * #2: b [in]  floating-point
 * #r:   [ret] 1: true, 0: false
 */
int32_t conch_csf_f64_eq(uint64_t a, uint64_t b)
{
	int32_t a_sign, b_sign, a_exp, b_exp;
	uint64_t a_sig, b_sig;

	a_sign = a >> 63;
	a_exp = (a >> 52) & 0x7ff;
	a_sig = a & 0xfffffffffffffULL;
	b_sign = b >> 63;
	b_exp = (b >> 52) & 0x7ff;
	b_sig = b & 0xfffffffffffffULL;

	if (a_sign != b_sign)
		return 0;
	if ((a_exp == 0x7ff && a_sig) || (b_exp == 0x7ff && b_sig))
		return 0;

	return a_exp == b_exp && a_sig == b_sig;
}

/* @func: conch_csf_f64_ne
 * #desc:
 *    floating-point compare is not equal (64-bit).
 *
 * #1: a [in]  floating-point
 * #2: b [in]  floating-point
 * #r:   [ret] 1: true, 0: false
 */
int32_t conch_csf_f64_ne(uint64_t a, uint64_t b)
{
	return !conch_csf_f64_eq(a, b);
}

/* @func: conch_csf_f64_gt
 * #desc:
 *    floating-point compare is greater than (64-bit).
 *
 * #1: a [in]  floating-point
 * #2: b [in]  floating-point
 * #r:   [ret] 1: true, 0: false
 */
int32_t conch_csf_f64_gt(uint64_t a, uint64_t b)
{
	int32_t a_sign, b_sign, a_exp, b_exp;
	uint64_t a_sig, b_sig;

	a_sign = a >> 63;
	a_exp = (a >> 52) & 0x7ff;
	a_sig = a & 0xfffffffffffffULL;
	b_sign = b >> 63;
	b_exp = (b >> 52) & 0x7ff;
	b_sig = b & 0xfffffffffffffULL;

	if ((a_exp == 0x7ff && a_sig) || (b_exp == 0x7ff && b_sig))
		return 0;

	if (a_sign != b_sign)
		return b_sign > a_sign;
	if ((a_exp - b_exp) > 0)
		return 1;

	return a_exp == b_exp && a_sig > b_sig;
}

/* @func: conch_csf_f64_ge
 * #desc:
 *    floating-point compare is greater than or equal (64-bit).
 *
 * #1: a [in]  floating-point
 * #2: b [in]  floating-point
 * #r:   [ret] 1: true, 0: false
 */
int32_t conch_csf_f64_ge(uint64_t a, uint64_t b)
{
	return conch_csf_f64_gt(a, b) || conch_csf_f64_eq(a, b);
}

/* @func: conch_csf_f64_lt
 * #desc:
 *    floating-point compare is less than (64-bit).
 *
 * #1: a [in]  floating-point
 * #2: b [in]  floating-point
 * #r:   [ret] 1: true, 0: false
 */
int32_t conch_csf_f64_lt(uint64_t a, uint64_t b)
{
	int32_t a_sign, b_sign, a_exp, b_exp;
	uint64_t a_sig, b_sig;

	a_sign = a >> 63;
	a_exp = (a >> 52) & 0x7ff;
	a_sig = a & 0xfffffffffffffULL;
	b_sign = b >> 63;
	b_exp = (b >> 52) & 0x7ff;
	b_sig = b & 0xfffffffffffffULL;

	if ((a_exp == 0x7ff && a_sig) || (b_exp == 0x7ff && b_sig))
		return 0;
	if (a_sign != b_sign)
		return a_sign > b_sign;
	if ((a_exp - b_exp) < 0)
		return 1;

	return a_exp == b_exp && a_sig < b_sig;
}

/* @func: conch_csf_f64_le
 * #desc:
 *    floating-point compare is less than or equal (64-bit).
 *
 * #1: a [in]  floating-point
 * #2: b [in]  floating-point
 * #r:   [ret] 1: true, 0: false
 */
int32_t conch_csf_f64_le(uint64_t a, uint64_t b)
{
	return conch_csf_f64_lt(a, b) || conch_csf_f64_eq(a, b);
}

/* @func: conch_csf_i64_to_f64
 * #desc:
 *    signed integer to floating-point conversion (64-bit).
 *
 * #1: a [in]  number
 * #r:   [ret] floating-point
 */
uint64_t conch_csf_i64_to_f64(int64_t a)
{
	int32_t sign = a < 0;

	if ((uint64_t)a & 0x7fffffffffffffffULL) {
		a = sign ? -a : a;
		return _float_norm_round_pack64(sign, 1023 + 61, (uint64_t)a);
	}

	return _float_pack64(sign, sign ? (1023 + 63) : 0, 0);
}

/* @func: conch_csf_u64_to_f64
 * #desc:
 *    unsigned integer to floating-point conversion (64-bit).
 *
 * #1: a [in]  number
 * #r:   [ret] floating-point
 */
uint64_t conch_csf_u64_to_f64(uint64_t a)
{
	if (!a)
		return 0;

	if (a & 0x8000000000000000ULL) {
		a = _float_rshift_jam64(a, 1);
		return _float_round_pack64(0, 1023 + 62, a);
	}

	return _float_norm_round_pack64(0, 1023 + 61, a);
}

/* @func: conch_csf_f64_to_i64
 * #desc:
 *    floating-point to signed integer conversion (64-bit).
 *
 * #1: a    [in]  floating-point
 * #2: mode [in]  rounding mode
 * #r:      [ret] number
 */
int64_t conch_csf_f64_to_i64(uint64_t a, int32_t mode)
{
	int32_t sign, exp, sh, round_bits;
	uint64_t sig;

	sign = a >> 63;
	exp = (a >> 52) & 0x7ff;
	sig = a & 0xfffffffffffffULL;

	if (exp == 0x7ff) /* inf, nan */
		return 0;

	sh = exp - 1023;
	if (sh <= 0)
		return 0;

	sig |= 0x10000000000000ULL; /* hidden bit */
	if (sh > 52) {
		if (sh > 63)
			goto e;
		sig <<= sh - 52;
	} else if (mode) { /* round */
		sig <<= 10;
		sig = _float_rshift_jam64(sig, 52 - sh);
		round_bits = sig & 0x3ff;

		sig = (sig + 0x200) >> 10;
		if (round_bits == 0x200)
			sig &= ~1ULL;
	} else {
		sig >>= 52 - sh;
	}

	sig = sign ? -sig : sig;
	if (sig && ((int32_t)(sig >> 63) ^ sign))
		goto e;

	return (int64_t)sig;
e:
	return sign ? INT64_MIN : INT64_MAX;
}

/* @func: conch_csf_f64_to_u64
 * #desc:
 *    floating-point to unsigned integer conversion (64-bit).
 *
 * #1: a    [in]  floating-point
 * #2: mode [in]  rounding mode
 * #r:      [ret] number
 */
uint64_t conch_csf_f64_to_u64(uint64_t a, int32_t mode)
{
	int32_t sign, exp, sh, round_bits;
	uint64_t sig;

	sign = a >> 63;
	exp = (a >> 52) & 0x7ff;
	sig = a & 0xfffffffffffffULL;

	if (exp == 0x7ff) /* inf, nan */
		return 0;

	sh = exp - 1023;
	if (sh <= 0)
		return 0;

	sig |= 0x10000000000000ULL; /* hidden bit */
	if (sh > 52) {
		if (sh > 63)
			goto e;
		sig <<= sh - 52;
	} else if (mode) { /* round */
		sig <<= 10;
		sig = _float_rshift_jam64(sig, 52 - sh);
		round_bits = sig & 0x3ff;

		sig = (sig + 0x200) >> 10;
		if (round_bits == 0x200)
			sig &= ~1ULL;
	} else {
		sig >>= 52 - sh;
	}

	if (sign && sig)
		goto e;

	return sig;
e:
	return UINT64_MAX;
}

/* @func: _float_pack32 (static)
 * #desc:
 *    floating-point binary package (32-bit).
 *
 * #1: sign [in]  sign
 * #2: exp  [in]  exponent
 * #3: sig  [in]  significand (with hidden bit or exponent bit)
 * #r:      [ret] floating-point
 */
static uint32_t _float_pack32(int32_t sign, int32_t exp, uint32_t sig)
{
	sig += (uint32_t)sign << 31;
	sig += (uint32_t)exp << 23;

	return sig;
}

/* @func: _float_rshift_jam32 (static)
 * #desc:
 *    right shift and add sticky to the low bit (32-bit).
 *
 * #1: a [in]  number
 * #2: b [in]  shift distance
 * #r:   [ret] value after right shift
 */
static uint32_t _float_rshift_jam32(uint32_t a, int32_t b)
{
	if (b < 31) { /* shift + sticky */
		a = (a >> b) | !!(a << (32 - b));
	} else {
		a = !!a; /* sticky */
	}

	return a;
}

/* @func: _float_round_pack32 (static)
 * #desc:
 *    rounding the grs bits and returns the floating-point (32-bit).
 *
 * #1: sign [in]  sign
 * #2: exp  [in]  exponent
 * #3: sig  [in]  significand (with grs bits)
 * #r:      [ret] floating-point
 */
static uint32_t _float_round_pack32(int32_t sign, int32_t exp, uint32_t sig)
{
	int32_t round_bits = sig & 0x7f; /* guard + round + sticky */
	/*
	 * |G R S              carry|
	 * +========================+
	 * |0 x x | < 0.5     | no  |
	 * |1 0 0 | = 0.5 tie | lsb |
	 * |1 0 1 | > 0.5     | yes |
	 * |1 1 x | > 0.5     | yes |
	 */

	if (exp < 0) { /* subnormal */
		sig = _float_rshift_jam32(sig, -exp);
		round_bits = sig & 0x7f;
		exp = 0;
	} else if ((exp > (127 + 126)) || ((exp == (127 + 126))
			&& ((sig + 0x40) >= 0x80000000))) {
		return _float_pack32(sign, 0xff, 0); /* inf */
	}

	sig = (sig + 0x40) >> 7; /* rounding */
	if (round_bits == 0x40) /* round-to-even */
		sig &= ~1U;

	if (!sig) /* zero */
		exp = 0;

	return _float_pack32(sign, exp, sig);
}

/* @func: _float_norm_round_pack32 (static)
 * #desc:
 *    normalization and rounding, then return a floating-point (32-bit).
 *
 * #1: sign [in]  sign
 * #2: exp  [in]  exponent
 * #3: sig  [in]  significand (with grs bits)
 * #r:      [ret] floating-point
 */
static uint32_t _float_norm_round_pack32(int32_t sign, int32_t exp,
		uint32_t sig)
{
	int32_t sh = conch_csf_clz32(sig) - 1;
	exp -= sh;

	if (sh >= 7 && exp < (127 + 126))
		return _float_pack32(sign, sig ? exp : 0, sig << (sh - 7));

	return _float_round_pack32(sign, exp, sig << sh);
}

/* @func: _float_add_mags32 (static)
 * #desc:
 *    floating-point magnitudes addition (32-bit).
 *
 * #1: a      [in]  addend
 * #2: b      [in]  addend
 * #3: z_sign [in]  sign
 * #r:        [ret] sum
 */
static uint32_t _float_add_mags32(uint32_t a, uint32_t b, int32_t z_sign)
{
	int32_t a_exp, b_exp, z_exp, sh;
	uint32_t a_sig, b_sig, z_sig;

	a_exp = (a >> 23) & 0xff;
	a_sig = a & 0x7fffff;
	b_exp = (b >> 23) & 0xff;
	b_sig = b & 0x7fffff;
	/*
	 *   x + NaN => NaN
	 * Inf + Inf => Inf
	 */

	sh = a_exp - b_exp;
	if (!sh) { /* a == b */
		if (a_exp == 0xff) { /* inf, nan */
			if (b_exp == 0xff && b_sig)
				return b;
			return a;
		}

		z_exp = a_exp;
		if (!z_exp) /* subnormal */
			return a + b_sig;

		/* hidden bit of a and b */
		z_sig = 0x1000000 + a_sig + b_sig;
		z_sig <<= 6; /* grs */
		return _float_round_pack32(z_sign, z_exp, z_sig);
	}

	a_sig <<= 6; /* grs */
	b_sig <<= 6;
	if (sh < 0) { /* a < b */
		if (b_exp == 0xff) { /* inf, nan */
			if (b_sig)
				return a;
			return _float_pack32(z_sign, 0xff, 0);
		}

		z_exp = b_exp;
		if (a_exp) { /* hidden bit */
			a_sig += 0x20000000;
		} else { /* subnormal */
			a_sig <<= 1;
		}
		a_sig = _float_rshift_jam32(a_sig, -sh);
	} else { /* a > b */
		if (a_exp == 0xff) /* inf, nan */
			return a;

		z_exp = a_exp;
		if (b_exp) {
			b_sig += 0x20000000;
		} else {
			b_sig <<= 1;
		}
		b_sig = _float_rshift_jam32(b_sig, sh);
	}

	/* hidden bit */
	z_sig = 0x20000000 + a_sig + b_sig;
	if (z_sig < 0x40000000) { /* normalization */
		z_exp--;
		z_sig <<= 1;
	}

	return _float_round_pack32(z_sign, z_exp, z_sig);
}

/* @func: _float_sub_mags32 (static)
 * #desc:
 *    floating-point magnitudes subtraction (32-bit).
 *
 * #1: a      [in]  minuend
 * #2: b      [in]  subtract
 * #3: z_sign [in]  sign
 * #r:        [ret] difference
 */
static uint32_t _float_sub_mags32(uint32_t a, uint32_t b, int32_t z_sign)
{
	int32_t a_exp, b_exp, z_exp, sh;
	uint32_t a_sig, b_sig, z_sig;

	a_exp = (a >> 23) & 0xff;
	a_sig = a & 0x7fffff;
	b_exp = (b >> 23) & 0xff;
	b_sig = b & 0x7fffff;
	/*
	 *    Inf -  Inf =>  NaN
	 *    Inf - -Inf =>  Inf
	 * finite -  Inf => -Inf
	 */

	sh = a_exp - b_exp;
	if (!sh) { /* a == b */
		if (a_exp == 0xff) { /* inf, nan */
			if (b_exp == 0xff && b_sig)
				return b;
			return _float_pack32(0, 0xff,
				0x400000);
		}

		z_sig = a_sig - b_sig;
		if (!z_sig) /* zero */
			return _float_pack32(0, 0, 0);

		if (a_exp)
			a_exp--;
		if (z_sig < 0) { /* a < b */
			z_sign = !z_sign;
			z_sig = -z_sig;
		}

		/* z_sig is aligned to 24 bit */
		sh = conch_csf_clz32(z_sig) - 8;
		z_exp = a_exp - sh;
		if (z_exp < 0) { /* subnormal */
			sh = a_exp;
			z_exp = 0;
		}

		return _float_pack32(z_sign, z_exp, z_sig << sh);
	}

	a_sig <<= 7; /* grs */
	b_sig <<= 7;
	if (sh < 0) { /* a < b */
		z_sign = !z_sign;
		if (b_exp == 0xff) { /* inf, nan */
			if (b_sig)
				return b;
			return _float_pack32(z_sign, 0xff, 0);
		}

		if (a_exp) { /* hidden bit */
			a_sig += 0x40000000;
		} else { /* subnormal */
			a_sig <<= 1;
		}
		a_sig = _float_rshift_jam32(a_sig, -sh);

		z_exp = b_exp; /* hidden bit */
		z_sig = 0x40000000 + b_sig - a_sig;
	} else { /* a > b */
		if (a_exp == 0xff) /* inf, nan */
			return a;

		if (b_exp) {
			b_sig += 0x40000000;
		} else {
			b_sig <<= 1;
		}
		b_sig = _float_rshift_jam32(b_sig, sh);

		z_exp = a_exp;
		z_sig = 0x40000000 + a_sig - b_sig;
	}

	return _float_norm_round_pack32(z_sign, z_exp - 1, z_sig);
}

/* @func: conch_csf_fadd32
 * #desc:
 *    floating-point addition (32-bit).
 *
 * #1: a [in]  addend
 * #2: b [in]  addend
 * #r:   [ret] sum
 */
uint32_t conch_csf_fadd32(uint32_t a, uint32_t b)
{
	int32_t a_sign = a >> 31;
	int32_t b_sign = b >> 31;

	if (a_sign == b_sign) {
		return _float_add_mags32(a, b, a_sign);
	} else {
		return _float_sub_mags32(a, b, a_sign);
	}
}

/* @func: conch_csf_fsub32
 * #desc:
 *    floating-point subtraction (32-bit).
 *
 * #1: a [in]  minuend
 * #2: b [in]  subtract
 * #r:   [ret] difference
 */
uint32_t conch_csf_fsub32(uint32_t a, uint32_t b)
{
	int32_t a_sign = a >> 31;
	int32_t b_sign = b >> 31;

	if (a_sign == b_sign) {
		return _float_sub_mags32(a, b, a_sign);
	} else {
		return _float_add_mags32(a, b, a_sign);
	}
}

/* @func: conch_csf_fmul32
 * #desc:
 *    floating-point multiplication (32-bit).
 *
 * #1: a [in]  multiplicand
 * #2: b [in]  multiplier
 * #r:   [ret] product
 */
uint32_t conch_csf_fmul32(uint32_t a, uint32_t b)
{
	int32_t a_sign, b_sign, z_sign, a_exp, b_exp, z_exp, sh;
	uint32_t a_sig, b_sig, z_sig;

	a_sign = a >> 31;
	a_exp = (a >> 23) & 0xff;
	a_sig = a & 0x7fffff;
	b_sign = b >> 31;
	b_exp = (b >> 23) & 0xff;
	b_sig = b & 0x7fffff;
	z_sign = a_sign ^ b_sign;
	/*
	 *   x * NaN      =>  NaN
	 * Inf * 0        =>  NaN (invalid)
	 * Inf * finite   =>  Inf
	 * Inf * Inf      =>  Inf
	 */

	if (a_exp == 0xff) { /* inf, nan, zero */
		if (a_sig)
			return a;
		if (b_exp == 0xff && b_sig)
			return b;
		if (b_exp || b_sig) {
			return _float_pack32(0, 0xff,
				0x400000);
		}
		return _float_pack32(z_sign, 0, 0);
	}
	if (b_exp == 0xff) { /* nan, zero */
		if (a_exp || a_sig) {
			return _float_pack32(0, 0xff,
				0x400000);
		}
		return _float_pack32(z_sign, 0, 0);
	}

	/* subnormal */
	if (!a_exp) {
		if (!a_sig)
			return _float_pack32(z_sign, 0, 0);
		/* aligned */
		sh = conch_csf_clz32(a_sig) - 8;
		a_exp = 1 - sh;
		a_sig = a_sig << sh;
	}
	if (!b_exp) {
		if (!b_sig)
			return _float_pack32(z_sign, 0, 0);
		/* aligned */
		sh = conch_csf_clz32(b_sig) - 8;
		b_exp = 1 - sh;
		b_sig = b_sig << sh;
	}

	z_exp = a_exp + b_exp - 127;
	a_sig += 0x800000; /* hidden bit */
	b_sig += 0x800000;
	a_sig <<= 7; /* aligned and grs */
	b_sig <<= 8;

	uint64_t t = (uint64_t)a_sig * b_sig;
	a_sig = t & 0xffffffff;
	b_sig = t >> 32;
	z_sig = b_sig | !!a_sig;

	if (z_sig < 0x40000000) { /* normalization */
		z_exp--;
		z_sig <<= 1;
	}

	return _float_round_pack32(z_sign, z_exp, z_sig);
}

/* @func: conch_csf_fdiv32
 * #desc:
 *    floating-point division (32-bit).
 *
 * #1: a [in]  dividend
 * #2: b [in]  divisor
 * #r:   [ret] quotient
 */
uint32_t conch_csf_fdiv32(uint32_t a, uint32_t b)
{
	int32_t a_sign, b_sign, z_sign, a_exp, b_exp, z_exp, sh;
	uint32_t a_sig, b_sig, z_sig;

	a_sign = a >> 31;
	a_exp = (a >> 23) & 0xff;
	a_sig = a & 0x7fffff;
	b_sign = b >> 31;
	b_exp = (b >> 23) & 0xff;
	b_sig = b & 0x7fffff;
	z_sign = a_sign ^ b_sign;
	/*
	 *      x / NaN    => NaN
	 *    NaN / x      => NaN
	 * finite / 0      => Inf
	 *      0 / 0      => NaN (invalid)
	 *    Inf / Inf    => NaN (invalid)
	 *    Inf / finite => Inf
	 * finite / Inf    => 0
	 */

	if (a_exp == 0xff) { /* inf, nan */
		if (a_sig)
			return a;
		if (b_exp == 0xff) {
			if (b_sig)
				return b;
			return _float_pack32(0, 0xff,
				0x400000);
		}
		return _float_pack32(z_sign, 0xff, 0);
	}
	if (b_exp == 0xff) { /* nan, zero */
		if (b_sig)
			return b;
		return _float_pack32(z_sign, 0, 0);
	}

	/* subnormal */
	if (!b_exp) {
		if (!b_sig) { /* inf, nan */
			if (!(a_exp || a_sig)) {
				return _float_pack32(0, 0xff,
					0x400000);
			}
			return _float_pack32(z_sign, 0xff, 0);
		}
		/* aligned */
		sh = conch_csf_clz32(b_sig) - 8;
		b_exp = 1 - sh;
		b_sig = b_sig << sh;
	}
	if (!a_exp) {
		if (!a_sig) /* zero */
			return _float_pack32(z_sign, 0, 0);
		/* aligned */
		sh = conch_csf_clz32(a_sig) - 8;
		a_exp = 1 - sh;
		a_sig = a_sig << sh;
	}

	z_exp = a_exp - b_exp + 126;
	a_sig += 0x800000; /* hidden bit */
	b_sig += 0x800000;
	if (a_sig < b_sig) { /* aligned and grs */
		z_exp--;
		a_sig <<= 8;
	} else {
		a_sig <<= 7;
	}
	b_sig <<= 8;

	/* recip32 = approx(2^32 / b_sig) */
	uint32_t recip32 = _float_approx_recip32_1(b_sig);
	/* z_sig = a_sig / b_sig quotient */
	z_sig = ((uint64_t)a_sig * recip32) >> 32;
	z_sig += 2;;

	if ((z_sig & 0x3f) < 2) {
		z_sig &= ~0x3ULL; /* mask grs */

		/* r = A - z * B */
		uint64_t rem = (uint64_t)a_sig << 31;
		rem -= (uint64_t)z_sig * b_sig;

		if (rem & 0x8000000000000000ULL) { /* r < 0 */
			z_sig -= 1 << 2;
		} else if (rem) { /* sticky */
			z_sig |= 1;
		}
	}

	return _float_round_pack32(z_sign, z_exp, z_sig);
}

/* @func: conch_csf_f32_eq
 * #desc:
 *    floating-point compare is equal (32-bit).
 *
 * #1: a [in]  floating-point
 * #2: b [in]  floating-point
 * #r:   [ret] 1: true, 0: false
 */
int32_t conch_csf_f32_eq(uint32_t a, uint32_t b)
{
	int32_t a_sign, b_sign, a_exp, b_exp;
	uint32_t a_sig, b_sig;

	a_sign = a >> 31;
	a_exp = (a >> 23) & 0xff;
	a_sig = a & 0x7fffff;
	b_sign = b >> 31;
	b_exp = (b >> 23) & 0xff;
	b_sig = b & 0x7fffff;

	if (a_sign != b_sign)
		return 0;
	if ((a_exp == 0xff && a_sig) || (b_exp == 0xff && b_sig))
		return 0;

	return a_exp == b_exp && a_sig == b_sig;
}

/* @func: conch_csf_f32_ne
 * #desc:
 *    floating-point compare is not equal (32-bit).
 *
 * #1: a [in]  floating-point
 * #2: b [in]  floating-point
 * #r:   [ret] 1: true, 0: false
 */
int32_t conch_csf_f32_ne(uint32_t a, uint32_t b)
{
	return !conch_csf_f32_eq(a, b);
}

/* @func: conch_csf_f32_gt
 * #desc:
 *    floating-point compare is greater than (32-bit).
 *
 * #1: a [in]  floating-point
 * #2: b [in]  floating-point
 * #r:   [ret] 1: true, 0: false
 */
int32_t conch_csf_f32_gt(uint32_t a, uint32_t b)
{
	int32_t a_sign, b_sign, a_exp, b_exp;
	uint32_t a_sig, b_sig;

	a_sign = a >> 31;
	a_exp = (a >> 23) & 0xff;
	a_sig = a & 0x7fffff;
	b_sign = b >> 31;
	b_exp = (b >> 23) & 0xff;
	b_sig = b & 0x7fffff;

	if ((a_exp == 0xff && a_sig) || (b_exp == 0xff && b_sig))
		return 0;

	if (a_sign != b_sign)
		return b_sign > a_sign;
	if ((a_exp - b_exp) > 0)
		return 1;

	return a_exp == b_exp && a_sig > b_sig;
}

/* @func: conch_csf_f32_ge
 * #desc:
 *    floating-point compare is greater than or equal (32-bit).
 *
 * #1: a [in]  floating-point
 * #2: b [in]  floating-point
 * #r:   [ret] 1: true, 0: false
 */
int32_t conch_csf_f32_ge(uint32_t a, uint32_t b)
{
	return conch_csf_f32_gt(a, b) || conch_csf_f32_eq(a, b);
}

/* @func: conch_csf_f32_lt
 * #desc:
 *    floating-point compare is less than (32-bit).
 *
 * #1: a [in]  floating-point
 * #2: b [in]  floating-point
 * #r:   [ret] 1: true, 0: false
 */
int32_t conch_csf_f32_lt(uint32_t a, uint32_t b)
{
	int32_t a_sign, b_sign, a_exp, b_exp;
	uint32_t a_sig, b_sig;

	a_sign = a >> 31;
	a_exp = (a >> 23) & 0xff;
	a_sig = a & 0x7fffff;
	b_sign = b >> 31;
	b_exp = (b >> 23) & 0xff;
	b_sig = b & 0x7fffff;

	if ((a_exp == 0xff && a_sig) || (b_exp == 0xff && b_sig))
		return 0;
	if (a_sign != b_sign)
		return a_sign > b_sign;
	if ((a_exp - b_exp) < 0)
		return 1;

	return a_exp == b_exp && a_sig < b_sig;
}

/* @func: conch_csf_f32_le
 * #desc:
 *    floating-point compare is less than or equal (32-bit).
 *
 * #1: a [in]  floating-point
 * #2: b [in]  floating-point
 * #r:   [ret] 1: true, 0: false
 */
int32_t conch_csf_f32_le(uint32_t a, uint32_t b)
{
	return conch_csf_f32_lt(a, b) || conch_csf_f32_eq(a, b);
}

/* @func: conch_csf_i32_to_f32
 * #desc:
 *    signed integer to floating-point conversion (32-bit).
 *
 * #1: a [in]  number
 * #r:   [ret] floating-point
 */
uint32_t conch_csf_i32_to_f32(int32_t a)
{
	int32_t sign = a < 0;

	if ((uint32_t)a & 0x7fffffff) {
		a = sign ? -a : a;
		return _float_norm_round_pack32(sign, 127 + 29, (uint32_t)a);
	}

	return _float_pack32(sign, sign ? (127 + 31) : 0, 0);
}

/* @func: conch_csf_u32_to_f32
 * #desc:
 *    unsigned integer to floating-point conversion (32-bit).
 *
 * #1: a [in]  number
 * #r:   [ret] floating-point
 */
uint32_t conch_csf_u32_to_f32(uint32_t a)
{
	if (!a)
		return 0;

	if (a & 0x80000000) {
		a = _float_rshift_jam32(a, 1);
		return _float_round_pack32(0, 127 + 30, a);
	}

	return _float_norm_round_pack32(0, 127 + 29, a);
}

/* @func: conch_csf_f32_to_i32
 * #desc:
 *    floating-point to signed integer conversion (32-bit).
 *
 * #1: a    [in]  floating-point
 * #2: mode [in]  rounding mode
 * #r:      [ret] number
 */
int32_t conch_csf_f32_to_i32(uint32_t a, int32_t mode)
{
	int32_t sign, exp, sh, round_bits;
	uint32_t sig;

	sign = a >> 31;
	exp = (a >> 23) & 0xff;
	sig = a & 0x7fffff;

	if (exp == 0xff) /* inf, nan */
		return 0;

	sh = exp - 127;
	if (sh <= 0)
		return 0;

	sig |= 0x800000; /* hidden bit */
	if (sh > 23) {
		if (sh > 31)
			goto e;
		sig <<= sh - 23;
	} else if (mode) { /* round */
		sig <<= 7;
		sig = _float_rshift_jam32(sig, 23 - sh);
		round_bits = sig & 0x7f;

		sig = (sig + 0x40) >> 7;
		if (round_bits == 0x40)
			sig &= ~1U;
	} else {
		sig >>= 23 - sh;
	}

	sig = sign ? -sig : sig;
	if (sig && ((int32_t)(sig >> 31) ^ sign))
		goto e;

	return (int32_t)sig;
e:
	return sign ? INT32_MIN : INT32_MAX;
}

/* @func: conch_csf_f32_to_u32
 * #desc:
 *    floating-point to unsigned integer conversion (32-bit).
 *
 * #1: a    [in]  floating-point
 * #2: mode [in]  rounding mode
 * #r:      [ret] number
 */
uint32_t conch_csf_f32_to_u32(uint32_t a, int32_t mode)
{
	int32_t sign, exp, sh, round_bits;
	uint32_t sig;

	sign = a >> 31;
	exp = (a >> 23) & 0xff;
	sig = a & 0x7fffff;

	if (exp == 0xff) /* inf, nan */
		return 0;

	sh = exp - 127;
	if (sh <= 0)
		return 0;

	sig |= 0x800000; /* hidden bit */
	if (sh > 23) {
		if (sh > 31)
			goto e;
		sig <<= sh - 23;
	} else if (mode) { /* round */
		sig <<= 7;
		sig = _float_rshift_jam32(sig, 23 - sh);
		round_bits = sig & 0x7f;

		sig = (sig + 0x40) >> 7;
		if (round_bits == 0x40)
			sig &= ~1U;
	} else {
		sig >>= 23 - sh;
	}

	if (sign && sig)
		goto e;

	return sig;
e:
	return UINT32_MAX;
}
