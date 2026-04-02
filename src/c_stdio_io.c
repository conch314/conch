/* @file: c_stdio_io.c
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>
#include <conch/c_string.h>
#include <conch/c_stdio.h>
#include <conch/c_unistd.h>
#include <conch/c_fcntl.h>
#include <conch/c_float.h>
#include <conch/c_math.h>


#define X_BUFSIZ 4096
#define X_IOFBF 0
#define X_IOLBF 1
#define X_IONBF 2

struct stdio_file {
	int32_t flags;
	int32_t fd;
	uint8_t *buf;
	size_t buf_size;
	uint8_t *rpos;
	uint8_t *rend;
	uint8_t *wbase;
	uint8_t *wpos;
	uint8_t *wend;
	uint8_t vbuf[X_BUFSIZ];
};

#define FG_EOF 0x01
#define FG_ERR 0x02
#define FG_NORD 0x04
#define FG_NOWR 0x08
#define FG_SEEK 0x10
#define FG_TEXT 0x20
#define FG_PERM 0x40

typedef struct {
	struct stdio_file fp;
} _FILE;


/* @func: _stdio_write (static)
 * #desc:
 *    write to the file from stream buffer and input buffer.
 *
 * #1: fp  [in/out] stdio file struct
 * #2: buf [in]     input buffer
 * #3: len [in]     input length
 * #r:     [ret]    write length of the input buffer
 */
static size_t _stdio_write(_FILE *fp, const uint8_t *buf, size_t len)
{
	struct stdio_file *f = (struct stdio_file *)fp;
	size_t k = (size_t)(f->wpos - f->wbase), t = len;
	ssize_t r;

	/* stream buffer */
	while (k) {
		r = conch_write(f->fd, f->wbase, k);
		if (r < 0) {
			f->wpos = f->wend = f->wbase = NULL;
			f->flags |= FG_ERR;
			return 0;
		}
		f->wbase += r;
		k -= r;
	}

	/* input buffer */
	while (len) {
		r = conch_write(f->fd, buf, len);
		if (r < 0) {
			f->wpos = f->wend = f->wbase = NULL;
			f->flags |= FG_ERR;
			return t - len;
		}
		buf += r;
		len -= r;
	}

	f->wpos = f->wend = f->wbase = f->buf;
	f->wend += f->buf_size;

	return t;
}

/* @func: _stdio_fflush (static)
 * #desc:
 *    flush stream buffer.
 *
 * #1: fp [in/out] stdio file struct
 * #r:    [ret]    0: no error, -1: error
 */
static int32_t _stdio_fflush(_FILE *fp)
{
	struct stdio_file *f = (struct stdio_file *)fp;
	xoff_t r;

	/* flush write */
	if (f->wpos != f->wbase) {
		_stdio_write(fp, NULL, 0);
		if (!f->wpos)
			return -1;
	}

	/* flush read */
	if (f->flags & FG_SEEK && f->rpos != f->rend) {
		r = conch_lseek(f->fd, 0, X_SEEK_CUR);
		if (r > 0) {
			r -= (xoff_t)(f->rend - f->rpos);
			conch_lseek(f->fd, r, X_SEEK_SET);
		}
	}

	f->wpos = f->wend = f->wbase = NULL;
	f->rpos = f->rend = NULL;

	return 0;
}

/* @func: _stdio_fread (static)
 * #desc:
 *    stdio read function.
 *
 * #1: t  [out]    output buffer
 * #2: m  [in]     items size
 * #3: n  [in]     items number
 * #4: fp [in/out] stdio file struct
 * #r:    [ret]    items number of the read
 */
