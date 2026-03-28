/* @file: c_stdlib.h
 * #desc:
 *    The definitions of standard library.
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

#ifndef _CONCH_C_STDLIB_H
#define _CONCH_C_STDLIB_H

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>


struct random_ctx {
	int32_t type;
	int32_t *state;
	int32_t *fptr;
	int32_t *bptr;
	int32_t *eptr;
};

/* LCG (Linear Congruential Generator) */
#define RANDOM_TYPE0_NEW(name, seed) \
	int32_t name##_random_tab0[1] = { \
		seed \
		}; \
	struct random_ctx name = { \
		.type = 0, \
		.state = name##_random_tab0 \
		}

/*
 * LFSR (Linear Feedback Shift Register)
 * x^31 + x^3 + 1
 */
#define RANDOM_TYPE1_NEW(name) \
	int32_t name##_random_tab1[32] = { \
		-1726662223, 379960547, 1735697613, 1040273694, \
		1313901226, 1627687941, -179304937, -2073333483, \
		1780058412, -1989503057, -615974602, 344556628, \
		939512070, -1249116260, 1507946756, -812545463, \
		154635395, 1388815473, -1926676823, 525320961, \
		-1009028674, 968117788, -123449607, 1284210865, \
		435012392, -2017506339, -911064859, -370259173, \
		1132637927, 1398500161, -205601318, 0 \
		}; \
	struct random_ctx name = { \
		.type = 1, \
		.state = name##_random_tab1, \
		.fptr = &name##_random_tab1[3], \
		.bptr = &name##_random_tab1[0], \
		.eptr = &name##_random_tab1[31] \
		}


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
int32_t conch_random_r(struct random_ctx *p, int32_t *v)
;

#ifdef __cplusplus
}
#endif


#endif
