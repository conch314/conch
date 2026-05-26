/* @file: z_unbzip2.h
 * #desc:
 *    The definitions of bzip2 decompression.
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

#ifndef _CONCH_Z_UNBZIP2_H
#define _CONCH_Z_UNBZIP2_H

#include <conch/config.h>
#include <conch/c_stdint.h>
#include <conch/bits_get.h>


/* max block size */
#define UNBZIP2_BLOCKSIZE_MAX 900000

/* number of the alpha symbol */
#define UNBZIP2_ALPHA_SIZE 258
/* max code length */
#define UNBZIP2_CODE_LEN_MAX 23

/* number of the huffman groups */
#define UNBZIP2_NGROUPS 6
/* group segment size */
#define UNBZIP2_SGROUPS_SIZE 50
/* number of the groups selectors */
#define UNBZIP2_NSELECTORS \
	(UNBZIP2_BLOCKSIZE_MAX / UNBZIP2_SGROUPS_SIZE + 2)

/* flush buffer */
#define UNBZIP2_IS_FLUSH 1
/* flush buffer and end */
#define UNBZIP2_IS_END 2

/* data incomplete */
#define UNBZIP2_ERR_INCOMP -1
/* block header error */
#define UNBZIP2_ERR_HEAD -2
/* block header crc error */
#define UNBZIP2_ERR_HEAD_CRC -3
/* end header crc error */
#define UNBZIP2_ERR_END_CRC -4
/* primary index error */
#define UNBZIP2_ERR_ORIG_INDEX -5
/* ngroups size error */
#define UNBZIP2_ERR_NGROUPS -6
/* nselectors size error */
#define UNBZIP2_ERR_NSELECTORS -7
/* selector mtf error */
#define UNBZIP2_ERR_SELECTOR_MTF -8
/* huffman length error */
#define UNBZIP2_ERR_HUFFMAN_LEN -9
/* huffman code error */
#define UNBZIP2_ERR_HUFFMAN_CODE -10

struct unbzip2_ctx {
	uint8_t block[UNBZIP2_BLOCKSIZE_MAX]; /* input block (after rle) */
	uint32_t block_max; /* max block length */
	uint32_t block_len; /* block length */

	/* crc */
	uint32_t block_crc;
	uint32_t combined_crc;
	uint32_t block_head_crc;
	const uint32_t *crc_t;

	/* input run-length decoding */
	uint32_t rle_inchr; /* repeat character */
	uint32_t rle_inlen; /* repeat length */
	uint8_t inuse[256]; /* characters in use */
	uint8_t inuse16[16];

	/* move-to-front */
	uint16_t *mtf_v; /* mtf value */
	uint16_t mtf_e;  /* end-block value */
	uint32_t mtf_n;  /* number of the mtf value */

	/* inverse burrows–wheeler transform */
	uint32_t rank_tmp[UNBZIP2_BLOCKSIZE_MAX]; /* temporary buffer */
	uint32_t orig_index; /* primary index */

	/* huffman decoding */
	uint8_t huf_len[UNBZIP2_NGROUPS][UNBZIP2_ALPHA_SIZE];
	int32_t huf_code[UNBZIP2_NGROUPS][UNBZIP2_ALPHA_SIZE];
	int32_t huf_limit[UNBZIP2_NGROUPS][UNBZIP2_ALPHA_SIZE];
	int32_t huf_base[UNBZIP2_NGROUPS][UNBZIP2_ALPHA_SIZE];
	int32_t huf_perm[UNBZIP2_NGROUPS][UNBZIP2_ALPHA_SIZE];
	int32_t huf_min[UNBZIP2_NGROUPS];

	/* huffman groups selectors */
	uint8_t selector[UNBZIP2_NSELECTORS];
	uint8_t selector_mtf[UNBZIP2_NSELECTORS];
	int32_t ngroups;
	int32_t nselectors;

	struct bits_get_ctx bits_ctx;
	const uint8_t *s; /* input buffer */
	uint32_t s_len;   /* input length */

	uint32_t t_len;
	int32_t t_i;
	int32_t t_j;
	int32_t t_k;
	int32_t t_n;

	uint32_t block_count; /* block counter */
	int32_t last;
	int32_t state;
	int32_t flush;

	uint8_t buf[UNBZIP2_BLOCKSIZE_MAX]; /* output buffer */
	uint32_t len;
};

#define UNBZIP2_NEW(x) struct unbzip2_ctx x

#define UNBZIP2_BUF(x) ((x)->buf)
#define UNBZIP2_LEN(x) ((x)->len)
#define UNBZIP2_COUNT(x) ((x)->block_count)

/* inflate tail offset of input buffer */
#define UNBZIP2_OFFSET(x, n) \
	((n) - (x)->s_len - BITS_GET_REMLEN(&(x)->bits_ctx))


#ifdef __cplusplus
extern "C" {
#endif

/* z_unbzip2.c */
extern
int32_t conch_unbzip2_init(struct unbzip2_ctx *ctx, int32_t lev)
;
extern
int32_t conch_unbzip2(struct unbzip2_ctx *ctx, const uint8_t *s,
		uint32_t len, int32_t flush)
;

#ifdef __cplusplus
}
#endif


#endif
