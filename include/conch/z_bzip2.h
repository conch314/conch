/* @file: z_bzip2.h
 * #desc:
 *    The definitions of bzip2 compression algorithm.
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

#ifndef _CONCH_Z_BZIP2_H
#define _CONCH_Z_BZIP2_H

#include <conch/config.h>
#include <conch/c_stdint.h>
#include <conch/bits_add.h>


/* max block size */
#define BZIP2_BLOCKSIZE_MAX 900000
/* number of selector groups */
#define BZIP2_SELECTOR (BZIP2_BLOCKSIZE_MAX / 50 + 2)
/* number of alpha symbol */
#define BZIP2_ALPHA_SIZE 258

/* temporary size for block sorting */
#define BZIP2_SORT_TMPSIZE \
	(BZIP2_BLOCKSIZE_MAX + (BZIP2_BLOCKSIZE_MAX / 32 + 3))

/* output buffer size */
#define BZIP2_BUFSIZE (BZIP2_BLOCKSIZE_MAX * 2)

/* flush buffer */
#define BZIP2_IS_FLUSH 1
/* flush buffer and end */
#define BZIP2_IS_END 2

struct bzip2_ctx {
	uint8_t block[BZIP2_BLOCKSIZE_MAX]; /* input block */
	uint32_t block_max; /* max input length */
	uint32_t block_len; /* block length */

	uint32_t block_crc;
	uint32_t combined_crc;
	const uint32_t *crc_t;

	/* input run-length */
	uint32_t rle_inchr;
	uint32_t rle_inlen;
	uint8_t inuse[256]; /* characters in use */

	/* block sorting */
	uint32_t sort_sa[BZIP2_BLOCKSIZE_MAX]; /* suffix array */
	uint32_t sort_tmp[BZIP2_SORT_TMPSIZE];
	uint32_t orig_index; /* primary index */

	/* move-to-front */
	uint16_t *mtf_v;
	uint16_t mtf_e;         /* end-block value */
	uint32_t mtf_n;
	uint32_t mtf_freq[258]; /* freq of value */

	/* huffman coding */
	uint8_t huf_len[6][BZIP2_ALPHA_SIZE];
	uint32_t huf_code[6][BZIP2_ALPHA_SIZE];
	uint32_t huf_rfreq[6][BZIP2_ALPHA_SIZE];
	uint8_t selector[BZIP2_SELECTOR];
	uint8_t selector_mtf[BZIP2_SELECTOR];

	const uint8_t *s; /* input buffer */
	uint32_t s_len;   /* input length */

	struct bits_add_ctx bits_ctx;
	int32_t lev;
	int32_t flush;

	uint8_t buf[BZIP2_BUFSIZE]; /* output buffer */
	uint32_t len;
};

#define BZIP2_NEW(x) struct bzip2_ctx x

#define BZIP2_BUF(x) ((x)->buf)
#define BZIP2_LEN(x) ((x)->len)


#ifdef __cplusplus
extern "C" {
#endif

/* z_bzip2.c */
extern
int32_t conch_bzip2_init(struct bzip2_ctx *ctx, int32_t lev)
;
extern
int32_t conch_bzip2(struct bzip2_ctx *ctx, const uint8_t *s,
		uint32_t len, int32_t flush)
;

#ifdef __cplusplus
}
#endif


#endif
