/* @file: blowfish.h
 * #desc:
 *    The definitions of blowfish symmetric-key block cipher.
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

#ifndef _CONCH_BLOWFISH_H
#define _CONCH_BLOWFISH_H

#include <conch/config.h>
#include <conch/c_stdint.h>


struct blowfish_ctx {
	uint32_t P[18];
	uint32_t S1[256];
	uint32_t S2[256];
	uint32_t S3[256];
	uint32_t S4[256];
};

#define BLOWFISH_NEW(x) struct blowfish_ctx x


#ifdef __cplusplus
extern "C" {
#endif

/* blowfish.c */
extern
void conch_blowfish_encrypt(struct blowfish_ctx *ctx,
		uint32_t *xl, uint32_t *xr)
;
extern
void conch_blowfish_decrypt(struct blowfish_ctx *ctx,
		uint32_t *xl, uint32_t *xr)
;
extern
void conch_blowfish_setkey(struct blowfish_ctx *ctx, const uint8_t *key,
		uint32_t len)
;
extern
void conch_blowfish_ekskey(struct blowfish_ctx *ctx, const uint8_t *data,
		uint32_t data_len, const uint8_t *key, uint32_t key_len)
;
extern
void conch_blowfish_init(struct blowfish_ctx *ctx, const uint8_t *key,
		uint32_t len)
;

#ifdef __cplusplus
}
#endif


#endif
