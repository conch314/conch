/* @file: c_stdio__scanf.c
 * #desc:
 *    The implementations of scanf functions internal.
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
#include <conch/c_string.h>
#include <conch/c_stdlib.h>
#include <conch/c_stdio.h>
#include <conch/c_limits.h>
#include <conch/c_math.h>


struct scanf_ctx {
	char specifiers;
	int32_t flags;
	int32_t buf_len;
	void *arg;
	/* is-peek, arg */
	int32_t (*get)(int32_t, void *);
};

#define FG_LONG 0x01
#define FG_LONG_LONG 0x02
#define FG_SHORT 0x04
#define FG_CHAR 0x08
#define FG_LONG_DOUBLE 0x10
#define FG_SKIP 0x20
#define SC_SPACE " \t\n"


/* @func: _scanf_di (static)
 * #desc:
 *    scanf integer (signed decimal/hexadecimal) format.
 *
 * #1: ctx [in]  scanf struct context
 * #2: ap  [out] variable parameter pointer
 * #r:     [ret] 0: no error, -1: error
 */
static int32_t _scanf_di(struct scanf_ctx *ctx, va_list *ap)
{
	uint64_t m, n = 0;
	int32_t c, d = 0, b = 10, neg = 0, st = 0;

	if (ctx->flags & FG_LONG) {
		m = LONG_MAX;
	} else if (ctx->flags & FG_LONG_LONG) {
		m = LLONG_MAX;
	} else if (ctx->flags & FG_SHORT) {
		m = SHRT_MAX;
	} else if (ctx->flags & FG_CHAR) {
		m = CHAR_MAX;
	} else {
		m = INT_MAX;
	}

	/* skip space */
	while (1) {
		c = ctx->get(1, ctx->arg);
		if (!conch_strchr(SC_SPACE, c))
			break;
		ctx->get(0, ctx->arg);
	}

	c = ctx->get(1, ctx->arg);
	if (c == '-' || c == '+') {
		neg = (c == '-') ? 1 : 0;
		ctx->get(0, ctx->arg);
	}

	/* prefix */
	c = ctx->get(1, ctx->arg);
	if (c == '0') {
		ctx->get(0, ctx->arg);
		c = ctx->get(1, ctx->arg);
		if (c == 'X' || c == 'x') {
			b = 16;
			ctx->get(0, ctx->arg);
		} else if (c >= '0' && c <= '7') {
			b = 8;
			ctx->get(0, ctx->arg);
		}
		if (ctx->specifiers == 'd' && b != 10)
			return -1;
	}

	while (1) {
		c = ctx->get(1, ctx->arg);
		if (c != '0')
			break;
		ctx->get(0, ctx->arg);
	}

	/* convert to number */
	while (1) {
		c = ctx->get(1, ctx->arg);
		if (c == '\0')
			break;
		if (st && conch_strchr(SC_SPACE, c))
			break;

		if (c >= '0' && c <= '9') {
			d = (c - '0');
		} else if (c >= 'A' && c <= 'F') {
			d = (c - 'A') + 10;
		} else if (c >= 'a' && c <= 'f') {
			d = (c - 'a') + 10;
		} else {
			return -1;
		}
		if (d >= b)
			return -1;

		if (n > ((m - d) / b))
			m = 0;
		n = (n * b) + d;

		ctx->get(0, ctx->arg);
		st = 1;
	}

	if (ctx->flags & FG_SKIP)
		return 0;

	n = m ? (neg ? -n : n) : 0;

	if (ctx->flags & FG_LONG) {
		*va_arg(*ap, long *) = (long)n;
	} else if (ctx->flags & FG_LONG_LONG) {
		*va_arg(*ap, long long *) = (long long)n;
	} else if (ctx->flags & FG_SHORT) {
		*va_arg(*ap, short *) = (short)n;
	} else if (ctx->flags & FG_CHAR) {
		*va_arg(*ap, char *) = (char)n;
	} else {
		*va_arg(*ap, int *) = (int)n;
	}

	return 0;
}

/* @func: _scanf_oux (static)
 * #desc:
 *    scanf integer (unsigned octal/decimal/hexadecimal) format.
 *
 * #1: ctx [in]  scanf struct context
 * #2: ap  [out] variable parameter pointer
 * #r:     [ret] 0: no error, -1: error
 */
