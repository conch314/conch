/* @file: c_stdlib.h
 * #desc:
 *    The definitions of standard library.
 *
 * #copy:
 *    Copyright (C) 1970 Public Free Software.
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

#ifndef _CONCH_C_STDLIB_H
#define _CONCH_C_STDLIB_H

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* c_stdlib_Exit.c */
extern
void conch_Exit(int32_t status)
;

/* c_stdlib_abort.c */
extern
void conch_abort(void)
;

/* c_stdlib_abs.c */
extern
int conch_abs(int n)
;
extern
long conch_labs(long n)
;
extern
long long conch_llabs(long long n)
;

/* c_stdlib_atof.c */
extern
double conch_atof(const char *s)
;
extern
double conch_strtod(const char *s, char **e)
;

/* c_stdlib_atoi.c */
extern
uint64_t __conch_strtoull(const char *s, char **e, uint64_t *m, int32_t b)
;
extern
int conch_atoi(const char *s)
;
extern
long conch_atol(const char *s)
;
extern
long long conch_atoll(const char *s)
;
extern
long conch_strtol(const char *s, char **e, int32_t b)
;
extern
unsigned long conch_strtoul(const char *s, char **e, int32_t b)
;
extern
long long conch_strtoll(const char *s, char **e, int32_t b)
;
extern
unsigned long long conch_strtoull(const char *s, char **e, int32_t b)
;

/* c_stdlib_exit.c */
extern
int32_t conch_atexit(void (*func)(void))
;
extern
void conch_exit(int32_t status)
;

/* c_stdlib_malloc.c */
extern
void *conch_malloc(size_t size)
;
extern
void conch_free(void *p)
;

/* c_stdlib_qsort.c */
extern
void conch_qsort(void *b, uint64_t n, uint64_t w,
		int32_t (*cmp)(const void *, const void *))
;
extern
void *conch_bsearch(const void *k, const void *b, uint64_t n, uint64_t w,
		int32_t (*cmp)(const void *, const void *))
;

/* c_stdlib_rand.c */
extern
int32_t conch_rand_r(int32_t *state)
;

#ifdef __cplusplus
}
#endif


#endif
