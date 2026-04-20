/* @file: c_stdio_snprintf.c
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
#include <conch/c_string.h>
#include <conch/c_stdio.h>


struct out_ctx {
	char *p;
	char *e;
	size_t len;
};

static int32_t _call_out(const char *s, int32_t len, void *arg)
{
	struct out_ctx *ctx = arg;

	if (len) {
		ctx->len += len;
		if (ctx->p < ctx->e) {
			if ((ctx->p + len) > ctx->e)
				len = (size_t)(ctx->e - ctx->p);

			conch_memcpy(ctx->p, s, len);
			ctx->p += len;
		} else {
			return -1;
		}
	}

	return 0;
}


/* @func: conch_vsnprintf
 * #desc:
 *    convert formatted output to the buffer.
 *
 * #1: buf [out] output buffer
 * #2: len [in]  buffer length
 * #3: fmt [in]  format string
 * #4: ap  [in]  variable parameter
 * #r:     [ret] >=0: output length, -1: errno
 */
int32_t conch_vsnprintf(char *buf, size_t len, const char *fmt, va_list ap)
{
	if (!len)
		return 0;

	struct out_ctx ctx = {
		.p = buf, .e = buf + len - 1,
		.len = 0
		};
	va_list _ap;
	va_copy(_ap, ap);

	if (__conch_printf(fmt, &_ap, &ctx, _call_out)) {
		va_end(_ap);
		return -1;
	}

	va_end(_ap);

	return ctx.len;
}

/* @func: conch_snprintf
 * #desc:
 *    convert formatted output to the buffer.
 *
 * #1: buf [out] output buffer
 * #2: len [in]  buffer length
 * #3: fmt [in]  format string
 * #N:     [in]  variable parameter
 * #r:     [ret] >=0: output length, -1: errno
 */
int32_t conch_snprintf(char *buf, size_t len, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	int32_t ret = conch_vsnprintf(buf, len, fmt, ap);
	va_end(ap);

	return ret;
}
