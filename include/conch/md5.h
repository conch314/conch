/* @file: md5.h
 * #desc:
 *    The definitions of md5 hash algorithm.
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

#ifndef _CONCH_MD5_H
#define _CONCH_MD5_H

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>


#define MD5_LEN 16
#define MD5_BLOCKSIZE 64

struct md5_ctx {
	uint32_t state[4];
	uint32_t count;
	uint8_t buf[MD5_BLOCKSIZE];
};

#define	MD5_NEW(x) struct md5_ctx x
#define MD5_STATE(x, n) (((uint8_t *)((x)->state))[n])


#ifdef __cplusplus
extern "C" {
#endif

/* md5.c */
extern
void conch_md5_init(struct md5_ctx *ctx)
;
extern
void conch_md5_process(struct md5_ctx *ctx, const uint8_t *s,
		size_t len)
;
extern
void conch_md5_finish(struct md5_ctx *ctx, uint64_t len)
;
extern
void conch_md5(struct md5_ctx *ctx, const uint8_t *s, size_t len)
;

#ifdef __cplusplus
}
#endif


#endif