static int32_t _scanf_oux(struct scanf_ctx *ctx, va_list *ap)
{
	uint64_t m, n = 0;
	int32_t c, d = 0, b = 10, st = 0;

	if (ctx->flags & FG_LONG) {
		m = ULONG_MAX;
	} else if (ctx->flags & FG_LONG_LONG) {
		m = ULLONG_MAX;
	} else if (ctx->flags & FG_SHORT) {
		m = USHRT_MAX;
	} else if (ctx->flags & FG_CHAR) {
		m = UCHAR_MAX;
	} else {
		m = UINT_MAX;
	}

	switch (ctx->specifiers) {
		case 'o':
			b = 8;
			break;
		case 'u':
			b = 10;
			break;
		case 'x': case 'X': case 'p':
			b = 16;
			break;
		default:
			return -1;
	}

	/* skip space */
	while (1) {
		c = ctx->get(1, ctx->arg);
		if (!conch_strchr(SC_SPACE, c))
			break;
		ctx->get(0, ctx->arg);
	}

	/* prefix */
	c = ctx->get(1, ctx->arg);
	if (c == '0') {
		ctx->get(0, ctx->arg);
		c = ctx->get(1, ctx->arg);
		if (c == 'B' || c == 'b') {
			if (b != 2)
				return -1;
			ctx->get(0, ctx->arg);
		} else if (c == 'X' || c == 'x') {
			if (b != 16)
				return -1;
			ctx->get(0, ctx->arg);
		}
	}

	while (1) {
		c = ctx->get(1, ctx->arg);
		if (c != '0')
			break;
		ctx->get(0, ctx->arg);
	}

	/* convert to number */
	while (1) {
		c = ctx->get(1, ctx->arg);
		if (c == '\0')
			break;
		if (st && conch_strchr(SC_SPACE, c))
			break;

		if (c >= '0' && c <= '9') {
			d = (c - '0');
		} else if (c >= 'A' && c <= 'F') {
			d = (c - 'A') + 10;
		} else if (c >= 'a' && c <= 'f') {
			d = (c - 'a') + 10;
		} else {
			return -1;
		}
		if (d >= b)
			return -1;

		if (n > ((m - d) / b))
			m = 0;
		n = (n * b) + d;

		ctx->get(0, ctx->arg);
		st = 1;
	}

	if (ctx->flags & FG_SKIP)
		return 0;

	n = m ? n : 0;

	if (ctx->specifiers == 'p') {
		*va_arg(*ap, void **) =	(void *)n;
	} else if (ctx->flags & FG_LONG) {
		*va_arg(*ap, unsigned long *) = (unsigned long)n;
	} else if (ctx->flags & FG_LONG_LONG) {
		*va_arg(*ap, unsigned long long *) = (unsigned long long)n;
	} else if (ctx->flags & FG_SHORT) {
		*va_arg(*ap, unsigned short *) = (unsigned short)n;
	} else if (ctx->flags & FG_CHAR) {
		*va_arg(*ap, unsigned char *) = (unsigned char)n;
	} else {
		*va_arg(*ap, unsigned int *) = (unsigned int)n;
	}

	return 0;
}

/* @func: _scanf_c (static)
 * #desc:
 *    scanf character format.
 *
 * #1: ctx [in]  scanf struct context
 * #2: ap  [out] variable parameter pointer
 * #r:     [ret] 0: no error, -1: error
 */
static int32_t _scanf_c(struct scanf_ctx *ctx, va_list *ap)
{
	int32_t c;

	/* skip space */
	while (1) {
		c = ctx->get(1, ctx->arg);
		if (!conch_strchr(SC_SPACE, c))
			break;
		ctx->get(0, ctx->arg);
	}

	c = ctx->get(1, ctx->arg);
	if (c == '\0')
		return -1;

	ctx->get(0, ctx->arg);
	if (ctx->flags & FG_SKIP)
		return 0;

	*va_arg(*ap, char *) = (char)c;

	return 0;
}

/* @func: _scanf_s (static)
 * #desc:
 *    scanf string format.
 *
 * #1: ctx [in/out] scanf struct context
 * #2: ap  [out]    variable parameter pointer
 * #r:     [ret]    0: no error, -1: error
 */
static int32_t _scanf_s(struct scanf_ctx *ctx, va_list *ap)
{
	int32_t c;
	char *p;

	/* skip space */
	while (1) {
		c = ctx->get(1, ctx->arg);
		if (!conch_strchr(SC_SPACE, c))
			break;
		ctx->get(0, ctx->arg);
	}

	if (ctx->flags & FG_SKIP) {
		p = NULL;
	} else {
		p = va_arg(*ap, char *);
	}

	/* copy string */
	while (ctx->buf_len--) {
		c = ctx->get(1, ctx->arg);
		if (c == '\0')
			break;
		if (conch_strchr(SC_SPACE, c))
			break;

		ctx->get(0, ctx->arg);
		if (p)
			*p++ = (char)c;
	}

	if (p)
		*p = '\0';

	return 0;
}

/* @func: _scanf_expr (static)
 * #desc:
 *    scanf expr-string format.
 *
 * #1: ctx  [in/out] scanf struct context
 * #2: ap   [out]    variable parameter pointer
 * #3: expr [in/out] expr-string
 * #r:      [ret]    0: no error, -1: error
 */
