/* @file: pbkdf2.h
 * #desc:
 *    The definitions of password-based key derivation function.
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

#ifndef _CONCH_PBKDF2_H
#define _CONCH_PBKDF2_H

#include <conch/config.h>
#include <conch/c_stdint.h>


#define PBKDF2_SHA3_224_TYPE 1
#define PBKDF2_SHA3_256_TYPE 2
#define PBKDF2_SHA3_384_TYPE 3
#define PBKDF2_SHA3_512_TYPE 4


#ifdef __cplusplus
extern "C" {
#endif

/* pbkdf2_blake2b.c */
extern
void conch_pbkdf2_blake2b(const uint8_t *pass, uint32_t pass_len,
		const uint8_t *salt, uint32_t salt_len, uint8_t *odk,
		uint32_t len, uint32_t k)
;

/* pbkdf2_blake2s.c */
extern
void conch_pbkdf2_blake2s(const uint8_t *pass, uint32_t pass_len,
		const uint8_t *salt, uint32_t salt_len, uint8_t *odk,
		uint32_t len, uint32_t k)
;

/* pbkdf2_sha1.c */
extern
void conch_pbkdf2_sha1(const uint8_t *pass, uint32_t pass_len,
		const uint8_t *salt, uint32_t salt_len, uint8_t *odk,
		uint32_t len, uint32_t k)
;

/* pbkdf2_sha256.c */
extern
void conch_pbkdf2_sha256(const uint8_t *pass, uint32_t pass_len,
		const uint8_t *salt, uint32_t salt_len, uint8_t *odk,
		uint32_t len, uint32_t k)
;
extern
void conch_pbkdf2_sha224(const uint8_t *pass, uint32_t pass_len,
		const uint8_t *salt, uint32_t salt_len, uint8_t *odk,
		uint32_t len, uint32_t k)
;

/* pbkdf2_sha3.c */
extern
int32_t conch_pbkdf2_sha3(const uint8_t *pass, uint32_t pass_len,
		const uint8_t *salt, uint32_t salt_len, uint8_t *odk,
		uint32_t len, uint32_t k, int32_t type)
;

/* pbkdf2_sha512.c */
extern
void conch_pbkdf2_sha512(const uint8_t *pass, uint32_t pass_len,
		const uint8_t *salt, uint32_t salt_len, uint8_t *odk,
		uint32_t len, uint32_t k)
;
extern
void conch_pbkdf2_sha384(const uint8_t *pass, uint32_t pass_len,
		const uint8_t *salt, uint32_t salt_len, uint8_t *odk,
		uint32_t len, uint32_t k)
;

#ifdef __cplusplus
}
#endif


#endif
