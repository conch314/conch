/* @file: c_stdio.h
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

#ifndef _CONCH_C_STDIO_H
#define _CONCH_C_STDIO_H

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>
#include <conch/c_stdarg.h>


#define X_SEEK_SET 0 /* absolute */
#define X_SEEK_CUR 1 /* relative */
#define X_SEEK_END 2 /* tail start */

#define X_BUFSIZ 4096
#define X_IOFBF 0 /* fully buffer */
#define X_IOLBF 1 /* line buffer */
#define X_IONBF 2 /* not buffer */

#define X_EOF (-1) /* end-of-file */

typedef struct {
	char _; /* internal implemented */
} xFILE;


#ifdef __cplusplus
extern "C" {
#endif

extern
xFILE *x_stdin;
extern
xFILE *x_stdout;
extern
xFILE *x_stderr;

/* c_stdio__printf.c */
extern
int32_t __conch_printf(const char *fmt, va_list *ap, void *arg,
		int32_t (*out)(const char *, int32_t, void *))
;

/* c_stdio__scanf.c */
extern
int32_t __conch_scanf(const char *fmt, va_list *ap, void *arg,
		int32_t (*get)(int32_t, void *))
;

/* c_stdio_io.c */
extern
int32_t conch_fflush(xFILE *fp)
;
extern
size_t conch_fread(void *t, size_t m, size_t n, xFILE *fp)
;
extern
size_t conch_fwrite(const void *s, size_t m, size_t n, xFILE *fp)
;
extern
int32_t conch_fseek(xFILE *fp, int64_t off, int32_t whence)
;
extern
int64_t conch_ftell(xFILE *fp)
;
extern
void conch_rewind(xFILE *fp)
;
extern
int32_t conch_fgetc(xFILE *fp)
;
extern
char *conch_fgets(char *buf, int32_t len, xFILE *fp)
;
extern
int32_t conch_fputc(int32_t c, xFILE *fp)
;
extern
int32_t conch_fputs(const char *s, xFILE *fp)
;
extern
xFILE *conch_fopen(const char *path, const char *mode)
;
extern
int32_t conch_fclose(xFILE *fp)
;
extern
int32_t conch_setvbuf(xFILE *fp, uint8_t *buf, int32_t type,
		size_t size)
;
extern
int32_t conch_feof(xFILE *fp)
;
extern
int32_t conch_ferror(xFILE *fp)
;
extern
void conch_clearerr(xFILE *fp)
;

/* c_stdio_printf.c */
extern
int32_t conch_vfprintf(xFILE *fp, const char *fmt, va_list ap)
;
extern
int32_t conch_fprintf(xFILE *fp, const char *fmt, ...)
;
extern
int32_t conch_vprintf(const char *fmt, va_list ap)
;
extern
int32_t conch_printf(const char *fmt, ...)
;

/* c_stdio_scanf.c */
extern
int32_t conch_vfscanf(xFILE *fp, const char *fmt, va_list ap)
;
extern
int32_t conch_vscanf(const char *fmt, va_list ap)
;
extern
int32_t conch_scanf(const char *fmt, ...)
;

/* c_stdio_snprintf.c */
extern
int32_t conch_vsnprintf(char *buf, size_t len, const char *fmt, va_list ap)
;
extern
int32_t conch_snprintf(char *buf, size_t len, const char *fmt, ...)
;

/* c_stdio_sscanf.c */
extern
int32_t conch_vsscanf(const char *s, const char *fmt, va_list ap)
;
extern
int32_t conch_sscanf(const char *s, const char *fmt, ...)
;

#ifdef __cplusplus
}
#endif


#endif
