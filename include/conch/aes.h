/* @file: aes.h
 * #desc:
 *    The definitions of advanced encryption standard.
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

#ifndef _CONCH_AES_H
#define _CONCH_AES_H

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>


/* aes128 */
#define AES_128_TYPE 0
#define AES_128_KEY_LEN 16
#define AES_128_ROUNDS 10
/* aes192 */
#define AES_192_TYPE 1
#define AES_192_KEY_LEN 24
#define AES_192_ROUNDS 12
/* aes256 */
#define AES_256_TYPE 2
#define AES_256_KEY_LEN 32
#define AES_256_ROUNDS 14

#define AES_BLOCKSIZE 16
#define AES_KEYEXP_LEN (AES_BLOCKSIZE * (AES_256_ROUNDS + 1))

struct aes_ctx {
	uint8_t keyexp[AES_KEYEXP_LEN];
	uint16_t keylen;
	uint16_t rounds;
};

#define AES_NEW(x) struct aes_ctx x


#ifdef __cplusplus
extern "C" {
#endif

/* aes_ext.c */
extern
void conch_aes_encrypt_cbc(struct aes_ctx *ctx, uint8_t *iv,
		uint8_t *state)
;
extern
void conch_aes_decrypt_cbc(struct aes_ctx *ctx, uint8_t *iv,
		uint8_t *state)
;
extern
void conch_aes_encrypt_cfb(struct aes_ctx *ctx, uint8_t *iv,
		uint8_t *state)
;
extern
void conch_aes_decrypt_cfb(struct aes_ctx *ctx, uint8_t *iv,
		uint8_t *state)
;
extern
void conch_aes_crypto_ofb(struct aes_ctx *ctx, uint8_t *iv,
		uint8_t *buf, size_t len)
;
extern
void conch_aes_crypto_ctr(struct aes_ctx *ctx, uint8_t *ran,
		uint8_t *buf, size_t len)
;

/* aes_sbox.c */
extern
int32_t conch_aes_init(struct aes_ctx *ctx, const uint8_t *key,
		int32_t type)
;
extern
void conch_aes_encrypt(struct aes_ctx *ctx, uint8_t *state)
;
extern
void conch_aes_decrypt(struct aes_ctx *ctx, uint8_t *state)
;

#ifdef __cplusplus
}
#endif


#endif
