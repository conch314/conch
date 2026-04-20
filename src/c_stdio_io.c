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
#include <conch/c_stdlib.h>
#include <conch/c_unistd.h>
#include <conch/c_fcntl.h>
#include <conch/c_float.h>
#include <conch/c_math.h>
#include <conch/c_atomic.h>


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
	spinlock_t lock;
};

#define FG_EOF  0x01
#define FG_ERR  0x02
#define FG_NORD 0x04
#define FG_NOWR 0x08
#define FG_SEEK 0x10
#define FG_TEXT 0x20
#define FG_PERM 0x40

static struct stdio_file __stdin = {
	.flags = FG_PERM | FG_NOWR,
	.fd = 0,
	.buf = __stdin.vbuf,
	.buf_size = X_BUFSIZ,
	.rpos = NULL,
	.rend = NULL,
	.wbase = NULL,
	.wpos = NULL,
	.wend = NULL,
	.lock = 0
	};

static struct stdio_file __stdout = {
	.flags = FG_PERM | FG_TEXT | FG_NORD,
	.fd = 1,
	.buf = __stdin.vbuf,
	.buf_size = X_BUFSIZ,
	.rpos = NULL,
	.rend = NULL,
	.wbase = NULL,
	.wpos = NULL,
	.wend = NULL,
	.lock = 0
	};

static struct stdio_file __stderr = {
	.flags = FG_PERM | FG_NORD,
	.fd = 2,
	.buf = NULL,
	.buf_size = 0,
	.rpos = NULL,
	.rend = NULL,
	.wbase = NULL,
	.wpos = NULL,
	.wend = NULL,
	.lock = 0
	};

xFILE *__conch_stdin = (xFILE *)&__stdin;
xFILE *__conch_stdout = (xFILE *)&__stdout;
xFILE *__conch_stderr = (xFILE *)&__stderr;


/* @func: _stdio_write (static)
 * #desc:
 *    write to the file from stream buffer and input buffer.
 *
 * #1: fp  [in/out] stdio file struct
 * #2: buf [in]     input buffer
 * #3: len [in]     input length
 * #r:     [ret]    write length of the input buffer
 */
static size_t _stdio_write(xFILE *fp, const uint8_t *buf, size_t len)
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
static int32_t _stdio_fflush(xFILE *fp)
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
 *    read from the input stream into the buffer.
 *
 * #1: t  [out]    output buffer
 * #2: m  [in]     items size
 * #3: n  [in]     items number
 * #4: fp [in/out] stdio file struct
 * #r:    [ret]    items number of the read
 */
static size_t _stdio_fread(void *t, size_t m, size_t n, xFILE *fp)
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
 *    write from the buffer to the output stream.
 *
 * #1: s  [in]     input buffer
 * #2: m  [in]     items size
 * #3: n  [in]     items number
 * #4: fp [in/out] stdio file struct
 * #r:    [ret]    items number of the write
 */
static size_t _stdio_fwrite(const void *s, size_t m, size_t n, xFILE *fp)
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
 *    reposition a stream.
 *
 * #1: fp     [in/out] stdio file struct
 * #2: off    [in]     file offset
 * #3: whence [in]     offset type
 * #r:        [ret]    0: no error, -1: error
 */
