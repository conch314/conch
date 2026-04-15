/* @file: c_math_double.c
 * #desc:
 *    The implementations of mathematical declarations.
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
#include <conch/c_stdint.h>
#include <conch/c_math.h>


/* @func: conch_fpclassify
 * #desc:
 *    classify real floating-point type.  
 *
 * #1: x [in]  number
 * #r:   [ret] type
 */
int32_t conch_fpclassify(double x)
{
	union {
		double f;
		uint64_t i;
	} u = { x };

	int32_t e = (u.i >> 52) & 0x7ff;
	if (!e) {
		return (u.i << 1) ? X_FP_SUBNORMAL : X_FP_ZERO;
	} else if (e == 0x7ff) {
		return (u.i << 12) ? X_FP_NAN : X_FP_INFINITE;
	}

	return X_FP_NORMAL;
}

/* @func: conch_fabs
 * #desc:
 *    return an floating-point absolute value.
 *
 * #1: x [in]  number
 * #r:   [ret] absolute value
 */
double conch_fabs(double x)
{
	union {
		double f;
		uint64_t i;
	} u = { x };

	u.i &= 0x7fffffffffffffffULL;

	return u.f;
}

/* @func: conch_ceil
 * #desc:
 *    returns the smallest integer value that is not less than x.
 *
 * #1: x [in]  number
 * #r:   [ret] smallest integer of x
 */
double conch_ceil(double x)
{
	union {
		double f;
		uint64_t i;
	} u = { x };

	int32_t e = ((u.i >> 52) & 0x7ff) - 1023;
	if (e > 51) {
		return x;
	} else if (e < 0) {
		return (u.i >> 63) ? -0.0 : 1.0;
	}

	uint64_t m = 1ULL << (52 - e);
	if (!(u.i & (m - 1))) /* integer */
		return x;

	if (x > 0.0) /* correction */
		u.i += m;
	u.i &= ~(m - 1); /* mask decimal part */

	return u.f;
}

/* @func: conch_floor
 * #desc:
 *    returns the largest integer value not greater than x.
 *
 * #1: x [in]  number
 * #r:   [ret] largest integer of x
 */
double conch_floor(double x)
{
	union {
		double f;
		uint64_t i;
	} u = { x };

	int32_t e = ((u.i >> 52) & 0x7ff) - 1023;
	if (e > 51) {
		return x;
	} else if (e < 0) {
		return (u.i >> 63) ? -1.0 : 0.0;
	}

	uint64_t m = 1ULL << (52 - e);
	if (!(u.i & (m - 1))) /* integer */
		return x;

	if (x < 0.0) /* correction */
		u.i += m;
	u.i &= ~(m - 1); /* mask decimal part */

	return u.f;
}

/* @func: conch_trunc
 * #desc:
 *    returns the round to integer value, toward zero.
 *
 * #1: x [in]  number
 * #r:   [ret] integer value
 */
double conch_trunc(double x)
{
	union {
		double f;
		uint64_t i;
	} u = { x };

	int32_t e = ((u.i >> 52) & 0x7ff) - 1023 + 12;
	if (e > (51 + 12)) {
		return x;
	} else if (e < 12) {
		e = 1;
	} /* +12 subnormal */

	uint64_t m = -1ULL >> e;
	if (!(u.i & m))
		return x;

	u.i &= ~m;

	return u.f;
}

/* @func: conch_round
 * #desc:
 *    returns the round to nearest integer, away from zero.
 *
 * #1: x [in]  number
 * #r:   [ret] integer value
 */
double conch_round(double x)
{
	union {
		double f;
		uint64_t i;
	} u = { x };

	int32_t e = ((u.i >> 52) & 0x7ff) - 1023;
	if (e > 51)
		return x;

	double y;
	if (u.i >> 63) {
		y = conch_floor(-x);
		if ((y + x) <= -0.5)
			y += 1;
		return -y;
	}

	y = conch_floor(x);
	if ((y - x) <= -0.5)
		y += 1;

	return y;
}

