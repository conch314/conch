/* @file: c_stdlib_atof.c
 * #desc:
 *    The implementations of standard library.
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
#include <conch/c_stdlib.h>
#include <conch/c_math.h>


/* @func: _strtod_num (static)
 * #desc:
 *    string to floating-point (integer) conversion.
 *
 * #1: s [in]  input string
 * #2: e [out] end pointer / NULL
 * #3: b [in]  base type
 * #r:   [ret] return double
 */
static double _strtod_num(const char *s, char **e, int32_t b)
{
	double x = 0.0;
	for (int32_t d = 0; *s != '\0'; s++) {
		if (*s >= '0' && *s <= '9') {
			d = (*s - '0');
		} else if (*s >= 'A' && *s <= 'Z') {
			d = (*s - 'A') + 10;
		} else if (*s >= 'a' && *s <= 'z') {
			d = (*s - 'a') + 10;
		} else {
			break;
		}
		if (d >= b)
			break;

		x = (x * b) + d;
	}

	if (e)
		*e = (char *)s;

	return x;
}

/* @func: _strtod (static)
 * #desc:
 *    string to floating-point conversion.
 *
 * #1: s [in]  input string
 * #2: e [out] end pointer / NULL
 * #r:   [ret] return double
 */
static double _strtod(const char *s, char **e)
{
	for (; *s == ' ' || *s == '\t'; s++);

	int32_t neg = 0, b = 10, n;
	if (*s == '-' || *s == '+') /* negative */
		neg = (*s++ == '-') ? 1 : 0;

	if (*s == '0') {
		s++;
		if (*s == 'X' || *s == 'x') {
			b = 16;
			s++;
		}
	}

	char *ee = NULL;
	if (!e)
		e = &ee;

	/* integer and decimals */
	double x = _strtod_num(s, e, b);
	s = *e;
	if (*s == '.') {
		s++;
		double p = _strtod_num(s, e, b);
		x += p / conch_pow(b, (uint32_t)(*e - s));
	}

	/* exponent */
	s = *e;
	if (b == 10 && (*s == 'E' || *s == 'e')) {
		s++;
		n = (int32_t)conch_strtol(s, e, 10);
		x *= conch_pow(10, n);
	} else if (b == 16 && (*s == 'P' || *s == 'p')) {
		s++;
		n = (int32_t)conch_strtol(s, e, 10);
		x *= conch_pow(2, n);
	}

	return neg ? -x : x;
}

/* @func: conch_atof
 * #desc:
 *    string to float conversion.
 *
 * #1: s [in]  input string
 * #r:   [ret] return double
 */
double conch_atof(const char *s)
{
	return _strtod(s, NULL);
}

/* @func: conch_strtod
 * #desc:
 *    string to double conversion.
 *
 * #1: s [in]  input string
 * #2: e [out] end pointer / NULL
 * #r:   [ret] return double
 */
double conch_strtod(const char *s, char **e)
{
	return _strtod(s, e);
}
