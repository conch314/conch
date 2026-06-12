/* @file: c_float.h
 * #desc:
 *    The definitions of floating types.
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

#ifndef _CONCH_C_FLOAT_H
#define _CONCH_C_FLOAT_H

#include <conch/config.h>


/* base */
#undef FLT_RADIX
#define FLT_RADIX 2

/* IEEE float */

/* mantissa */
#undef FLT_MANT_DIG
#define FLT_MANT_DIG 24
/* exact number of significant digits */
#undef FLT_DIG
#define FLT_DIG 6
/* accuracy error range */
#undef FLT_EPSILON
#define FLT_EPSILON 1.19209290e-7
/* exponent */
#undef FLT_MIN_EXP
#undef FLT_MAX_EXP
#undef FLT_MIN_10_EXP
#undef FLT_MAX_10_EXP
#define FLT_MIN_EXP (-125)
#define FLT_MAX_EXP 128
#define FLT_MIN_10_EXP (-37)
#define FLT_MAX_10_EXP 38
/* size */
#undef FLT_MIN
#undef FLT_MAX
#define FLT_MIN 1.17549435e-38
#define FLT_MAX 3.40282347e+38

/* IEEE double */

/* mantissa */
#undef DBL_MANT_DIG
#define DBL_MANT_DIG 53
/* exact number of significant digits */
#undef DBL_DIG
#define DBL_DIG 15
/* accuracy error range */
#undef DBL_EPSILON
#define DBL_EPSILON 2.2204460492503131e-16
/* exponent */
#undef DBL_MIN_EXP
#undef DBL_MAX_EXP
#undef DBL_MIN_10_EXP
#undef DBL_MAX_10_EXP
#define DBL_MIN_EXP (-1021)
#define DBL_MAX_EXP 1024
#define DBL_MIN_10_EXP (-307)
#define DBL_MAX_10_EXP 308
/* size */
#undef DBL_MIN
#undef DBL_MAX
#define DBL_MIN 2.2250738585072014e-308
#define DBL_MAX 1.7976931348623157e+308


#endif
