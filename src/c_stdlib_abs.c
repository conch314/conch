/* @file: c_stdlib_abs.c
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
#include <conch/c_stdlib.h>


/* @func: conch_abs
 * #desc;
 *    return an integer absolute value.
 *
 * #1: n [in]  number
 * #r:   [ret] absolute value
 */
int conch_abs(int n)
{
	return (n < 0) ? -n : n;
}

/* @func: conch_labs
 * #desc;
 *    return an integer absolute value.
 *
 * #1: n [in]  number
 * #r:   [ret] absolute value
 */
long conch_labs(long n)
{
	return (n < 0) ? -n : n;
}

/* @func: conch_llabs
 * #desc;
 *    return an integer absolute value.
 *
 * #1: n [in]  number
 * #r:   [ret] absolute value
 */
long long conch_llabs(long long n)
{
	return (n < 0) ? -n : n;
}
