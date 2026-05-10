/* @file: c_atomic.h
 * #desc:
 *    The definitions of atomic operations.
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

#ifndef _CONCH_C_ATOMIC_H
#define _CONCH_C_ATOMIC_H

#include <conch/config.h>
#include <conch/c_stdint.h>


typedef volatile int32_t spinlock_t;
typedef volatile int32_t atomic_t;

#define SPIN_SINGLE(x) conch_atomic_cas(x, 0, 1)
#define SPIN_LOCK(x) \
	do { \
		while (conch_atomic_cas(x, 0, 1)); \
	} while (0)
#define SPIN_UNLOCK(x) \
	do { \
		while (!conch_atomic_cas(x, 1, 0)); \
	} while (0)

#define ATOMIC_ADD(x, v) \
	do { \
		int32_t ___atomic_add_c = *((int32_t *)(x)); \
		int32_t ___atomic_add_o; \
		do { \
			___atomic_add_o = conch_atomic_cas(x, \
				___atomic_add_c, \
				___atomic_add_c + (v)); \
			if (___atomic_add_o == ___atomic_add_c) \
				break; \
			___atomic_add_c = ___atomic_add_o; \
		} while (1); \
	} while (0)
#define ATOMIC_SUB(x, v) \
	do { \
		int32_t ___atomic_sub_c = *((int32_t *)(x)); \
		int32_t ___atomic_sub_o; \
		do { \
			___atomic_sub_o = conch_atomic_cas(x, \
				___atomic_sub_c, \
				___atomic_sub_c - (v)); \
			if (___atomic_sub_o == ___atomic_sub_c) \
				break; \
			___atomic_sub_c = ___atomic_sub_o; \
		} while (1); \
	} while (0)
#define ATOMIC_AND(x, v) \
	do { \
		int32_t ___atomic_and_c = *((int32_t *)(x)); \
		int32_t ___atomic_and_o; \
		do { \
			___atomic_and_o = conch_atomic_cas(x, \
				___atomic_and_c, \
				___atomic_and_c & (v)); \
			if (___atomic_and_o == ___atomic_and_c) \
				break; \
			___atomic_and_c = ___atomic_and_o; \
		} while (1); \
	} while (0)
#define ATOMIC_OR(x, v) \
	do { \
		int32_t ___atomic_or_c = *((int32_t *)(x)); \
		int32_t ___atomic_or_o; \
		do { \
			___atomic_or_o = conch_atomic_cas(x, \
				___atomic_or_c, \
				___atomic_or_c | (v)); \
			if (___atomic_or_o == ___atomic_or_c) \
				break; \
			___atomic_or_c = ___atomic_or_o; \
		} while (1); \
	} while (0)
#define ATOMIC_XOR(x, v) \
	do { \
		int32_t ___atomic_xor_c = *((int32_t *)(x)); \
		int32_t ___atomic_xor_o; \
		do { \
			___atomic_xor_o = conch_atomic_cas(x, \
				___atomic_xor_c, \
				___atomic_xor_c ^ (v)); \
			if (___atomic_xor_o == ___atomic_xor_c) \
				break; \
			___atomic_xor_c = ___atomic_xor_o; \
		} while (1); \
	} while (0)


#ifdef __cplusplus
extern "C" {
#endif

/* c_atomic_cas.S */

extern
int32_t conch_atomic_cas(volatile int32_t *v, int32_t old, int32_t _new)
;

#ifdef __cplusplus
}
#endif


#endif