static int32_t _scanf_expr(struct scanf_ctx *ctx, va_list *ap,
		const char **expr)
{
	char table[256];
	const char *p;
	char *p2;
	int32_t st = 0, tmp = 0, not = 0;
	conch_memset(table, 0, sizeof(table));

	p = *expr;
	for (; *p != '\0'; p++) {
		char c = *p;
		switch (st) {
			case 0:
				if (c != '[')
					return -1;
				st = 1;
				break;
			case 1: /* [^char or [char */
				if (c == '^') {
					not = 1;
					st = 2;
					break;
				}
			case 2:
				tmp = (uint8_t)c;
				table[tmp] = 1;
				st = 3;
				break;
			case 3: /* range */
				if (c == ']')
					goto e;

				if (c == '-') {
					st = 4;
				} else {
					tmp = (uint8_t)c;
					table[tmp] = 1;
				}
				break;
			case 4:
				if (c == ']') {
					table['-'] = 1;
					goto e;
				}

				for (; tmp <= (uint8_t)c; tmp++)
					table[tmp] = 1;
				st = 3;
				break;
			default:
				return -1;
		}
	}
e:
	if (*p != ']')
		return -1;
	*expr = p;

	if (ctx->flags & FG_SKIP) {
		p2 = NULL;
	} else {
		p2 = va_arg(*ap, char *);
	}

	/* copy string */
	while (ctx->buf_len--) {
		int32_t c = ctx->get(1, ctx->arg);
		if (c & ~0xff)
			return -1;

		if (not) {
			if (table[c])
				break;
		} else {
			if (!table[c])
				break;
		}

		ctx->get(0, ctx->arg);
		if (p2)
			*p2++ = (char)c;
	}

	if (p2)
		*p2 = '\0';

	return 0;
}

/* @func: _scanf_f (static)
 * #desc:
 *    scanf floating format.
 *
 * #1: ctx [in]  scanf struct context
 * #2: ap  [out] variable parameter pointer
 * #r:     [ret] 0: no error, -1: error
 */
static int32_t _scanf_f(struct scanf_ctx *ctx, va_list *ap)
{
	double x = 0.0, x2 = 0.0;
	int32_t c, n, d = 0, b = 10, neg = 0, st = 0;

	/* skip space */
	while (1) {
		c = ctx->get(1, ctx->arg);
		if (!conch_strchr(SC_SPACE, c))
			break;
		ctx->get(0, ctx->arg);
	}

	c = ctx->get(1, ctx->arg);
	if (c == '-' || c == '+') {
		neg = (c == '-') ? 1 : 0;
		ctx->get(0, ctx->arg);
		c = ctx->get(1, ctx->arg);
	}

	/* prefix */
	if (c == '0') {
		ctx->get(0, ctx->arg);
		c = ctx->get(1, ctx->arg);
		if (c == 'X' || c == 'x') {
			b = 16;
			ctx->get(0, ctx->arg);
		}
	}

	while (1) {
		c = ctx->get(1, ctx->arg);
		if (c != '0')
			break;
		ctx->get(0, ctx->arg);
	}

	/* integer */
	while (1) {
		c = ctx->get(1, ctx->arg);
		if (c == '\0')
			break;
		if (st && conch_strchr(SC_SPACE, c))
			break;

		if (b == 10 && (c == 'E' || c == 'e')) {
			break;
		} else if (b == 16 && (c == 'P' || c == 'p')) {
			break;
		} else if (c == '.') {
			break;
		}

		if (c >= '0' && c <= '9') {
			d = (c - '0');
		} else if (c >= 'A' && c <= 'F') {
			d = (c - 'A') + 10;
		} else if (c >= 'a' && c <= 'f') {
			d = (c - 'a') + 10;
		} else {
			return -1;
		}
		if (d >= b)
			return -1;

		x = (x * b) + d;
		ctx->get(0, ctx->arg);
		st = 1;
	}

	/* decimals */
	c = ctx->get(1, ctx->arg);
	if (c == '.') {
		ctx->get(0, ctx->arg);
		n = 0;
		st = 0;

		while (1) {
			c = ctx->get(1, ctx->arg);
			if (c == '\0')
				break;
			if (st && conch_strchr(SC_SPACE, c))
				break;

			if (b == 10 && (c == 'E' || c == 'e')) {
				break;
			} else if (b == 16 && (c == 'P' || c == 'p')) {
				break;
			}

			if (c >= '0' && c <= '9') {
				d = (c - '0');
			} else if (c >= 'A' && c <= 'F') {
				d = (c - 'A') + 10;
			} else if (c >= 'a' && c <= 'f') {
				d = (c - 'a') + 10;
			} else {
				return -1;
			}
			if (d >= b)
				return -1;

			x2 = (x2 * b) + d;
			ctx->get(0, ctx->arg);
			n++;
			st = 1;
		}

		if (n)
			x += x2 / conch_pow(b, n);
	}

	/* exponent */
	c = ctx->get(1, ctx->arg);
	if ((b == 10 && (c == 'E' || c == 'e'))
			|| (b == 16 && (c == 'P' || c == 'p'))) {
		ctx->get(0, ctx->arg);
		n = 0;
		st = 0;

		while (1) {
			c = ctx->get(1, ctx->arg);
			if (c == '\0')
				break;
			if (st && conch_strchr(SC_SPACE, c))
				break;

			if (c >= '0' && c <= '9') {
				d = (c - '0');
			} else {
				return -1;
			}

			n = (n * 10) + d;
			ctx->get(0, ctx->arg);
			st = 1;
		}

		if (b == 10) {
			x *= conch_pow(10, n);
		} else {
			x *= conch_pow(2, n);
		}
	}

	if (ctx->flags & FG_SKIP)
		return 0;

	x = neg ? -x : x;
	*va_arg(*ap, double *) = x;

	return 0;
}

