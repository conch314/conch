/* @file: c_stdlib_rand.c
 * #desc:
 *    The implementations of standard library.
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
#include <conch/c_stdint.h>
#include <conch/c_stdlib.h>


/* @func: conch_rand_r
 * #desc:
 *    pseudo-random number generator.
 *
 * #1: state [in/out] seed state
 * #r:       [ret]    random number
 */
int32_t conch_rand_r(int32_t *state)
{
	int32_t ret;

	/* LCG (Linear Congruential Generator) */
	ret = ((*state * 1103515245) + 12345) & INT32_MAX;
	*state = ret;

	return ret;
}
