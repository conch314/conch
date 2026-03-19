/* @file: c_math.h
 * #desc:
 *    The definitions of mathematical declarations.
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

#ifndef _CONCH_C_MATH_H
#define _CONCH_C_MATH_H

#include <conch/config.h>
#include <conch/c_stdint.h>


#undef M_E
#undef M_LOG2E
#undef M_LOG10E
#undef M_LN2
#undef M_LN10
#undef M_PI
#undef M_PI_2
#undef M_PI_4
#undef M_1_PI
#undef M_2_PI
#undef M_2_SQRTPI
#undef M_SQRT2
#undef M_SQRT1_2

/* logarithm e */
#define M_E 2.7182818284590452354
/* log2(e) */
#define M_LOG2E 2.7182818284590452354
/* log10(e) */
#define M_LOG10E 0.43429448190325182765
/* log(2) */
#define M_LN2 0.69314718055994530942
/* log(10) */
#define M_LN10 2.30258509299404568402
/* pi */
#define M_PI 3.14159265358979323846
/* pi/2 */
#define M_PI_2 1.57079632679489661923
/* pi/4 */
#define M_PI_4 0.78539816339744830962
/* 1/pi */
#define M_1_PI 0.31830988618379067154
/* 2/pi */
#define M_2_PI 0.63661977236758134308
/* 2/sqrt(pi) */
#define M_2_SQRTPI 1.12837916709551257390
/* square root of 2 */
#define M_SQRT2 1.41421356237309504880
/* 1/sqrt(2) */
#define M_SQRT1_2 0.70710678118654752440

#define X_FP_NAN 0
#define X_FP_INFINITE 1
#define X_FP_ZERO 2
#define X_FP_SUBNORMAL 3
#define X_FP_NORMAL 4

static const union {
	float f;
	uint32_t i;
} _ZERO = { .i = 0x0 };

static const union {
	float f;
	uint32_t i;
} _NAN = { .i = 0x7fc00000 };

static const union {
	float f;
	uint32_t i;
} _INFINITY = { .i = 0x7f800000 };

#undef ZERO
#undef NAN
#undef INFINITY
#define ZERO (_ZERO.f)
#define NAN (_NAN.f)
#define INFINITY (_INFINITY.f)


#ifdef __cplusplus
extern "C" {
#endif

/* c_math_double.c */
extern
int32_t conch_fpclassify(double x)
;
extern
double conch_fabs(double x)
;
extern
double conch_floor(double x)
;
extern
double conch_fmod(double x, double y)
;
extern
double conch_frexp(double x, int32_t *e)
;
extern
double conch_modf(double x, double *n)
;
extern
double conch_ldexp(double x, int32_t n)
;
extern
double conch_cos(double x)
;
extern
double conch_sin(double x)
;
extern
double conch_tan(double x)
;
extern
double conch_acos(double x)
;
extern
double conch_atan(double x)
;
extern
double conch_sqrt(double x)
;
extern
double conch_asin(double x)
;
extern
double conch_exp(double x)
;
extern
double conch_log(double x)
;
extern
double conch_log2(double x)
;
extern
double conch_log10(double x)
;
extern
double conch_log1p(double x)
;
extern
double conch_pow(double x, double e)
;

#ifdef __cplusplus
}
#endif


#endif