/* @func: conch_fmod
 * #desc:
 *    floating-point remainder function.
 *
 * #1: x [in]  number
 * #2: y [in]  denominator
 * #r:   [ret] remainder
 */
double conch_fmod(double x, double y)
{
	double n = conch_floor(x / y);

	return x - (n * y);
}

/* @func: conch_frexp
 * #desc:
 *    extract floating-point mantissa and exponent.
 *
 * #1: x [in]  number
 * #2: e [out] exponent
 * #r:   [ret] mantissa
 */
double conch_frexp(double x, int32_t *e)
{
	union {
		double f;
		uint64_t i;
	} u = { x };

	int32_t ee = (u.i >> 52) & 0x7ff;
	if (!ee) {
		if (x) {
			x = conch_frexp(x * 0x1p64, e);
			*e -= 64;
		} else {
			*e = 0;
		}
		return x;
	} else if (ee == 0x7ff) {
		return x;
	}

	*e = ee - 1022;
	u.i &= 0x800fffffffffffffULL;
	u.i |= 0x3fe0000000000000ULL;

	return u.f;
}

/* @func: conch_ldexp
 * #desc:
 *    multiply floating-point number by integral power of 2.
 *
 * #1: x [in]  number
 * #2: e [in]  exponent
 * #r:   [ret] return the x*2^e
 */
double conch_ldexp(double x, int32_t e)
{
	union {
		double f;
		uint64_t i;
	} u = { x };

	if (e > 1023) {
		x *= 0x1p1023;
		e -= 1023;
		if (e > 1023) {
			x *= 0x1p1023;
			e -= 1023;
			if (e > 1023)
				e = 1023;
		}
	} else if (e < -1022) {
		x *= 0x1p-1022 * 0x1p53;
		e += 1022 - 53;
		if (e < -1022) {
			x *= 0x1p-1022 * 0x1p53;
			e += 1022 - 53;
			if (e < -1022)
				e = -1022;
		}
	}

	u.i = (uint64_t)(1023 + e) << 52;
	x *= u.f;

	return x;
}

/* @func: conch_modf
 * #desc:
 *    extract floating-point integer and decimals.
 *
 * #1: x [in]  number
 * #2: n [out] integer
 * #r:   [ret] decimals
 */
double conch_modf(double x, double *n)
{
	union {
		double f;
		uint64_t i;
	} u = { x };

	int32_t e = ((u.i >> 52) & 0x7ff) - 1023;
	if (e > 51) {
		*n = x;
		if (e == 1024 && (u.i << 12))
			return x;
		u.i &= 0x8000000000000000ULL;
		return u.f;
	} else if (e < 0) {
		u.i &= 0x8000000000000000ULL;
		*n = u.f;
		return x;
	}

	uint64_t m = 1ULL << (52 - e);
	if (!(u.i & (m - 1))) {
		*n = x;
		u.i &= 0x8000000000000000ULL;
		return u.f;
	}

	u.i &= ~(m - 1);
	*n = u.f;

	return x - u.f;
}

/* @func: conch_sqrt
 * #desc:
 *    square root function.
 *
 * #1: x [in]  number
 * #r:   [ret] square root of x
 */
double conch_sqrt(double x)
{
	if (x < 0.0)
		return NAN;

#if 0

	/* newton-raphson method */
	double m = x, q = 1.0;
	for (int32_t i = 0; i < 20; i++) {
		m = (m + q) / 2;
		q = x / m;
	}

	return m;

#else

	/* newton-raphson method */
	union {
		double f;
		uint64_t i;
	} u = { x };

	u.i = 0x1ff7a3bea91d9b1bULL + (u.i >> 1);

	double m = u.f;
	m = 0.5 * (m + x / m);
	m = 0.5 * (m + x / m);
	m = 0.5 * (m + x / m);
	m = 0.5 * (m + x / m);

	return m;

#endif
}

/* @func: conch_sin
 * #desc:
 *    sine function.
 *
 * #1: x [in]  number
 * #r:   [ret] sine of x
 */
