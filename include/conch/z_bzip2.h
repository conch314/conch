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

/* number of the alpha symbol */
#define BZIP2_ALPHA_SIZE 258
/* max code length */
#define BZIP2_CODE_LEN_MAX 23

/* number of the iterations */
#define BZIP2_ITERS 4
/* number of the huffman groups */
#define BZIP2_NGROUPS 6
/* group segment size */
#define BZIP2_SGROUPS_SIZE 50
/* number of the groups selectors */
#define BZIP2_NSELECTORS \
	(BZIP2_BLOCKSIZE_MAX / BZIP2_SGROUPS_SIZE + 2)

/* temporary size for block sorting */
#define BZIP2_SORT_TMPSIZE \
	(BZIP2_BLOCKSIZE_MAX + (BZIP2_BLOCKSIZE_MAX / 32 + 3))

/* flush buffer */
#define BZIP2_IS_FLUSH 1
/* flush buffer and end */
#define BZIP2_IS_END 2

struct bzip2_ctx {
	uint8_t block[BZIP2_BLOCKSIZE_MAX]; /* input block (after rle) */
	uint32_t block_max; /* max block length */
	uint32_t block_len; /* block length */

	/* crc */
	uint32_t block_crc;
	uint32_t combined_crc;
	const uint32_t *crc_t;

	/* input run-length */
	uint32_t rle_inchr; /* repeat character */
	uint32_t rle_inlen; /* repeat length */
	uint8_t inuse[256]; /* characters in use */

	/* block sorting */
	uint32_t sort_sa[BZIP2_BLOCKSIZE_MAX]; /* suffix array */
	uint32_t sort_tmp[BZIP2_SORT_TMPSIZE]; /* temporary buffer */
	uint32_t orig_index; /* primary index */

	/* move-to-front */
	uint16_t *mtf_v;        /* mtf value */
	uint16_t mtf_e;         /* end-block value */
	uint32_t mtf_n;         /* number of the mtf value */
	uint32_t mtf_freq[258]; /* freq of the mtf value */

	/* huffman coding */
	uint8_t huf_len[BZIP2_NGROUPS][BZIP2_ALPHA_SIZE];
	uint32_t huf_code[BZIP2_NGROUPS][BZIP2_ALPHA_SIZE];
	uint32_t huf_rfreq[BZIP2_NGROUPS][BZIP2_ALPHA_SIZE];

	/* huffman groups selectors */
	uint8_t selector[BZIP2_NSELECTORS];
	uint8_t selector_mtf[BZIP2_NSELECTORS];
	int32_t ngroups;
	int32_t nselectors;

	const uint8_t *s; /* input buffer */
	uint32_t s_len;   /* input length */

	struct bits_add_ctx bits_ctx;
	uint32_t block_count; /* block counter */
	int32_t flush;

	uint8_t buf[BZIP2_BLOCKSIZE_MAX * 2]; /* output buffer */
	uint32_t len;
};

#define BZIP2_NEW(x) struct bzip2_ctx x

#define BZIP2_BUF(x) ((x)->buf)
#define BZIP2_LEN(x) ((x)->len)
#define BZIP2_COUNT(x) ((x)->block_count)


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
