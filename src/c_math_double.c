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
 * #1: x [in]  input value
 * #r:   [ret] floating-point type
 */
int32_t conch_fpclassify(double x)
{
	union {
		double f;
		uint64_t i;
	} u = { x };
	uint64_t e = (u.i >> 52) & 0x7ff;
	uint64_t f = u.i & 0xfffffffffffffULL;

	if (!e)
		return f ? X_FP_SUBNORMAL : X_FP_ZERO;
	if (e == 0x7ff)
		return f ? X_FP_NAN : X_FP_INFINITE;

	return X_FP_NORMAL;
}

/* @func: conch_fabs
 * #desc:
 *    return an floating-point absolute value.
 *
 * #1: x [in]  input value
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

/* @func: conch_floor
 * #desc:
 *    compute the maximum integer value of x.
 *
 * #1: x [in]  input value
 * #r:   [ret] maximum integer value
 */
double conch_floor(double x)
{
	union {
		double f;
		uint64_t i;
	} u = { x };
	uint32_t e = ((u.i >> 52) & 0x7ff) - 1023;

	if (e > 51)
		return x;
	if (e < 0)
		return (u.i << 63) ? -1.0 : 0.0;

	uint64_t m = 1ULL << (52 - e);
	if (!(u.i & (m - 1)))
		return x;

	if (x < 0.0)
		u.i += m;
	u.i &= ~(m - 1);

	return u.f;
}

/* @func: conch_fmod
 * #desc:
 *    remainder function.
 *
 * #1: x [in]  input value
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
 *    extract the mantissa and exponent from floating-point.
 *
 * #1: x [in]  input value
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

/* @func: conch_modf
 * #desc:
 *    extract floating-point integer and decimals
 *
 * #1: x [in]  input value
 * #2: n [out] integer
 * #r:   [ret] decimals
 */