static size_t _stdio_fread(void *t, size_t m, size_t n, _FILE *fp)
{
	struct stdio_file *f = (struct stdio_file *)fp;
	uint8_t *buf = t;
	size_t k, len = m * n;
	ssize_t r;

	if (!len)
		return 0;

	/* initialize read stream buffer */
	if (!f->rend || f->wend) {
		if (f->wpos != f->wbase) /* flush write */
			_stdio_write(fp, NULL, 0);

		f->wpos = f->wend = f->wbase = NULL;
		if (f->flags & FG_NORD) {
			f->flags |= FG_ERR;
			return 0;
		}
		if (f->flags & FG_EOF)
			return 0;
	}

	/* read to stream buffer */
	if (f->rpos == f->rend && f->buf_size) {
		f->rpos = f->rend = f->buf;
		r = conch_read(f->fd, f->buf, f->buf_size);
		if (r <= 0) {
			f->flags |= r ? FG_ERR : FG_EOF;
			return 0;
		}
		f->rend += r;
	}

#undef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

	k = (size_t)(f->rend - f->rpos);
	if (k) {
		k = MIN(k, len);
		conch_memcpy(buf, f->rpos, k);
		f->rpos += k;
		buf += k;
		len -= k;
	}

	while (len) {
		r = conch_read(f->fd, buf, len);
		if (r <= 0) {
			f->flags |= r ? FG_ERR : FG_EOF;
			return ((m * n) - len) / m;
		}
		buf += r;
		len -= r;
	}

	return n;
}

/* @func: _stdio_fwrite (static)
 * #desc:
 *    stdio write function.
 *
 * #1: s  [in]     input buffer
 * #2: m  [in]     items size
 * #3: n  [in]     items number
 * #4: fp [in/out] stdio file struct
 * #r:    [ret]    items number of the write
 */
static size_t _stdio_fwrite(const void *s, size_t m, size_t n, _FILE *fp)
{
	struct stdio_file *f = (struct stdio_file *)fp;
	const uint8_t *buf = s;
	size_t k, w, len = m * n;

	if (!len)
		return 0;

	/* initialize write stream buffer */
	if (!f->wend) {
		if (f->flags & FG_NOWR) {
			f->flags |= FG_ERR;
			return 0;
		}

		f->rpos = f->rend = NULL;
		f->wpos = f->wend = f->wbase = f->buf;
		f->wend += f->buf_size;
	}

	k = (size_t)(f->wend - f->wpos);
	if (len > k) { /* len > remaining */
		if (k) {
			conch_memcpy(f->wpos, buf, k);
			f->wpos += k;
			buf += k;
			len -= k;
		}

		k += _stdio_write(fp, buf, len);
		return (k == len) ? n : (k / m);
	}

	/* text stream */
	if (f->flags & FG_TEXT) {
		for (k = len; k && buf[k - 1] != '\n'; k--);
		if (len == 1 && k == 1) {
			*f->wpos++ = *buf;
			_stdio_write(fp, NULL, 0);
			return (f->flags & FG_ERR) ? 0 : 1;
		}

		if (k) {
			w = _stdio_write(fp, buf, k);
			if (w < k)
				return w / m;
			buf += w;
			len -= w;
		}
	}

	conch_memcpy(f->wpos, buf, len);
	f->wpos += len;

	return n;
}

/* @func: _stdio_fseek (static)
 * #desc:
 *    stdio seek function.
 *
 * #1: fp     [in/out] stdio file struct
 * #2: off    [in]     file offset
 * #3: whence [in]     seek type
 * #r:        [ret]    0: no error, -1: error
 */
static int32_t _stdio_fseek(_FILE *fp, int64_t off, int32_t whence)
{
	struct stdio_file *f = (struct stdio_file *)fp;
	xoff_t r;

	switch (whence) {
		case X_SEEK_SET:
		case X_SEEK_CUR:
		case X_SEEK_END:
			break;
		default:
			return -1;
	}

	if (_stdio_fflush(fp))
		return -1;

	if (f->flags & FG_SEEK) {
		r = conch_lseek(f->fd, off, whence);
		if (r < 0)
			return -1;
	}
	f->flags &= ~FG_EOF;

	return 0;
}

