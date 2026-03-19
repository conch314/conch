/* @file: des.h
 * #desc:
 *    The definitions of data encryption standard.
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

#ifndef _CONCH_DES_H
#define _CONCH_DES_H

#include <conch/config.h>
#include <conch/c_stdint.h>


#define DES_KEYLEN 8
#define DES_BLOCKSIZE 8
#define DES_ENCRYPT 0
#define DES_DECRYPT 1

struct des_ctx {
	uint8_t key_k[17][8];
	uint8_t key_c[17][4];
	uint8_t key_d[17][4];
};

#define DES_NEW(x) struct des_ctx x


#ifdef __cplusplus
extern "C" {
#endif

/* des.c */
extern
void conch_des_init(struct des_ctx *ctx, const uint8_t *key)
;
extern
void conch_des_crypto(struct des_ctx *ctx, uint8_t *buf, int32_t is_decrypt)
;

#ifdef __cplusplus
}
#endif


#endif