double conch_sin(double x)
{
	int32_t neg = 1;
	x = conch_fmod(x, M_PI * 2);
	if (x > M_PI) {
		x -= M_PI;
		neg = -1;
	}

	/* taylor series running product */
	int32_t sign = -1;
	double m = x, q = x, x2 = x * x;
	for (int32_t i = 1; i <= 12; i++) {
		q *= x2 / ((2 * i) * (2 * i + 1));
		m += sign * q;
		sign *= -1;
	}

	return m * neg;
}

/* @func: conch_cos
 * #desc:
 *    cosine function.
 *
 * #1: x [in]  number
 * #r:   [ret] cosine of x
 */
double conch_cos(double x)
{
	int32_t neg = 1;
	x = conch_fmod(x, M_PI * 2);
	if (x > M_PI) {
		x -= M_PI;
		neg = -1;
	}

	/* NOTE: https://austinhenley.com/blog/cosine.html */

	/* taylor series running product */
	int32_t sign = -1;
	double m = 1.0, q = 1.0, x2 = x * x;
	for (int32_t i = 1; i <= 12; i++) {
		q *= x2 / ((2 * i) * (2 * i - 1.0));
		m += sign * q;
		sign *= -1;
	}

	return m * neg;
}

/* @func: conch_tan
 * #desc:
 *    tangent function.
 *
 * #1: x [in]  number
 * #r:   [ret] tangent of x
 */
double conch_tan(double x)
{
	return conch_sin(x) / conch_cos(x);
}

/* @func: conch_acos
 * #desc:
 *    arc cosine function.
 *
 * #1: x [in]  number
 * #r:   [ret] arc cosine of x
 */
double conch_acos(double x)
{
	if (x < -1.0 || x > 1.0)
		return NAN;

	double m = M_PI_2;
	for (int32_t i = 0; i < 5; i++)
		m -= (conch_cos(m) - x) / -conch_sin(m);

	return m;
}

/* @func: conch_atan
 * #desc:
 *    arc tangent function.
 *
 * #1: x [in]  number
 * #r:   [ret] arc tangent of x
 */
double conch_atan(double x)
{
	if (x > 1.0) {
		return M_PI_2 - conch_atan(1.0 / x);
	} else if (x < -1.0) {
		return -M_PI_2 - conch_atan(1.0 / x);
	}

#if 0

	/* taylor series running product */
	double m = x, q = x, x2 = x * x;
	for (int32_t i = 1; i <= 10; i++) {
		q *= -x2;
		m += q / (2 * i + 1);
	}

	return m;

#else

	/* precomputed coefficients */
	static const double coeffs[10] = {
		1.0,
		-1.0 / 3,
		1.0 / 5,
		-1.0 / 7,
		1.0 / 9,
		-1.0 / 11,
		1.0 / 13,
		-1.0 / 15,
		1.0 / 17,
		-1.0 / 19
		};

	double m = 0.0, p = x;
	for (int32_t i = 0; i < 10; i++) {
		m += coeffs[i] * p;
		p *= x * x;
	}

	return m;

#endif
}

/* @func: conch_asin
 * #desc:
 *    arc sine function.
 *
 * #1: x [in]  number
 * #r:   [ret] arc sine of x
 */
double conch_asin(double x)
{
	if (x < -1.0 || x > 1.0)
		return NAN;

	if (x == 1.0) {
		return M_PI / 2;
	} else if (x == -1.0) {
		return -M_PI / 2;
	}

	return conch_atan(x / conch_sqrt(1 - x * x));
}

/* @func: conch_exp
 * #desc:
 *    exponential function based on the natural logarithm.
 *
 * #1: x [in]  number
 * #r:   [ret] return the power of e^x
 */