double conch_modf(double x, double *n)
{
	union {
		double f;
		uint64_t i;
	} u = { x };
	uint32_t e = ((u.i >> 52) & 0x7ff) - 1023;

	if (e > 51) {
		*n = x;
		return 0.0;
	}
	if (e < 0) {
		*n = 0.0;
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

/* @func: conch_ldexp
 * #desc:
 *    multiply by an integer power of 2.
 *
 * #1: x [in]  input floating-point
 * #2: n [in]  exponent
 * #r:   [ret] return the x*2^n
 */
double conch_ldexp(double x, int32_t n)
{
	union {
		double f;
		uint64_t i;
	} u = { x };

	if (n > 1023) {
		x *= 0x1p1023;
		n -= 1023;
		if (n > 1023) {
			x *= 0x1p1023;
			n -= 1023;
			if (n > 1023)
				n = 1023;
		}
	} else if (n < -1022) {
		x *= 0x1p-1022 * 0x1p53;
		n += 1022 - 53;
		if (n < -1022) {
			x *= 0x1p-1022 * 0x1p53;
			n += 1022 - 53;
			if (n < -1022)
				n = -1022;
		}
	}

	u.i = (uint64_t)(1023 + n) << 52;
	x *= u.f;

	return x;
}

/* @func: conch_cos
 * #desc:
 *    cosine function.
 *
 * #1: x [in]  input value
 * #r:   [ret] cosine of value
 */
double conch_cos(double x)
{
	int32_t dneg = 1;
	x = conch_fmod(x, M_PI * 2);
	if (x > M_PI) {
		x -= M_PI;
		dneg = -1;
	}

	/* taylor series running product */
	int32_t sign = -1;
	double m = 1.0, q = 1.0, x2 = x * x;
	for (int32_t i = 1; i <= 10; i++) {
		q *= x2 / ((2 * i) * (2 * i - 1.0));
		m += sign * q;
		sign *= -1;
	}

	return m * dneg;
}

/* @func: conch_sin
 * #desc:
 *    sine function.
 *
 * #1: x [in]  input value
 * #r:   [ret] sine of value
 */
double conch_sin(double x)
{
	int32_t dneg = 1;
	x = conch_fmod(x, M_PI * 2);
	if (x > M_PI) {
		x -= M_PI;
		dneg = -1;
	}

	/* taylor series running product */
	int32_t sign = -1;
	double m = x, q = x, x2 = x * x;
	for (int32_t i = 1; i <= 10; i++) {
		q *= x2 / ((2 * i) * (2 * i + 1));
		m += sign * q;
		sign *= -1;
	}

	return m * dneg;
}

/* @func: conch_tan
 * #desc:
 *    tangent function.
 *
 * #1: x [in]  input value
 * #r:   [ret] tangent of value
 */
double conch_tan(double x)
{
	return conch_sin(x) / conch_cos(x);
}

/* @func: conch_acos
 * #desc:
 *    arc cosine function.
 *
 * #1: x [in]  input value
 * #r:   [ret] arc cosine of value
 */
double conch_acos(double x)
{
	if (x < -1.0 || x > 1.0)
		return NAN;

	double m = M_PI_2;
	for (int32_t i = 0; i < 5; i++)
		m -= (conch_cos(m) - x) / (-conch_sin(m));

	return m;
}

/* @func: conch_atan
 * #desc:
 *    arc tangent function.
 *
 * #1: x [in]  input value
 * #r:   [ret] arc tangent of value
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

#endif

	return m;
}

/* @func: conch_sqrt
 * #desc:
 *    square root function.
 *
 * #1: x [in]  input value
 * #r:   [ret] square root of value
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

#endif

	return m;
}

/* @func: conch_asin
 * #desc:
 *    arc cosine function.
 *
 * #1: x [in]  input value
 * #r:   [ret] arc cosine of value
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
 *    exponential function.
 *
 * #1: x [in]  input value
 * #r:   [ret] cardinality exponent of value
 */
double conch_exp(double x)
{
	x *= 1 / M_LN2;
	int32_t n = (int32_t)x;
	x -= n;

	/* horner + remez poly */
	double m = 0.00133335581;
	m = m * x + 0.00961812911;
	m = m * x + 0.05550410866;
	m = m * x + 0.24022650695;
	m = m * x + 0.69314718056;
	m = m * x + 1.0;

	return conch_ldexp(m, n);
}

/* @func: conch_log
 * #desc:
 *    natural logarithm function.
 *
 * #1: x [in]  input value
 * #r:   [ret] natural logarithm of value
 */
double conch_log(double x)
{
	if (x <= 0.0)
		return NAN;
	if (x == 1)
		return 0.0;

	int32_t e;
	x = conch_frexp(x, &e);

	if (x < M_SQRT1_2) {
		x *= 2.0;
		e -= 1;
	}
	x -= 1.0;

	/* horner + remez poly */
	double m = 0.14218894;
	m = m * x + -0.16679639;
	m = m * x + 0.19992875;
	m = m * x + -0.24997585;
	m = m * x + 0.33333736;
	m = m * x + -0.49999908;
	m = m * x + 0.99999955;
	m = m * x + -0.00027870118;

	m += e * M_LN2;

	return m;
}

/* @func: conch_log2
 * #desc:
 *    natural logarithm function of base 2.
 *
 * #1: x [in]  input value
 * #r:   [ret] natural logarithm of value
 */
double conch_log2(double x)
{
	return conch_log(x) / M_LN2;
}

/* @func: conch_log10
 * #desc:
 *    natural logarithm function of base 10.
 *
 * #1: x [in]  input value
 * #r:   [ret] natural logarithm of value
 */
double conch_log10(double x)
{
	return conch_log(x) / M_LN10;
}

/* @func: conch_log1p
 * #desc:
 *    logarithm of 1 plus argument.
 *
 * #1: x [in]  input value
 * #r:   [ret] natural logarithm of value
 */
double conch_log1p(double x)
{
	if (x <= -1.0)
		return NAN;

	return conch_log(1.0 + x);
}

/* @func: _pow (static)
 * #desc:
 *    power function.
 *
 * #1: x [in]  input value
 * #2: e [in]  exponent value
 * #r:   [ret] return x^e power value
 */
static double _pow(double x, double e)
{
	double m = 1.0;
	uint64_t abs_e = (uint64_t)conch_fabs(e);
	while (abs_e > 0) {
		if (abs_e % 2 == 1)
			m *= x;
		abs_e >>= 1;
		x *= x;
	}
	m = (e < 0.0) ? (1.0 / m) : m;

	return m;
}

/* @func: conch_pow
 * #desc:
 *    power function.
 *
 * #1: x [in]  input value
 * #2: e [in]  exponent value
 * #r:   [ret] return x^e power value
 */
double conch_pow(double x, double e)
{
	double m = 0.0, q = 0.0;
	m = _pow(conch_fabs(x), e);

	q = e - (int64_t)e;
	if (q != 0.0)
		m *= conch_exp(q * conch_log(x));

	return m;
}
