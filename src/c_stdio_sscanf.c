/* @file: c_stdio_sscanf.c
 * #desc:
 *    The implementations of standard input/output.
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
#include <conch/c_stdarg.h>
#include <conch/c_stdio.h>


static int32_t _call_get(int32_t peek, void *arg)
{
	const char **p = (const char **)arg;

	return peek ? **p : *((*p)++);
}


/* @func: conch_vsscanf
 * #desc:
 *    input string formatted value conversion.
 *
 * #1: s   [in]  input string
 * #2: fmt [in]  format string
 * #3: ap  [out] variable parameter
 * #r:     [ret] >0: no error, 0: error
 */
int32_t conch_vsscanf(const char *s, const char *fmt, va_list ap)
{
	const char *p = s;
	va_list _ap;
	va_copy(_ap, ap);

	int32_t ret = __conch_scanf(fmt, &_ap, (void *)&p, _call_get);
	if (ret < 0) {
		va_end(_ap);
		return -ret;
	}

	va_end(_ap);

	return ret;
}

/* @func: conch_sscanf
 * #desc:
 *    input string formatted value conversion.
 *
 * #1: s   [in]  input string
 * #2: fmt [in]  format string
 * #N:     [out] variable parameter
 * #r:     [ret] >0: no error, 0: error
 */
int32_t conch_sscanf(const char *s, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	int32_t ret = conch_vsscanf(s, fmt, ap);
	va_end(ap);

	return ret;
}
