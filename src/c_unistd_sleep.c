/* @file: c_unistd_sleep.c
 * #desc:
 *    The implementations of unix standard.
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
#include <conch/c_time.h>
#include <conch/c_unistd.h>


/* @func: conch_sleep
 * #desc:
 *    sleep for a specified number of seconds.
 *
 * #1: n [in]  seconds
 * #r:   [ret] 0: no error, -1: errno
 */
int32_t conch_sleep(uint32_t n)
{
	struct xtimespec a, b;
	a.tv_sec = n;
	a.tv_nsec = 0;

	if (conch_nanosleep(&a, &b))
		return -1;

	return b.tv_sec;
}