/* @func: _stdio_ftell (static)
 * #desc:
 *    stdio get the file position function.
 *
 * #1: fp [in]  stdio file struct
 * #r:    [ret] >=0: file position, -1: error
 */
static int64_t _stdio_ftell(_FILE *fp)
{
	struct stdio_file *f = (struct stdio_file *)fp;
	xoff_t r;

	if (f->flags & FG_SEEK) {
		r = conch_lseek(f->fd, 0, X_SEEK_CUR);
		if (r < 0)
			return -1;

		if (f->rend) {
			r -= (xoff_t)(f->rpos - f->buf);
		} else if (f->wend) {
			r += (xoff_t)(f->wpos - f->buf);
		}

		return r;
	}

	return -1;
}

/* @func: _stdio_rewind (static)
 * #desc:
 *    stdio reposition to the start function.
 *
 * #1: fp [in/out] stdio file struct
 */
static void _stdio_rewind(_FILE *fp)
{
	_stdio_fseek(fp, 0, X_SEEK_SET);
}

/* @func: _stdio_fgetc (static)
 * #desc:
 *    stdio get the character function.
 *
 * #1: fp [in/out] stdio file struct
 * #r:    [ret]    >=0: character, -1: error
 */
static int32_t _stdio_fgetc(_FILE *fp)
{
	struct stdio_file *f = (struct stdio_file *)fp;
	uint8_t c;

	if (f->rpos != f->rend) {
		return *f->rpos++;
	} else {
		if (!_stdio_fread(&c, 1, 1, fp))
			return -1;
		return c;
	}

	return -1;
}

/* @func: _stdio_fgets (static)
 * #desc:
 *    stdio get the character function.
 *
 * #1: buf [out]    output buffer
 * #2: len [in]     buffer length
 * #3: fp  [in/out] stdio file struct
 * #r:     [ret]    returns the string / NULL
 */
static char *_stdio_fgets(char *buf, int32_t len, _FILE *fp)
{
	if (!_stdio_fread(buf, 1, len, fp))
		return NULL;

	return buf;
}

/* @func: _stdio_fputc (static)
 *    stdio put the character function.
 *
 * #1: c  [in]     input character
 * #2: fp [in/out] stdio file struct
 * #r:    [ret]    0: no error, -1: error
 */
static int32_t _stdio_fputc(int32_t c, _FILE *fp)
{
	if (!_stdio_fwrite(&c, 1, 1, fp))
		return -1;

	return 0;
}

/* @func: _stdio_fputs (static)
 *    stdio put the string function.
 *
 * #1: s  [in]     input string
 * #2: fp [in/out] stdio file struct
 * #r:    [ret]    0: no error, -1: error
 */
static int32_t _stdio_fputs(const char *s, _FILE *fp)
{
	size_t n = conch_strlen(s);
	if (!_stdio_fwrite(s, 1, n, fp))
		return -1;

	return 0;
}

/* @func: _stdio_fopen (static)
 * #desc:
 *    stdio open file stream function.
 *
 * #1: path [in]  path name
 * #2: mode [in]  access mode
 * #r:      [ret] returns the stdio pointer / NULL
 */
