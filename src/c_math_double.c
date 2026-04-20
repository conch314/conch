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
	u.i &= ~(m - 1); /* mask decimals part */

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
	u.i &= ~(m - 1); /* mask decimals part */

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

	*e = ee - 1022; /* [0.5, 1.0] */
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

/* @func: _erf_horner (static)
 * #desc:
 *   remez + horner's method of error function polynomial.
 *
 * #1: r [in]  number
 * #2: n [in]  choose polynomial constant
 * #r:   [ret] return the horner polynomial of x
 */
static double _erf_horner(double r, int32_t n)
{
	/* [0, 0.84375] */
	static const double p_coeffs_1[5] = {
		 1.28379167095512558561e-01,
		-3.25042107247001499370e-01,
		-2.84817495755985104766e-02,
		-5.77027029648944159157e-03,
		-2.37630166566501626084e-05,
		};
	static const double q_coeffs_1[6] = {
		 1.00000000000000000000e+00,
		 3.97917223959155352819e-01,
		 6.50222499887672944485e-02,
		 5.08130628187576562776e-03,
		 1.32494738004321644526e-04,
		-3.96022827877536812320e-06,
		};
	/* [0.84375, 1.25] */
	static const double p_coeffs_2[7] = {
		-2.36211856075265944077e-03,
		 4.14856118683748331666e-01,
		-3.72207876035701323847e-01,
		 3.18346619901161753674e-01,
		-1.10894694282396677476e-01,
		 3.54783043256182359371e-02,
		-2.16637559486879084300e-03
		};
	static const double q_coeffs_2[7] = {
		1.00000000000000000000e+00,
		1.06420880400844228286e-01,
		5.40397917702171048937e-01,
		7.18286544141962662868e-02,
		1.26171219808761642112e-01,
		1.36370839120290507362e-02,
		1.19844998467991074170e-02
		};
	/* [1.25, 1/0.35] */
	static const double p_coeffs_3[8] = {
		-9.86494403484714822705e-03,
		-6.93858572707181764372e-01,
		-1.05586262253232909814e+01,
		-6.23753324503260060396e+01,
		-1.62396669462573470355e+02,
		-1.84605092906711035994e+02,
		-8.12874355063065934246e+01,
		-9.81432934416914548592e+00
		};
	static const double q_coeffs_3[9] = {
		 1.00000000000000000000e+00,
		 1.96512716674392571292e+01,
		 1.37657754143519042600e+02,
		 4.34565877475229228821e+02,
		 6.45387271733267880336e+02,
		 4.29008140027567833386e+02,
		 1.08635005541779435134e+02,
		 6.57024977031928170135e+00,
		-6.04244152148580987438e-02
		};
	/* [1/.35, 28] */
	static const double p_coeffs_4[7] = {
		-9.86494292470009928597e-03,
		-7.99283237680523006574e-01,
		-1.77579549177547519889e+01,
		-1.60636384855821916062e+02,
		-6.37566443368389627722e+02,
		-1.02509513161107724954e+03,
		-4.83519191608651397019e+02
		};
	static const double q_coeffs_4[8] = {
		 1.00000000000000000000e+00,
		 3.03380607434824582924e+01,
		 3.25792512996573918826e+02,
		 1.53672958608443695994e+03,
		 3.19985821950859553908e+03,
		 2.55305040643316442583e+03,
		 4.74528541206955367215e+02,
		-2.24409524465858183362e+01
		};

	double p, q;
	switch (n) {
		case 1: /* x < 0.84375 */
			p = p_coeffs_1[4];
			for (int32_t i = 3; i >= 0; i--)
				p = p * r + p_coeffs_1[i];
			q = q_coeffs_1[5];
			for (int32_t i = 4; i >= 0; i--)
				q = q * r + q_coeffs_1[i];
			break;
		case 2: /* x < 1.25 */
			p = p_coeffs_2[6];
			for (int32_t i = 5; i >= 0; i--)
				p = p * r + p_coeffs_2[i];
			q = q_coeffs_2[6];
			for (int32_t i = 5; i >= 0; i--)
				q = q * r + q_coeffs_2[i];
			break;
		case 3: /* x < 1/.35 ~ 2.85714 */
			p = p_coeffs_3[7];
			for (int32_t i = 6; i >= 0; i--)
				p = p * r + p_coeffs_3[i];
			q = q_coeffs_3[8];
			for (int32_t i = 6; i >= 0; i--)
				q = q * r + q_coeffs_3[i];
			break;
		case 4: /* x > 1/.35 */
			p = p_coeffs_4[6];
			for (int32_t i = 5; i >= 0; i--)
				p = p * r + p_coeffs_4[i];
			q = q_coeffs_4[7];
			for (int32_t i = 6; i >= 0; i--)
				q = q * r + q_coeffs_4[i];
		default:
			return NAN;
	}

	return p / q;
}

