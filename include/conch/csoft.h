/* @file: csoft.h
 * #desc:
 *    The definitions of software mathematical operations in based
 *    on C.  only depends on bitwise operations, addition, subtraction,
 *    and multiplication.
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

#ifndef _CONCH_CSOFT_H
#define _CONCH_CSOFT_H

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* csoft.c */
extern
int32_t conch_csf_clz32(uint32_t x)
;
extern
int32_t conch_csf_clz64(uint64_t x)
;
extern
int32_t conch_csf_ctz32(uint32_t x)
;
extern
int32_t conch_csf_ctz64(uint64_t x)
;
extern
uint32_t conch_csf_udivmod32(uint32_t a, uint32_t b, uint32_t *rem)
;
extern
int32_t conch_csf_idivmod32(int32_t a, int32_t b, int32_t *rem)
;
extern
uint64_t conch_csf_udivmod64(uint64_t a, uint64_t b, uint64_t *rem)
;
extern
int64_t conch_csf_idivmod64(int64_t a, int64_t b, int64_t *rem)
;
extern
uint32_t conch_csf_udiv32(uint32_t a, uint32_t b)
;
extern
uint32_t conch_csf_umod32(uint32_t a, uint32_t b)
;
extern
int32_t conch_csf_idiv32(int32_t a, int32_t b)
;
extern
int32_t conch_csf_imod32(int32_t a, int32_t b)
;
extern
uint64_t conch_csf_udiv64(uint64_t a, uint64_t b)
;
extern
uint64_t conch_csf_umod64(uint64_t a, uint64_t b)
;
extern
int64_t conch_csf_idiv64(int64_t a, int64_t b)
;
extern
int64_t conch_csf_imod64(int64_t a, int64_t b)
;
extern
uint64_t conch_csf_umul64(uint64_t a, uint64_t b)
;
extern
int64_t conch_csf_smul64(int64_t a, int64_t b)
;
extern
void conch_csf_umul64_128(uint64_t *a, uint64_t *b)
;
extern
uint64_t conch_csf_fadd64(uint64_t a, uint64_t b)
;
extern
uint64_t conch_csf_fsub64(uint64_t a, uint64_t b)
;
extern
uint64_t conch_csf_fmul64(uint64_t a, uint64_t b)
;
extern
uint64_t conch_csf_fdiv64(uint64_t a, uint64_t b)
;
extern
int32_t conch_csf_f64_eq(uint64_t a, uint64_t b)
;
extern
int32_t conch_csf_f64_ne(uint64_t a, uint64_t b)
;
extern
int32_t conch_csf_f64_gt(uint64_t a, uint64_t b)
;
extern
int32_t conch_csf_f64_ge(uint64_t a, uint64_t b)
;
extern
int32_t conch_csf_f64_lt(uint64_t a, uint64_t b)
;
extern
int32_t conch_csf_f64_le(uint64_t a, uint64_t b)
;
extern
uint64_t conch_csf_i64_to_f64(int64_t a)
;
extern
uint64_t conch_csf_u64_to_f64(uint64_t a)
;
extern
int64_t conch_csf_f64_to_i64(uint64_t a, int32_t mode)
;
extern
uint64_t conch_csf_f64_to_u64(uint64_t a, int32_t mode)
;
extern
uint32_t conch_csf_fadd32(uint32_t a, uint32_t b)
;
extern
uint32_t conch_csf_fsub32(uint32_t a, uint32_t b)
;
extern
uint32_t conch_csf_fmul32(uint32_t a, uint32_t b)
;
extern
uint32_t conch_csf_fdiv32(uint32_t a, uint32_t b)
;
extern
int32_t conch_csf_f32_eq(uint32_t a, uint32_t b)
;
extern
int32_t conch_csf_f32_ne(uint32_t a, uint32_t b)
;
extern
int32_t conch_csf_f32_gt(uint32_t a, uint32_t b)
;
extern
int32_t conch_csf_f32_ge(uint32_t a, uint32_t b)
;
extern
int32_t conch_csf_f32_lt(uint32_t a, uint32_t b)
;
extern
int32_t conch_csf_f32_le(uint32_t a, uint32_t b)
;
extern
uint32_t conch_csf_i32_to_f32(int32_t a)
;
extern
uint32_t conch_csf_u32_to_f32(uint32_t a)
;
extern
int32_t conch_csf_f32_to_i32(uint32_t a, int32_t mode)
;
extern
uint32_t conch_csf_f32_to_u32(uint32_t a, int32_t mode)
;

#ifdef __cplusplus
}
#endif


#endif