double conch_exp(double x)
{
	if (x > 709.782712893384)
		return INFINITY;
	if (x < -745.1332191019411)
		return 0.0;

#if 0

	/* taylor expansion */
	double m = 1.0, q = 1.0;
	for (int32_t i = 1; i <= 25; i++) {
		q *= x / i;
		m += q;
	}

	return m;

#else

	/* minimax coefficients */
	static const double coeffs[13] = {
		1.00000000000000000000,
		1.00000000000000000000,
		0.50000000000000000000,
		0.16666666666666666667,
		0.04166666666666666667,
		0.00833333333333333333,
		0.00138888888888888889,
		0.00019841269841269841,
		0.00002480158730158730,
		0.00000275573192239859,
		0.00000027557319223986,
		0.00000002505210838544,
		0.00000000208767569879
		};

	const double ln2_hi = 0.69314718055994528623;
	const double ln2_lo = 2.31904681384629955842e-17;

	int32_t e = (int32_t)conch_round(x * (1 / M_LN2));
	double r = x - (ln2_hi * e) - (ln2_lo * e);

	/* remez + horner's method */
	x = coeffs[12];
	for (int32_t i = 11; i >= 0; i--)
		x = x * r + coeffs[i];

	return conch_ldexp(x, e);

#endif
}

/* @func: conch_exp2
 * #desc:
 *     base-2 exponential function.
 *
 * #1: x [in]  number
 * #r:   [ret] return the power of 2^x
 */
double conch_exp2(double x)
{
	return conch_exp(x * M_LN2);
}

/* @func: conch_log
 * #desc:
 *    natural logarithm function.
 *
 * #1: x [in]  number
 * #r:   [ret] natural logarithm of x
 */
double conch_log(double x)
{
	if (x <= 0.0)
		return NAN;
	if (x == 1)
		return 0.0;

#if 0

	/* newton-raphson method */
	double m = x, q = 0.0;
	for (int32_t i = 0; i < 15; i++) {
		q = conch_exp(m);
		m -= (q - x) / q;
	}

	return m;

#else

	int32_t e;
	double f = conch_frexp(x, &e);

	if (f < M_SQRT1_2) {
		f *= 2.0;
		e--;
	}

	/* minimax coefficients */
	static const double coeffs[8] = {
		2.00000000000000000000,
		0.66666666666666666667,
		0.40000000000000000000,
		0.28571428571428571429,
		0.22222222222222222222,
		0.18181818181818181818,
		0.15384615384615384615,
		0.13333333333333333333 
		};

	const double ln2_hi = 0.69314718055994528623;
	const double ln2_lo = 2.31904681384629955842e-17;

	double s = (f - 1.0) / (f + 1.0);
	double s2 = s * s;

	/* remez + horner's method */
	x = coeffs[7];
	for (int32_t i = 6; i >= 0; i--)
		x = x * s2 + coeffs[i];
	x *= s;

	return x + (ln2_hi * e) + (ln2_lo * e);

#endif
}

/* @func: conch_log2
 * #desc:
 *    base-2 natural logarithm function.
 *
 * #1: x [in]  number
 * #r:   [ret] base-2 natural logarithm of x
 */
double conch_log2(double x)
{
	if (x <= 0.0)
		return NAN;

	return conch_log(x) * (1 / M_LN2);
}

/* @func: conch_log10
 * #desc:
 *    base-10 natural logarithm function.
 *
 * #1: x [in]  number
 * #r:   [ret] base-10 natural logarithm of x
 */
double conch_log10(double x)
{
	if (x <= 0.0)
		return NAN;

	return conch_log(x) * (1 / M_LN10);
}

/* @func: conch_log1p
 * #desc:
 *    logarithm of 1 plus argument.
 *
 * #1: x [in]  number
 * #r:   [ret] natural logarithm of 1+x
 */
double conch_log1p(double x)
{
	if (x <= -1.0)
		return NAN;

	return conch_log(1.0 + x);
}

/* @func: conch_pow
 * #desc:
 *    power function.
 *
 * #1: x [in]  number
 * #2: e [in]  exponent
 * #r:   [ret] return the power of x 
 */
double conch_pow(double x, double e)
{
	if (e == 0.0)
		return 1.0;
	if (x == 1.0)
		return 1.0;
	if (x == 0.0) {
		if (e > 0.0)
			return 0.0;
		return INFINITY;
	}
	if (x < 0.0 && conch_floor(e) != e)
		return NAN;

	return conch_exp(e * conch_log(conch_fabs(x)));
}
