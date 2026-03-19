/* @file: rc4.h
 * #desc:
 *    The definitions of rivest cipher 4 encryption algorithm.
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

#ifndef _CONCH_RC4_H
#define _CONCH_RC4_H

#include <conch/config.h>
#include <conch/c_stdint.h>


struct rc4_ctx {
	uint8_t s[256];
	int32_t i, j;
};

#define RC4_NEW(x) struct rc4_ctx x


#ifdef __cplusplus
extern "C" {
#endif

/* rc4.c */
extern
void conch_rc4_init(struct rc4_ctx *ctx, const uint8_t *key, uint32_t len);
extern
void conch_rc4_crypto(struct rc4_ctx *ctx, uint8_t *buf, uint32_t len);

#ifdef __cplusplus
}
#endif


#endif