static _FILE *_stdio_fopen(const char *path, const char *mode)
{
	struct stdio_file *f;
	int32_t flags;

	f = malloc(sizeof(struct stdio_file));
	if (!f)
		return NULL;

	conch_memset(f, 0, sizeof(struct stdio_file));
	f->buf = f->vbuf;
	f->buf_size = X_BUFSIZ;

	switch (*mode) {
		case 'r':
			flags = X_O_RDONLY;
			if (mode[1] == '+') {
				flags = X_O_RDWR;
			} else {
				f->flags |= FG_NOWR;
			}
			break;
		case 'w':
			flags = X_O_CREAT | X_O_TRUNC;
			if (mode[1] == '+') {
				flags |= X_O_RDWR;
			} else {
				flags |= X_O_WRONLY;
				f->flags |= FG_NORD;
			}
			break;
		case 'a':
			flags = X_O_CREAT | X_O_APPEND;
			if (mode[1] == '+') {
				flags |= X_O_RDWR;
			} else {
				flags |= X_O_WRONLY;
				f->flags |= FG_NORD;
			}
			break;
		default:
			free(f);
			return NULL;
	}

	if (conch_strchr(mode, 'x'))
		flags |= X_O_EXCL;
	if (conch_strchr(mode, 'e'))
		flags |= X_O_CLOEXEC;

	f->fd = conch_open(path, flags);
	if (f->fd < 0) {
		free(f);
		return NULL;
	}

	if (conch_lseek(f->fd, 0, X_SEEK_CUR) >= 0)
		f->flags |= FG_SEEK;

	return (_FILE *)f;
}

/* @func: _stdio_fclose (static)
 * #desc:
 *    stdio close file stream function.
 *
 * #1: fp [in/out] stdio file struct
 * #r:    [ret]    0: no error, -1: error
 */
static int32_t _stdio_fclose(_FILE *fp)
{
	struct stdio_file *f = (struct stdio_file *)fp;

	_stdio_fflush(fp);

	if (f->flags & FG_PERM)
		return -1;
	if (conch_close(f->fd))
		return -1;

	free(f);

	return 0;
}

/* @func: _stdio_setvbuf (static)
 * #desc:
 *    stdio stream buffer operations function.
 *
 * #1: fp   [in/out] stdio file struct
 * #2: buf  [in/out] stream buffer / NULL
 * #3: type [in]     set type
 * #4: size [in]     buffer size
 * #r:      [ret]    0: no error, -1: error
 */
static int32_t _stdio_setvbuf(_FILE *fp, uint8_t *buf, int32_t type,
		size_t size)
{
	struct stdio_file *f = (struct stdio_file *)fp;
	f->flags &= ~FG_TEXT;

	if (type == X_IONBF) {
		f->buf_size = 0;
	} else if (type == X_IOFBF || type == X_IOLBF) {
		if (buf && size) {
			f->buf = buf;
			f->buf_size = size;
		}
		if (type == X_IOLBF)
			f->flags |= FG_TEXT;
	} else {
		return -1;
	}

	return 0;
}

/* @func: _stdio_clearerr (static)
 * #desc:
 *    stdio get the eof flags function.
 *
 * #1: fp [in/out] stdio file struct
 * #r:    [ret]    0: no eof, -1: eof
 */
static int32_t _stdio_feof(_FILE *fp)
{
	struct stdio_file *f = (struct stdio_file *)fp;

	return (f->flags & FG_EOF) ? -1 : 0;
}

/* @func: _stdio_clearerr (static)
 * #desc:
 *    stdio get the error flags function.
 *
 * #1: fp [in/out] stdio file struct
 * #r:    [ret]    0: no error, -1: error
 */
static int32_t _stdio_ferror(_FILE *fp)
{
	struct stdio_file *f = (struct stdio_file *)fp;

	return (f->flags & FG_ERR) ? -1 : 0;
}

/* @func: _stdio_clearerr (static)
 * #desc:
 *    stdio clear error flags function.
 *
 * #1: fp [in/out] stdio file struct
 */
static void _stdio_clearerr(_FILE *fp)
{
	struct stdio_file *f = (struct stdio_file *)fp;

	f->flags &= ~FG_ERR;
}

static int32_t _call_out(const char *s, int32_t len, void *arg)
{
	if (len && !_stdio_fwrite(s, 1, len, (_FILE *)arg))
		return -1;

	return 0;
}

int32_t conch_vfprintf(_FILE *fp, const char *fmt, va_list ap)
{
	va_list _ap;
	va_copy(_ap, ap);

	if (__conch_printf(fmt, &_ap, fp, _call_out)) {
		va_end(_ap);
		return -1;
	}

	va_end(_ap);

	return 0;
}

