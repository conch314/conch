/* @file: c_stdio_printf.c
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
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>
#include <conch/c_stdarg.h>
#include <conch/c_stdio.h>


struct out_ctx {
	xFILE *fp;
	size_t len;
};

static int32_t _call_out(const char *s, int32_t len, void *arg)
{
	struct out_ctx *ctx = arg;
	size_t r;

	if (len) {
		r = conch_fwrite(s, 1, len, ctx->fp);
		if (r != len && conch_ferror(ctx->fp))
			return -1;
		ctx->len += len;
	}

	return 0;
}


/* @func: conch_vfprintf
 * #desc:
 *    formatted output conversion.
 *
 * #1: fp  [in/out] stdio file struct
 * #2: fmt [in]     format string
 * #3: ap  [in]     variable parameter
 * #r:     [ret]    >=0: output length, -1: errno
 */
int32_t conch_vfprintf(xFILE *fp, const char *fmt, va_list ap)
{
	struct out_ctx ctx = { fp, 0 };
	va_list _ap;
	va_copy(_ap, ap);

	if (__conch_printf(fmt, &_ap, &ctx, _call_out)) {
		va_end(_ap);
		return -1;
	}

	va_end(_ap);

	return ctx.len;
}

/* @func: conch_fprintf
 * #desc:
 *    formatted output conversion.
 *
 * #1: fp  [in/out] stdio file struct
 * #2: fmt [in]     format string
 * #N:     [in]     variable parameter
 * #r:     [ret]    >=0: output length, -1: errno
 */
int32_t conch_fprintf(xFILE *fp, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	int32_t ret = conch_vfprintf(fp, fmt, ap);
	va_end(ap);

	return ret;
}

/* @func: conch_vprintf
 * #desc:
 *    formatted output conversion.
 *
 * #1: fmt [in]  format string
 * #2: ap  [in]  variable parameter
 * #r:     [ret] >=0: output length, -1: errno
 */
int32_t conch_vprintf(const char *fmt, va_list ap)
{
	va_list _ap;
	va_copy(_ap, ap);

	int32_t ret = conch_vfprintf(x_stdout, fmt, _ap);
	va_end(_ap);

	return ret;
}

/* @func: conch_printf
 * #desc:
 *    formatted output conversion.
 *
 * #1: fmt [in]  format string
 * #N:     [in]  variable parameter
 * #r:     [ret] >=0: output length, -1: errno
 */
int32_t conch_printf(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	int32_t ret = conch_vfprintf(x_stdout, fmt, ap);
	va_end(ap);

	return ret;
}