/* @func: conch_erf
 * #desc:
 *    error function.
 *
 * #1: x [in]  number
 * #r:   [ret] return the error function of x
 */
double conch_erf(double x)
{
	union {
		double f;
		uint64_t i;
	} u = { x };

	uint32_t ix = (u.i >> 32) & 0x7fffffff;
	uint32_t sign = u.i >> 63;
	double pq, y, r;

	if (ix >= 0x7ff00000) /* nan=>nan, +-inf=>+-1 */
		return (2 * sign) + (1.0 / x);

	if (ix < 0x3feb0000) { /* x < 0.84375 */
		if (ix < 0x3e300000) {  /* |x| < 2^-28 */
			if (ix < 0x00800000)
				return (8 * x + 1.0270333367641007 * x) / 8;
			return x + 0.1283791670955126 * x;
		}

		r = x * x;
		pq = _erf_horner(r, 1);
		return x + x * pq;
	}

	y = 0x1p-1022;
	if (ix < 0x3ff40000) { /* x < 1.25 */
		r = conch_fabs(x) - 1.0;
		pq = _erf_horner(r, 2);
		y = 1.0 - 0.8450629115104675 - pq;
	} else if (ix < 0x40180000) { /* x < 6 */
		x = conch_fabs(x);
		r = 1.0 / (x * x);

		if (ix < 0x4006db6d) { /* x < 1/.35 ~ 2.85714 */
			pq = _erf_horner(r, 3);
		} else { /* x > 1/.35 */
			pq = _erf_horner(r, 4);
		}

		u.f = x;
		u.i &= ~0xffffffff;
		y = conch_exp(-u.f * u.f - 0.5625)
			* conch_exp((u.f - x) * (u.f + x) + pq) / x;
	}
	y = 1.0 - y;

	return sign ? -y : y;
}

/* @func: conch_erfc
 * #desc:
 *    complementary error function.
 *
 * #1: x [in]  number
 * #r:   [ret] return the complementary error of x
 */
double conch_erfc(double x)
{
	union {
		double f;
		uint64_t i;
	} u = { x };

	uint32_t ix = (u.i >> 32) & 0x7fffffff;
	uint32_t sign = u.i >> 63;
	double pq, y, r;

	if (ix >= 0x7ff00000) /* nan=>nan, +-inf=>+-0,2 */
		return (2.0 * sign) + (1.0 / x);

	if (ix < 0x3feb0000) { /* x < 0.84375 */
		if (ix < 0x3c700000)  /* x < 2^-56 */
			return 1.0 - x;

		r = x * x;
		pq = _erf_horner(r, 1);
		if (sign || ix < 0x3fd00000) { /* x < 1/4 */
			return 1.0 - (x + x * pq);
		} else {
			return 0.5 - (x - 0.5 + x * pq);
		}
	}

	if (ix < 0x403c0000) {  /* x < 28 */
		if (ix < 0x3ff40000) { /* x < 1.25 */
			r = conch_fabs(x) - 1.0;
			pq = _erf_horner(r, 2);
			y = 1.0 - 0.8450629115104675 - pq;
		} else { /* x < 28 */
			x = conch_fabs(x);
			r = 1.0 / (x * x);

			if (ix < 0x4006db6d) { /* x < 1/.35 ~ 2.85714 */
				pq = _erf_horner(r, 3);
			} else { /* x > 1/.35 */
				pq = _erf_horner(r, 4);
			}

			u.f = x;
			u.i &= ~0xffffffff;
			y = conch_exp(-u.f * u.f - 0.5625)
				* conch_exp((u.f - x) * (u.f + x) + pq) / x;
		}

		return sign ? (2.0 - y) : y;
	}

	return sign ? (2.0 - 0x1p-1022) : (0x1p-1022 * 0x1p-1022);
}