/* @func: __conch_scanf
 * #desc:
 *    formatted input conversion.
 *
 * #1: fmt [in]  format string
 * #2: ap  [out] variable argument pointer
 * #3: arg [in]  callback arg
 * #4: get [in]  callback (is-peek, arg)
 * #r:     [ret] >0: number of matching, <0: format error
 */
int32_t __conch_scanf(const char *fmt, va_list *ap, void *arg,
		int32_t (*get)(int32_t, void *))
{
	int32_t c, n = 0;
	char *p;
	struct scanf_ctx ctx = {
		.arg = arg, .get = get
		};

	for (; *fmt != '\0'; fmt++) {
		if (*fmt == '%') {
			fmt++;
			if (*fmt == '%') {
				c = ctx.get(1, ctx.arg);
				if (c != *fmt)
					return -n;
				ctx.get(0, ctx.arg);
				continue;
			}
		} else {
			c = ctx.get(1, ctx.arg);
			if (*fmt == ' ') {
				if (!conch_strchr(SC_SPACE, c))
					return -n;
			} else {
				if (c != *fmt)
					return -n;
			}
			ctx.get(0, ctx.arg);
			continue;
		}

		ctx.flags = 0;
		ctx.buf_len = INT32_MAX;

		/* assignment-suppression character */
		if (*fmt == '*') {
			ctx.flags |= FG_SKIP;
			fmt++;
		}

		/* buffer size limit */
		if (*fmt > '0' && *fmt <= '9') {
			ctx.buf_len = (int32_t)conch_strtol(fmt,
				(char **)&fmt, 10);
		}

		/* length modifier */
		switch (*fmt) {
			case 'L': /* long double */
				ctx.flags |= FG_LONG_DOUBLE;
				fmt++;
				break;
			case 'l':
				fmt++;
				if (*fmt == 'l') { /* long long */
					ctx.flags |= FG_LONG_LONG;
				} else { /* long */
					ctx.flags |= FG_LONG;
					fmt--;
				}
				fmt++;
				break;
			case 'h':
				fmt++;
				if (*fmt == 'h') { /* char */
					ctx.flags |= FG_CHAR;
				} else { /* short */
					ctx.flags |= FG_SHORT;
					fmt--;
				}
				fmt++;
				break;
			case 'z': /* size_t */
			case 't': /* ptrdiff_t */
				ctx.flags |= FG_LONG;
				fmt++;
				break;
			default:
				break;
		}

		ctx.specifiers = *fmt;
		switch (ctx.specifiers) {
			case 'n':
				return -n;
			case 'd': /* decimal */
			case 'i': /* prefix determination */
				if (_scanf_di(&ctx, ap))
					return -n;
				break;
			case 'o': /* octal */
			case 'u': /* decimal */
			case 'x': /* hexadecimal */
			case 'X':
			case 'p': /* pointer */
				if (_scanf_oux(&ctx, ap))
					return -n;
				break;
			case 'c': /* character */
				if (_scanf_c(&ctx, ap))
					return -n;
				break;
			case 's': /* string */
				if (_scanf_s(&ctx, ap))
					return -n;
				break;
			case '[': /* expr-string */
				if (_scanf_expr(&ctx, ap, &fmt))
					return -n;
				break;
			case 'e': /* floating */
			case 'E':
			case 'f': case 'F': case 'g':
			case 'G': case 'a': case 'A':
				if (_scanf_f(&ctx, ap))
					return -n;
				break;
			default:
				return -n;
		}
		n++;
	}

	return n;
}
