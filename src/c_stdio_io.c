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

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>
#include <conch/c_string.h>
#include <conch/c_stdio.h>


#include <unistd.h>
#include <fcntl.h>


typedef struct stdio_file {
	int32_t flags;
	int32_t fd;
	uint8_t *buf;
	size_t buf_size;
	uint8_t *rpos;
	uint8_t *rend;
	uint8_t *wbase;
	uint8_t *wpos;
	uint8_t *wend;
} _FILE;

#define FG_EOF 0x01
#define FG_ERR 0x02
#define FG_NORD 0x04
#define FG_NOWR 0x08
#define FG_SEEK 0x10
#define FG_TEXT 0x20


/* @func: _stdio_write (static)
 * #desc:
 *    write to the file from file buffer and input buffer.
 *
 * #1: fp  [in/out] stdio file struct
 * #2: buf [in]     input buffer
 * #3: len [in]     input length
 * #r:     [ret]    write length of the input buffer
 */
static size_t _stdio_write(_FILE *fp, const uint8_t *buf, size_t len)
{
	struct stdio_file *f = fp;
	size_t k = (size_t)(f->wpos - f->wbase), t = len;
	ssize_t r;

	/* file buffer */
	while (k) {
		r = write(f->fd, f->wbase, k);
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
		r = write(f->fd, buf, len);
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
 *    flush file buffer.
 *
 * #1: fp [in/out] stdio file struct
 * #r:    [ret]    0: no error, -1: error
 */
static int32_t _stdio_fflush(_FILE *fp)
{
	struct stdio_file *f = fp;
	ssize_t r;

	if (f->wpos != f->wbase) {
		_stdio_write(fp, NULL, 0);
		if (!fp->wpos)
			return -1;
	}

	if (f->flags & FG_SEEK && f->rpos != f->rend) {
		r = lseek(f->fd, 0, SEEK_CUR);
		if (r > 0) {
			r -= (ssize_t)(f->rend - f->rpos);
			lseek(f->fd, r, SEEK_SET);
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
	struct stdio_file *f = fp;
	uint8_t *buf = t;
	size_t k, len = m * n;
	ssize_t r;

	if (!f->rend || f->wend) {
		if (f->wpos != f->wbase)
			_stdio_write(fp, NULL, 0);

		f->wpos = f->wend = f->wbase = NULL;
		if (f->flags & FG_NORD) {
			f->flags |= FG_ERR;
			return 0;
		}
		if (f->flags & FG_EOF)
			return 0;
	}

	if (f->rpos == f->rend) {
		f->rpos = f->rend = f->buf;
		r = read(f->fd, f->buf, f->buf_size);
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
		r = read(f->fd, buf, len);
		if (r <= 0) {
			f->flags |= r ? FG_ERR : FG_EOF;
			return ((m * n) - len) / m;
		}
		buf += r;
		len -= r;
	}

	return m ? n : 0;
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
	struct stdio_file *f = fp;
	const uint8_t *buf = s;
	size_t k, w, len = m * n;

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
	if (len > k) {
		k = _stdio_write(fp, buf, len);
		return (k == len) ? n : (k / m);
	}

	if (f->flags & FG_TEXT) {
		for (k = len; k && buf[k - 1] != '\n'; k--);
		if (k) {
			w = _stdio_write(f, buf, k);
			if (w < k)
				return w / m;
			buf += w;
			len -= w;
		}
	}

	conch_memcpy(f->wpos, buf, len);
	f->wpos += len;

	return m ? n : 0;
}

static int32_t _stdio_fseek(_FILE *fp, int64_t off, int32_t whence)
{
	struct stdio_file *f = fp;
	ssize_t r;

	switch (whence) {
		case SEEK_SET: case SEEK_CUR: case SEEK_END:
			break;
		default:
			return -1;
	}

	_stdio_fflush(fp);

	if (f->flags & FG_SEEK) {
		r = lseek(f->fd, off, whence);
		if (r < 0)
			return -1;
	}
	f->flags &= ~FG_EOF;

	return 0;
}

static int64_t _stdio_ftell(_FILE *fp)
{
	struct stdio_file *f = fp;
	ssize_t r;

	if (f->flags & FG_SEEK) {
		r = lseek(f->fd, 0, SEEK_CUR);
		if (r < 0)
			return -1;

		if (f->rend) {
			r -= (ssize_t)(f->rpos - f->buf);
		} else if (f->wend) {
			r += (ssize_t)(f->wpos - f->buf);
		}

		return r;
	}

	return -1;
}

static void _stdio_rewind(_FILE *fp)
{
	_stdio_fseek(fp, 0, SEEK_SET);
}

static int32_t _stdio_fgetc(_FILE *fp)
{
	struct stdio_file *f = fp;
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

static int32_t _stdio_fputc(int32_t c, _FILE *fp)
{
	if (!_stdio_fwrite(&c, 1, 1, fp))
		return -1;

	return 0;
}

static int32_t _stdio_fgets(char *buf, int32_t len, _FILE *fp)
{
	if (!_stdio_fread(buf, 1, len, fp))
		return -1;

	return 0;
}

static int32_t _stdio_fputs(const char *s, _FILE *fp)
{
	size_t n = strlen(s);
	if (!_stdio_fwrite(s, 1, n, fp))
		return -1;

	return 0;
}

int main(void)
{
//	printf("h\nh%de\n", 1);

	_FILE fp_stdout;
	uint8_t buf[1024], buf2[1024];

	memset(&fp_stdout, 0, sizeof(_FILE));
	fp_stdout.flags |= FG_TEXT;
//	fp_stdout.flags |= FG_NORD;
	fp_stdout.fd = 1;
	fp_stdout.buf = buf;
	fp_stdout.buf_size = sizeof(buf);

	stdio_fwrite("h\nh%de\nh", 1, 8, &fp_stdout);
	stdio_fread(buf2, 1, 1, &fp_stdout);
	stdio_fflush(&fp_stdout);

	return 0;
}