int32_t conch_fprintf(_FILE *fp, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	if (conch_vfprintf(fp, fmt, ap)) {
		va_end(ap);
		return -1;
	}

	va_end(ap);

	return 0;
}

static int32_t _call_get(int32_t peek, void *arg)
{
	const char **p = (const char **)arg;

	return peek ? (**p) : (*((*p)++));
}

int32_t conch_vsscanf(const char *s, const char *fmt, va_list ap)
{
	const char *p = s;
	va_list _ap;
	va_copy(_ap, ap);

	if (__conch_scanf(fmt, &_ap, (void *)&p, _call_get)) {
		va_end(_ap);
		return -1;
	}

	va_end(_ap);

	return 0;
}

int32_t conch_sscanf(const char *s, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	if (conch_vsscanf(s, fmt, ap)) {
		va_end(ap);
		return -1;
	}

	va_end(ap);

	return 0;
}

struct getc_ctx {
	int32_t st;
	int32_t c;
};

static int32_t _call_getc(int32_t peek, void *arg)
{
	struct getc_ctx *ctx = arg;
	int32_t c;

	switch (ctx->st) {
		case 0:
			ctx->c = c = fgetc(stdin);
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

int32_t conch_scanf(const char *fmt, ...)
{
	struct getc_ctx ctx = { 0 };
	va_list ap;
	va_start(ap, fmt);

	if (__conch_scanf(fmt, &ap, &ctx, _call_getc)) {
		va_end(ap);
		return -1;
	}

	va_end(ap);

	return 0;
}

int main(void)
{
//	printf("h\nh%de\n", 1);

	struct stdio_file _fp_stdout;
	uint8_t buf[1024], buf2[1230];

	conch_memset(&_fp_stdout, 0, sizeof(_fp_stdout));
	_fp_stdout.flags |= FG_TEXT;
	_fp_stdout.flags |= FG_NORD;
	_fp_stdout.flags |= FG_PERM;
	_fp_stdout.fd = 1;
	_fp_stdout.buf = buf;
	_fp_stdout.buf_size = sizeof(buf);

	_FILE *fp_stdout = (_FILE *)&_fp_stdout;
//	_FILE *fp_stdout = _stdio_fopen("t.bin", "w+");

	_stdio_fwrite("h\nh%de\nh", 1, 8, fp_stdout);
	_stdio_fread(buf2, 1, 1, fp_stdout);
	_stdio_fflush(fp_stdout);

	conch_fprintf(fp_stdout, "%s\n", "Hello, World!");

#define __stdout stdout

	/* signed */
	fprintf(__stdout,
		"0.1 A: hhd: %hhd %hhd\n", INT8_MIN, INT8_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"0.2 B: hhd: %hhd %hhd\n", INT8_MIN, INT8_MAX);
	_stdio_fflush(fp_stdout);
	fprintf(__stdout,
		"1.1 A: hhd: %hhd %hhd\n", INT32_MIN, INT32_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"1.2 B: hhd: %hhd %hhd\n", INT32_MIN, INT32_MAX);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"2.1 A: hd: %hd %hd\n", INT16_MIN, INT16_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"2.2 B: hd: %hd %hd\n", INT16_MIN, INT16_MAX);
	_stdio_fflush(fp_stdout);
	fprintf(__stdout,
		"3.1 A: hd: %hd %hd\n", INT32_MIN, INT32_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"3.2 B: hd: %hd %hd\n", INT32_MIN, INT32_MAX);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"4.1 A: d: %d %d\n", INT32_MIN, INT32_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"4.2 B: d: %d %d\n", INT32_MIN, INT32_MAX);
	_stdio_fflush(fp_stdout);
	fprintf(__stdout,
		"5.1 A: d: %d %d\n", INT64_MIN, INT64_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"5.2 B: d: %d %d\n", INT64_MIN, INT64_MAX);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"6.1 A: ld: %ld %ld\n", INT64_MIN, INT64_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"6.2 B: ld: %ld %ld\n", INT64_MIN, INT64_MAX);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"7.1 A: lld: %lld %lld\n", INT64_MIN, INT64_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"7.2 B: lld: %lld %lld\n", INT64_MIN, INT64_MAX);
	_stdio_fflush(fp_stdout);

	/* unsigned */
	fprintf(__stdout,
		"8.1 A: hhu: %hhu\n", UINT8_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"8.2 B: hhu: %hhu\n", UINT8_MAX);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"9.1 A: hhu: %hhu\n", UINT32_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"9.2 B: hhu: %hhu\n", UINT32_MAX);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"10.1 A: hu: %hu\n", UINT16_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"10.2 B: hu: %hu\n", UINT16_MAX);
	_stdio_fflush(fp_stdout);
	fprintf(__stdout,
		"11.1 A: hu: %hu\n", UINT32_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"11.2 B: hu: %hu\n", UINT32_MAX);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"12.1 A: u: %u\n", UINT32_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"12.2 B: u: %u\n", UINT32_MAX);
	_stdio_fflush(fp_stdout);
	fprintf(__stdout,
		"13.1 A: u: %u\n", UINT64_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"13.2 B: u: %u\n", UINT64_MAX);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"14.1 A: lu: %lu\n", UINT64_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"14.2 B: lu: %lu\n", UINT64_MAX);
	_stdio_fflush(fp_stdout);

	/* signed */
	fprintf(__stdout,
		"15.1 A: hhx: %hhx %hhx\n", INT8_MIN, INT8_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"15.2 B: hhx: %hhx %hhx\n", INT8_MIN, INT8_MAX);
	_stdio_fflush(fp_stdout);
	fprintf(__stdout,
		"16.1 A: hhx: %hhx %hhx\n", INT32_MIN, INT32_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"16.2 B: hhx: %hhx %hhx\n", INT32_MIN, INT32_MAX);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"17.1 A: hx: %hx %hx\n", INT16_MIN, INT16_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"17.2 B: hx: %hx %hx\n", INT16_MIN, INT16_MAX);
	_stdio_fflush(fp_stdout);
	fprintf(__stdout,
		"18.1 A: hx: %hx %hx\n", INT32_MIN, INT32_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"18.2 B: hx: %hx %hx\n", INT32_MIN, INT32_MAX);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"19.1 A: x: %x %x\n", INT32_MIN, INT32_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"19.2 B: x: %x %x\n", INT32_MIN, INT32_MAX);
	_stdio_fflush(fp_stdout);
	fprintf(__stdout,
		"20.1 A: x: %x %x\n", INT64_MIN, INT64_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"20.2 B: x: %x %x\n", INT64_MIN, INT64_MAX);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"21.1 A: lx: %lx %lx\n", INT64_MIN, INT64_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"21.2 B: lx: %lx %lx\n", INT64_MIN, INT64_MAX);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"22.1 A: llx: %llx %llx\n", INT64_MIN, INT64_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"22.2 B: llx: %llx %llx\n", INT64_MIN, INT64_MAX);
	_stdio_fflush(fp_stdout);

	/* unsigned hex */
	fprintf(__stdout,
		"23.1 A: llu: %llu\n", UINT64_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"23.2 B: llu: %llu\n", UINT64_MAX);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"24.1 A: hhx: %hhx\n", UINT8_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"24.2 B: hhx: %hhx\n", UINT8_MAX);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"25.1 A: hhx: %hhx\n", UINT32_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"25.2 B: hhx: %hhx\n", UINT32_MAX);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"26.1 A: hx: %hx\n", UINT16_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"26.2 B: hx: %hx\n", UINT16_MAX);
	_stdio_fflush(fp_stdout);
	fprintf(__stdout,
		"27.1 A: hx: %hx\n", UINT32_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"27.2 B: hx: %hx\n", UINT32_MAX);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"28.1 A: x: %x\n", UINT32_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"28.2 B: x: %x\n", UINT32_MAX);
	_stdio_fflush(fp_stdout);
	fprintf(__stdout,
		"29.1 A: x: %x\n", UINT64_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"29.2 B: x: %x\n", UINT64_MAX);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"30.1 A: lx: %lx\n", UINT64_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"30.2 B: lx: %lx\n", UINT64_MAX);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"31.1 A: llx: %llx\n", UINT64_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"31.2 B: llx: %llx\n", UINT64_MAX);
	_stdio_fflush(fp_stdout);

	/* floating */
	fprintf(__stdout,
		"32.1 A: f: %.380f\n", X_FP_DBL_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"32.2 B: f: %.380f\n", X_FP_DBL_MAX);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"33.1 A: f: %.380f\n", -X_FP_DBL_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"33.2 B: f: %.380f\n", -X_FP_DBL_MAX);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"34.1 A: f: %.380f\n", X_FP_DBL_MIN);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"34.2 B: f: %.380f\n", X_FP_DBL_MIN);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"35.1 A: f: %.380f\n", -X_FP_DBL_MIN);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"35.2 B: f: %.380f\n", -X_FP_DBL_MIN);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"36.1 A: f: %.380f\n", -X_FP_DBL_MIN);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"36.2 B: f: %.380f\n", -X_FP_DBL_MIN);
	_stdio_fflush(fp_stdout);

	/* character and string */
	fprintf(__stdout,
		"37.1 A: c: %c\n", 'w');
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"37.2 B: c: %c\n", 'w');
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"38.1 A: c: %9c\n", 'w');
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"38.2 B: c: %9c\n", 'w');
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"39.1 A: s: %s\n", "Hello, World");
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"39.2 B: s: %s\n", "Hello, World");
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"40.1 A: s: %.6s\n", "Hello, World");
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"40.2 B: s: %.6s\n", "Hello, World");
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"41.1 A: s: %19s\n", "Hello, World");
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"41.2 B: s: %19s\n", "Hello, World");
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"42.1 A: s: %-19s-\n", "Hello, World");
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"42.2 B: s: %-19s-\n", "Hello, World");
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"43.1 A: s: %33s-\n", "Hello, World");
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"43.2 B: s: %33s-\n", "Hello, World");
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"44.1 A: s: %33s ω %s-\n", "Hello, World ω", NULL);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"44.2 B: s: %33s ω %s-\n", "Hello, World ω", NULL);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"45.1 A: s: %*s ω %s-\n", -33, "Hello, World ω", NULL);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"45.2 B: s: %*s ω %s-\n", -33, "Hello, World ω", NULL);
	_stdio_fflush(fp_stdout);

	/* align */
	fprintf(__stdout,
		"46.1 A: %20lx$\n", INT64_MIN);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"46.2 B: %20lx$\n", INT64_MIN);
	_stdio_fflush(fp_stdout);
	fprintf(__stdout,
		"47.1 A: %020lx$\n", INT64_MIN);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"47.2 B: %020lx$\n", INT64_MIN);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"48.1 A: %-20lx$\n", INT64_MIN);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"48.2 B: %-20lx$\n", INT64_MIN);
	_stdio_fflush(fp_stdout);
	fprintf(__stdout,
		"49.1 A: %-020lx$\n", INT64_MIN);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"49.2 B: %-020lx$\n", INT64_MIN);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"50.1 A: %-.20lx$\n", INT64_MIN);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"50.2 B: %-.20lx$\n", INT64_MIN);
	_stdio_fflush(fp_stdout);
	fprintf(__stdout,
		"51.1 A: %-0.20lx$\n", INT64_MIN);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"51.2 B: %-0.20lx$\n", INT64_MIN);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"52.1 A: %#.20lx$\n", INT64_MIN);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"52.2 B: %#.20lx$\n", INT64_MIN);
	_stdio_fflush(fp_stdout);
	fprintf(__stdout,
		"53.1 A: %40.20lx$\n", INT64_MIN);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"53.2 B: %40.20lx$\n", INT64_MIN);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"54.1 A: %040c$\n", 'A');
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"54.2 B: %040c$\n", 'A');
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"55.1 A: %40c$\n", 'A');
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"55.2 B: %40c$\n", 'A');
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"56.1 A: %-40c$\n", 'A');
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"56.2 B: %-40c$\n", 'A');
	_stdio_fflush(fp_stdout);

	/* floating */
	fprintf(__stdout,
		"57.1 A: %.324f\n", 0.0);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"57.2 B: %.324f\n", 0.0);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"58.1 A: %.324f\n", 17.9729);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"58.2 B: %.324f\n", 17.9729);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"59.1 A: %.324f\n", 1.79729);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"59.2 B: %.324f\n", 1.79729);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"60.1 A: %.324f\n", 0.179729);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"60.2 B: %.324f\n", 0.179729);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"61.1 A: %.324f\n", X_FP_DBL_MIN);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"61.2 B: %.324f\n", X_FP_DBL_MIN);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"62.1 A: %.324f\n", X_FP_DBL_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"62.2 B: %.324f\n", X_FP_DBL_MAX);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"63.1 A: %.0f\n", X_FP_DBL_MIN);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"63.2 B: %.0f\n", X_FP_DBL_MIN);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"64.1 A: %.0f\n", X_FP_DBL_MAX);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"64.2 B: %.0f\n", X_FP_DBL_MAX);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"65.1 A: %.324f\n", 0.001897);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"65.2 B: %.324f\n", 0.001897);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"66.1 A: %.324f\n", 527.1481286241434);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"66.2 B: %.324f\n", 527.1481286241434);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"66.1 A: %f\n", NAN);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"66.2 B: %f\n", NAN);
	_stdio_fflush(fp_stdout);

	fprintf(__stdout,
		"66.1 A: %f\n", INFINITY);
	fflush(__stdout);
	conch_fprintf(fp_stdout,
		"66.2 B: %f\n", INFINITY);
	_stdio_fflush(fp_stdout);

	_stdio_fclose(fp_stdout);

	clock_t start, end;
	double time;
	uint64_t len;

	_FILE *fp1 = _stdio_fopen("/dev/zero", "w");
	FILE *fp2 = fopen("/dev/zero", "w");

	len = 0;
	start = clock();
	for (int32_t i = 0; i < (1 << 16); i++) {
		_stdio_fwrite(buf2, 1, sizeof(buf2), fp1);
		len += sizeof(buf2);
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("%.6f - %zu MiB (%.2f MiB/s)\n", time,
		len / 1024 / 1024,
		(len / time) / 1024 / 1024);

	len = 0;
	start = clock();
	for (int32_t i = 0; i < (1 << 16); i++) {
		fwrite(buf2, 1, sizeof(buf2), fp2);
		len += sizeof(buf2);
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("%.6f - %zu MiB (%.2f MiB/s)\n", time,
		len / 1024 / 1024,
		(len / time) / 1024 / 1024);

	int32_t d1, d2, d3;
	char s1[8], s2[12];
	double f1;

	conch_sscanf("123 -0x123 0 hello He[a]o 1.0071e10 0x12",
		"%*d %i %o %8s %10[][A-Za-z] %f %x",
		&d1, &d2, s1, s2, &f1, &d3);
	conch_fprintf(fp_stdout,
		"%d %d %s %s %.15f %#x\n", d1, d2, s1, s2, f1, d3);
	_stdio_fflush(fp_stdout);

	conch_scanf("%d %i %c", &d1, &d2, &d3);
	conch_fprintf(fp_stdout,
		"%d %d %c\n", d1, d2, d3);
	_stdio_fflush(fp_stdout);

	return 0;
}