static int32_t _stdio_fseek(xFILE *fp, int64_t off, int32_t whence)
{
	struct stdio_file *f = (struct stdio_file *)fp;
	xoff_t r;

	switch (whence) {
		case X_SEEK_SET: case X_SEEK_CUR: case X_SEEK_END:
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
 *    get the stream position.
 *
 * #1: fp [in]  stdio file struct
 * #r:    [ret] >=0: file position, -1: error
 */
static int64_t _stdio_ftell(xFILE *fp)
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
 *    reposition stream to the start.
 *
 * #1: fp [in/out] stdio file struct
 */
static void _stdio_rewind(xFILE *fp)
{
	_stdio_fseek(fp, 0, X_SEEK_SET);
}

/* @func: _stdio_fgetc (static)
 * #desc:
 *    get the character function.
 *
 * #1: fp [in/out] stdio file struct
 * #r:    [ret]    >=0: character, -1: error
 */
static int32_t _stdio_fgetc(xFILE *fp)
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
 *    get the character function.
 *
 * #1: buf [out]    output buffer
 * #2: len [in]     buffer length
 * #3: fp  [in/out] stdio file struct
 * #r:     [ret]    returns the string / NULL
 */
static char *_stdio_fgets(char *buf, int32_t len, xFILE *fp)
{
	if (!_stdio_fread(buf, 1, len, fp))
		return NULL;

	return buf;
}

/* @func: _stdio_fputc (static)
 * #desc:
 *    put the character function.
 *
 * #1: c  [in]     input character
 * #2: fp [in/out] stdio file struct
 * #r:    [ret]    0: no error, -1: error
 */
static int32_t _stdio_fputc(int32_t c, xFILE *fp)
{
	if (!_stdio_fwrite(&c, 1, 1, fp))
		return -1;

	return 0;
}

/* @func: _stdio_fputs (static)
 * #desc:
 *    put the string function.
 *
 * #1: s  [in]     input string
 * #2: fp [in/out] stdio file struct
 * #r:    [ret]    0: no error, -1: error
 */
static int32_t _stdio_fputs(const char *s, xFILE *fp)
{
	size_t n = conch_strlen(s);
	if (!_stdio_fwrite(s, 1, n, fp))
		return -1;

	return 0;
}

/* @func: _stdio_fopen (static)
 * #desc:
 *    open the file stream function.
 *
 * #1: path [in]  path name
 * #2: mode [in]  access mode
 * #r:      [ret] returns the stdio pointer / NULL
 */
static xFILE *_stdio_fopen(const char *path, const char *mode)
{
	struct stdio_file *f;
	int32_t flags;

	f = conch_malloc(sizeof(struct stdio_file));
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
			conch_free(f);
			return NULL;
	}

	if (conch_strchr(mode, 'x'))
		flags |= X_O_EXCL;
	if (conch_strchr(mode, 'e'))
		flags |= X_O_CLOEXEC;

	f->fd = conch_open(path, flags);
	if (f->fd < 0) {
		conch_free(f);
		return NULL;
	}

	if (conch_lseek(f->fd, 0, X_SEEK_CUR) >= 0)
		f->flags |= FG_SEEK;

	return (xFILE *)f;
}

/* @func: _stdio_fclose (static)
 * #desc:
 *    close the file stream function.
 *
 * #1: fp [in/out] stdio file struct
 * #r:    [ret]    0: no error, -1: error
 */
static int32_t _stdio_fclose(xFILE *fp)
{
	struct stdio_file *f = (struct stdio_file *)fp;

	if (!fp)
		return -1;

	_stdio_fflush(fp);

	if (f->flags & FG_PERM)
		return -1;
	if (conch_close(f->fd))
		return -1;

	conch_free(f);

	return 0;
}

/* @func: _stdio_setvbuf (static)
 * #desc:
 *    stream buffer operations function.
 *
 * #1: fp   [in/out] stdio file struct
 * #2: buf  [in/out] stream buffer / NULL
 * #3: type [in]     buffer type
 * #4: size [in]     buffer size
 * #r:      [ret]    0: no error, -1: error
 */
static int32_t _stdio_setvbuf(xFILE *fp, uint8_t *buf, int32_t type,
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

/* @func: _stdio_feof (static)
 * #desc:
 *    get the eof flags function.
 *
 * #1: fp [in/out] stdio file struct
 * #r:    [ret]    0: no eof, -1: eof
 */
static int32_t _stdio_feof(xFILE *fp)
{
	struct stdio_file *f = (struct stdio_file *)fp;

	return (f->flags & FG_EOF) ? X_EOF : 0;
}

/* @func: _stdio_ferror (static)
 * #desc:
 *    get the error flags function.
 *
 * #1: fp [in/out] stdio file struct
 * #r:    [ret]    0: no error, -1: error
 */
static int32_t _stdio_ferror(xFILE *fp)
{
	struct stdio_file *f = (struct stdio_file *)fp;

	return (f->flags & FG_ERR) ? -1 : 0;
}

/* @func: _stdio_clearerr (static)
 * #desc:
 *    clear error flags function.
 *
 * #1: fp [in/out] stdio file struct
 */
static void _stdio_clearerr(xFILE *fp)
{
	struct stdio_file *f = (struct stdio_file *)fp;

	f->flags &= ~FG_ERR;
}

/* @func: conch_fflush
 * #desc:
 *    flush stream buffer.
 *
 * #1: fp [in/out] stdio file struct
 * #r:    [ret]    0: no error, -1: error
 */
int32_t conch_fflush(xFILE *fp)
{
	struct stdio_file *f = (struct stdio_file *)fp;

	if (!f)
		return 0;

	SPIN_LOCK(&f->lock);
	int32_t ret = _stdio_fflush(fp);
	SPIN_UNLOCK(&f->lock);

	return ret;
}

/* @func: conch_fread
 * #desc:
 *    read from the input stream into the buffer.
 *
 * #1: t  [out]    output buffer
 * #2: m  [in]     items size
 * #3: n  [in]     items number
 * #4: fp [in/out] stdio file struct
 * #r:    [ret]    items number of the read
 */
size_t conch_fread(void *t, size_t m, size_t n, xFILE *fp)
{
	struct stdio_file *f = (struct stdio_file *)fp;

	SPIN_LOCK(&f->lock);
	size_t ret = _stdio_fread(t, m, n, fp);
	SPIN_UNLOCK(&f->lock);

	return ret;
}

/* @func: conch_fwrite
 * #desc:
 *    write from the buffer to the output stream.
 *
 * #1: s  [in]     input buffer
 * #2: m  [in]     items size
 * #3: n  [in]     items number
 * #4: fp [in/out] stdio file struct
 * #r:    [ret]    items number of the write
 */
size_t conch_fwrite(const void *s, size_t m, size_t n, xFILE *fp)
{
	struct stdio_file *f = (struct stdio_file *)fp;

	SPIN_LOCK(&f->lock);
	size_t ret = _stdio_fwrite(s, m, n, fp);
	SPIN_UNLOCK(&f->lock);

	return ret;
}

/* @func: conch_fseek
 * #desc:
 *    reposition stream to the start.
 *
 * #1: fp     [in/out] stdio file struct
 * #2: off    [in]     file offset
 * #3: whence [in]     seek type
 * #r:        [ret]    0: no error, -1: error
 */
int32_t conch_fseek(xFILE *fp, int64_t off, int32_t whence)
{
	struct stdio_file *f = (struct stdio_file *)fp;

	SPIN_LOCK(&f->lock);
	int32_t ret = _stdio_fseek(fp, off, whence);
	SPIN_UNLOCK(&f->lock);

	return ret;
}

/* @func: conch_ftell
 * #desc:
 *    get the stream position.
 *
 * #1: fp [in]  stdio file struct
 * #r:    [ret] >=0: file position, -1: error
 */
int64_t conch_ftell(xFILE *fp)
{
	struct stdio_file *f = (struct stdio_file *)fp;

	SPIN_LOCK(&f->lock);
	int64_t ret = _stdio_ftell(fp);
	SPIN_UNLOCK(&f->lock);

	return ret;
}

/* @func: conch_rewind
 * #desc:
 *    reposition stream to the start.
 *
 * #1: fp [in/out] stdio file struct
 */
void conch_rewind(xFILE *fp)
{
	struct stdio_file *f = (struct stdio_file *)fp;

	SPIN_LOCK(&f->lock);
	_stdio_rewind(fp);
	SPIN_UNLOCK(&f->lock);
}

/* @func: conch_fgetc
 * #desc:
 *    get the character function.
 *
 * #1: fp [in/out] stdio file struct
 * #r:    [ret]    >=0: character, -1: error
 */
int32_t conch_fgetc(xFILE *fp)
{
	struct stdio_file *f = (struct stdio_file *)fp;

	SPIN_LOCK(&f->lock);
	int32_t ret = _stdio_fgetc(fp);
	SPIN_UNLOCK(&f->lock);

	return ret;
}

/* @func: conch_fgets
 * #desc:
 *    get the character function.
 *
 * #1: buf [out]    output buffer
 * #2: len [in]     buffer length
 * #3: fp  [in/out] stdio file struct
 * #r:     [ret]    returns the string / NULL
 */
char *conch_fgets(char *buf, int32_t len, xFILE *fp)
{
	struct stdio_file *f = (struct stdio_file *)fp;

	SPIN_LOCK(&f->lock);
	char *ret = _stdio_fgets(buf, len, fp);
	SPIN_UNLOCK(&f->lock);

	return ret;
}

/* @func: conch_fputc
 * #desc:
 *    put the character function.
 *
 * #1: c  [in]     input character
 * #2: fp [in/out] stdio file struct
 * #r:    [ret]    0: no error, -1: error
 */
int32_t conch_fputc(int32_t c, xFILE *fp)
{
	struct stdio_file *f = (struct stdio_file *)fp;

	SPIN_LOCK(&f->lock);
	int32_t ret = _stdio_fputc(c, fp);
	SPIN_UNLOCK(&f->lock);

	return ret;
}

/* @func: conch_fputs
 * #desc:
 *    put the string function.
 *
 * #1: s  [in]     input string
 * #2: fp [in/out] stdio file struct
 * #r:    [ret]    0: no error, -1: error
 */
int32_t conch_fputs(const char *s, xFILE *fp)
{
	struct stdio_file *f = (struct stdio_file *)fp;

	SPIN_LOCK(&f->lock);
	int32_t ret = _stdio_fputs(s, fp);
	SPIN_UNLOCK(&f->lock);

	return ret;
}

/* @func: conch_fopen
 * #desc:
 *    open the file stream function.
 *
 * #1: path [in]  path name
 * #2: mode [in]  access mode
 * #r:      [ret] returns the stdio pointer / NULL
 */
xFILE *conch_fopen(const char *path, const char *mode)
{
	return _stdio_fopen(path, mode);
}

/* @func: conch_fclose
 * #desc:
 *    close the file stream function.
 *
 * #1: fp [in/out] stdio file struct
 * #r:    [ret]    0: no error, -1: error
 */
int32_t conch_fclose(xFILE *fp)
{
	struct stdio_file *f = (struct stdio_file *)fp;

	SPIN_LOCK(&f->lock);
	int32_t ret = _stdio_fclose(fp);
	SPIN_UNLOCK(&f->lock);

	return ret;
}

/* @func: conch_setvbuf
 * #desc:
 *    stream buffer operations function.
 *
 * #1: fp   [in/out] stdio file struct
 * #2: buf  [in/out] stream buffer / NULL
 * #3: type [in]     buffer type
 * #4: size [in]     buffer size
 * #r:      [ret]    0: no error, -1: error
 */
int32_t conch_setvbuf(xFILE *fp, uint8_t *buf, int32_t type,
		size_t size)
{
	struct stdio_file *f = (struct stdio_file *)fp;

	SPIN_LOCK(&f->lock);
	int32_t ret = _stdio_setvbuf(fp, buf, type, size);
	SPIN_UNLOCK(&f->lock);

	return ret;
}

/* @func: conch_feof
 * #desc:
 *    get the eof flags function.
 *
 * #1: fp [in/out] stdio file struct
 * #r:    [ret]    0: no eof, -1: eof
 */
int32_t conch_feof(xFILE *fp)
{
	struct stdio_file *f = (struct stdio_file *)fp;

	SPIN_LOCK(&f->lock);
	int32_t ret = _stdio_feof(fp);
	SPIN_UNLOCK(&f->lock);

	return ret;
}

/* @func: conch_ferror
 * #desc:
 *    get the error flags function.
 *
 * #1: fp [in/out] stdio file struct
 * #r:    [ret]    0: no error, -1: error
 */
int32_t conch_ferror(xFILE *fp)
{
	struct stdio_file *f = (struct stdio_file *)fp;

	SPIN_LOCK(&f->lock);
	int32_t ret = _stdio_ferror(fp);
	SPIN_UNLOCK(&f->lock);

	return ret;
}

/* @func: conch_clearerr
 * #desc:
 *    clear error flags function.
 *
 * #1: fp [in/out] stdio file struct
 */
void conch_clearerr(xFILE *fp)
{
	struct stdio_file *f = (struct stdio_file *)fp;

	SPIN_LOCK(&f->lock);
	_stdio_clearerr(fp);
	SPIN_UNLOCK(&f->lock);
}
