/* @file: c_stdio_scanf.c
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


struct getc_ctx {
	xFILE *fp;
	int32_t st;
	int32_t c;
};

static int32_t _call_getc(int32_t peek, void *arg)
{
	struct getc_ctx *ctx = arg;
	int32_t c;

	switch (ctx->st) {
		case 0:
			ctx->c = c = conch_fgetc(ctx->fp);
			if (peek)
				ctx->st = 1;
			break;
		case 1:
			c = ctx->c;
			if (!peek)
				ctx->st = 0;
			break;
		default:
			return -1;
	}

	return c;
}


/* @func: conch_vfscanf
 * #desc:
 *    input string formatted value conversion.
 *
 * #1: fp  [in]  stdio file struct
 * #2: fmt [in]  format string
 * #3: ap  [out] variable parameter
 * #r:     [ret] >0: no error, 0: error
 */
int32_t conch_vfscanf(xFILE *fp, const char *fmt, va_list ap)
{
	struct getc_ctx ctx = { fp, 0, 0 };
	va_list _ap;
	va_copy(_ap, ap);

	int32_t ret = __conch_scanf(fmt, &ap, &ctx, _call_getc);
	if (ret < 0) {
		va_end(_ap);
		return -ret;
	}

	va_end(_ap);

	return ret;
}

/* @func: conch_vscanf
 * #desc:
 *    input string formatted value conversion.
 *
 * #1: fmt [in]  format string
 * #2: ap  [out] variable parameter
 * #r:     [ret] >0: no error, 0: error
 */
int32_t conch_vscanf(const char *fmt, va_list ap)
{
	va_list _ap;
	va_copy(_ap, ap);

	int32_t ret = conch_vfscanf(x_stdin, fmt, _ap);
	va_end(_ap);

	return ret;
}

/* @func: conch_scanf
 * #desc:
 *    input string formatted value conversion.
 *
 * #1: fmt [in]  format string
 * #N:     [out] variable parameter
 * #r:     [ret] >0: no error, 0: error
 */
int32_t conch_scanf(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	int32_t ret = conch_vfscanf(x_stdin, fmt, ap);
	va_end(ap);

	return ret;
}
