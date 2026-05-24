/* @file: z_unbzip2.c
 * #desc:
 *    The implementations of.
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

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>
#include <conch/c_string.h>
#include <conch/c_stdio.h>
#include <conch/c_getopt.h>
#include <conch/bits_get.h>
#include <conch/crc.h>


#define UNBZIP2_BLOCKSIZE_MAX 900000
#define UNBZIP2_ALPHA_SIZE 258
#define UNBZIP2_CODE_LEN_MAX 23
#define UNBZIP2_NGROUPS 6
#define UNBZIP2_SGROUPS_SIZE 50
#define UNBZIP2_NSELECTORS \
	(UNBZIP2_BLOCKSIZE_MAX / UNBZIP2_SGROUPS_SIZE + 2)

struct unbzip2_ctx {
	uint8_t block[UNBZIP2_BLOCKSIZE_MAX];
	uint32_t block_max;
	uint32_t block_len;

	uint32_t block_crc;
	uint32_t block_crc2;
	uint32_t combined_crc;
	const uint32_t *crc_t;

	/* input run-length */
	uint32_t rle_inchr; /* repeat character */
	uint32_t rle_inlen; /* repeat length */
	uint8_t inuse[256]; /* characters in use */
	uint8_t inuse16[16];

	uint16_t *mtf_v;        /* mtf value */
	uint16_t mtf_e;
	uint32_t mtf_n;         /* number of the mtf value */

	uint32_t rank_tmp[UNBZIP2_BLOCKSIZE_MAX]; /* temporary buffer */
	uint32_t orig_index;

	uint8_t huf_len[UNBZIP2_NGROUPS][UNBZIP2_ALPHA_SIZE];
	int32_t huf_code[UNBZIP2_NGROUPS][UNBZIP2_ALPHA_SIZE];
	int32_t huf_limit[UNBZIP2_NGROUPS][UNBZIP2_ALPHA_SIZE];
	int32_t huf_base[UNBZIP2_NGROUPS][UNBZIP2_ALPHA_SIZE];
	int32_t huf_perm[UNBZIP2_NGROUPS][UNBZIP2_ALPHA_SIZE];
	int32_t huf_min[UNBZIP2_NGROUPS];

	uint8_t selector[UNBZIP2_NSELECTORS];
	uint8_t selector_mtf[UNBZIP2_NSELECTORS];
	int32_t ngroups;
	int32_t nselectors;

	struct bits_get_ctx bits_ctx;
	const uint8_t *s; /* input buffer */
	uint32_t s_len;   /* input length */

	int32_t t_i;
	int32_t t_j;
	int32_t t_k;
	int32_t t_n;
	int32_t t_l;

	uint32_t block_count; /* block counter */
	int32_t last;
	int32_t state;
	int32_t flush;

	uint8_t buf[UNBZIP2_BLOCKSIZE_MAX]; /* output buffer */
	uint32_t len;
};

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
/* end header error */
#define UNBZIP2_ERR_END_HEAD -4
/* primary index error */
#define UNBZIP2_ERR_ORIG_INDEX -5
/* ngroups too size error */
#define UNBZIP2_ERR_NGROUPS -6
/* nselectors too size error */
#define UNBZIP2_ERR_NSELECTORS -7
/* nselectors mtf error */
#define UNBZIP2_ERR_NSELECTORS_MTF -8
/* huffman code error */
#define UNBZIP2_ERR_HUFFMAN_CODE -9

#define UNBZIP2_NEW(x) struct unbzip2_ctx x

#define UNBZIP2_BUF(x) ((x)->buf)
#define UNBZIP2_LEN(x) ((x)->len)
#define UNBZIP2_COUNT(x) ((x)->block_count)

/* inflate tail offset of input buffer */
#define UNBZIP2_OFFSET(x, n) \
	((n) - (x)->s_len - BITS_GET_REMLEN(&(x)->bits_ctx))

#define BITS_FILL(s, f) \
	do { \
		if (_bits_fill(s) && !f) \
			goto e; \
	} while (0)
#define BITS_PEEK(s, v, n) \
	do { \
		if (_bits_peek(s, v, n)) \
			return UNBZIP2_ERR_INCOMP; \
	} while (0)
#define BITS_PEEK2(s, v, t, m, g) \
	do { \
		t = g; \
		if (_bits_peek(s, v, t)) { \
			t = m; \
			if (_bits_peek(s, v, t)) \
				return UNBZIP2_ERR_INCOMP; \
		} \
	} while (0)
#define BITS_DUMP(s, v, n) \
	do { \
		if (_bits_dump(s, v, n)) \
			return UNBZIP2_ERR_INCOMP; \
	} while (0)


/* @func: _bits_fill (static)
 * #desc:
 *    fill the bits buffer.
 *
 * #1: ctx [in/out] unbzip2 struct context
 * #r:     [ret]    0: no error, 1: remaining bits are less than the expected
 */
static int32_t _bits_fill(struct unbzip2_ctx *ctx)
{
	uint32_t n = conch_bits_getfill(&ctx->bits_ctx, ctx->s, ctx->s_len);
	ctx->s += n;
	ctx->s_len -= n;

	if (!ctx->s_len && BITS_GET_REMLEN(&ctx->bits_ctx) < 4)
		return 1;

	return 0;
}

/* @func: _bits_peek (static)
 * #desc:
 *    peek at the bits in the buffer.
 *
 * #1: ctx [in/out] unbzip2 struct context
 * #2: v   [out]    bits value
 * #3: len [in]     bits length
 * #r:     [ret]    0: no error, -1: bits of no extra
 */
static int32_t _bits_peek(struct unbzip2_ctx *ctx, uint32_t *v, uint32_t len)
{
	if (conch_bits_beget(&ctx->bits_ctx, v, len, 1))
		return -1;

	return 0;
}

/* @func: _bits_dump (static)
 * #desc:
 *    bits in the dump buffer.
 *
 * #1: ctx [in/out] unbzip2 struct context
 * #2: v   [out]    bits value
 * #3: len [in]     bits length
 * #r:     [ret]    0: no error, -1: bits of no extra
 */
static int32_t _bits_dump(struct unbzip2_ctx *ctx, uint32_t *v, uint32_t len)
{
	if (conch_bits_beget(&ctx->bits_ctx, v, len, 0))
		return -1;

	return 0;
}

void bwt_inverse_lf_mapping(uint8_t *out, const uint8_t *in, uint32_t len,
		uint32_t index, uint32_t *rank)
{
	uint32_t count[256], occ[256];
	uint32_t c, n = 0;

	conch_memset(count, 0, sizeof(count));
	conch_memset(occ, 0, sizeof(occ));

	/* statistical the freq */
	for (uint32_t i = 0; i < len; i++)
		count[in[i]]++;

	/* offset */
	for (uint32_t i = 0; i < 256; i++) {
		c = count[i];
		count[i] = n;
		n += c;
	}

	/* build rank (last -> first) */
	for (uint32_t i = 0; i < len; i++) {
		c = in[i];
		rank[i] = count[c] + occ[c];
		occ[c]++;
	}

	/* reverse */
	for (uint32_t i = len; i > 0; ) {
		out[--i] = in[index];
		index = rank[index];
	}
}

/* mtf decoding */
uint32_t mtfd(uint8_t *out, uint16_t *in, uint32_t len, uint8_t *inuse)
{
	uint8_t tab[256], seq[256], c;
	uint32_t pos, e, n = 0, w = 0;

	/* make a sequence table */
	for (uint32_t i = 0; i < 256; i++) {
		if (inuse[i]) {
			seq[n] = (uint8_t)i;
			n++;
		}
	}

	/* characters set */
	for (uint32_t i = 0; i < n; i++)
		tab[i] = (uint8_t)i;

	for (uint32_t i = 0; i < len; i++) {
		pos = in[i];
		if (pos < 2) { /* zero rle */
			n = 0;
			e = 1;
			while (1) {
				n += e << (in[i++] & 1);
				e <<= 1;
				if (i >= len)
					break;
				if (in[i] > 1)
					break;
			}
			i--;

			c = seq[tab[0]];
			while (n--)
				out[w++] = c;
		} else {
			c = tab[--pos];
			out[w++] = seq[c];

			for (; pos > 0; pos--) /* move */
				tab[pos] = tab[pos - 1];
			tab[0] = c;
		}
	}

	return w;
}

void _gen_se(struct unbzip2_ctx *ctx)
{
	uint8_t tab[UNBZIP2_NGROUPS], pos, c;

	for (int32_t i = 0; i < ctx->ngroups; i++)
		tab[i] = (uint8_t)i;

	for (int32_t i = 0; i < ctx->nselectors; i++) {
		pos = ctx->selector_mtf[i];

		c = tab[pos];
		ctx->selector[i] = c;

		for (; pos > 0; pos--) /* move */
			tab[pos] = tab[pos - 1];
		tab[0] = c;
	}
}

static void _add_to_buffer(struct unbzip2_ctx *ctx, uint32_t len)
{
	uint32_t c = ctx->rle_inchr;

	switch (ctx->rle_inlen) {
		case 3:
			ctx->buf[ctx->len++] = (uint8_t)c;
		case 2:
			ctx->buf[ctx->len++] = (uint8_t)c;
		case 1:
			ctx->buf[ctx->len++] = (uint8_t)c;
			break;
		default:
			ctx->buf[ctx->len++] = (uint8_t)c;
			ctx->buf[ctx->len++] = (uint8_t)c;
			ctx->buf[ctx->len++] = (uint8_t)c;
			ctx->buf[ctx->len++] = (uint8_t)c;

			while (len--)
				ctx->buf[ctx->len++] = (uint8_t)c;
			break;
	}
}

void _gen_createdecodetables(int32_t *limit,
		int32_t *base,
		int32_t *perm,
		uint8_t *length,
		int32_t minlen,
		int32_t maxlen,
		int32_t alpha_size)
{
	int32_t p = 0;
	for (int32_t i = minlen; i <= maxlen; i++) {
		for (int32_t j = 0; j < alpha_size; j++) {
			if (length[j] == i)
				perm[p++] = j;
		}
	}

	for (int32_t i = 0; i < 23; i++)
		base[i] = 0;

	for (int32_t i = 0; i < alpha_size; i++)
		base[length[i] + 1]++;

	for (int32_t i = 1; i < 23; i++)
		base[i] += base[i - 1];

	for (int32_t i = 0; i < 23; i++)
		limit[i] = 0;

	uint32_t vec = 0;
	for (int32_t i = minlen; i <= maxlen; i++) {
		vec += (base[i + 1] - base[i]);
		limit[i] = vec - 1;
		vec <<= 1;
	}

	for (int32_t i = minlen + 1; i <= maxlen; i++)
		base[i] = ((limit[i - 1] + 1) << 1) - base[i];
}

void _build_symbol(struct unbzip2_ctx *ctx, int32_t alpha_size)
{
	for (int32_t i = 0; i < ctx->ngroups; i++) {
		int32_t min_len = 32;
		int32_t max_len = 0;
		for (int32_t j = 0; j < alpha_size; j++) {
			if (ctx->huf_len[i][j] > max_len)
				max_len = ctx->huf_len[i][j];
			if (ctx->huf_len[i][j] < min_len)
				min_len = ctx->huf_len[i][j];
		}
		_gen_createdecodetables(ctx->huf_limit[i],
				ctx->huf_base[i],
				ctx->huf_perm[i],
				ctx->huf_len[i],
				min_len,
				max_len,
				alpha_size);
		ctx->huf_min[i] = min_len;
	}
}

/* @func: _unbzip2_block (static)
 * #desc:
 *    unbzip2 block function.
 *
 * #1: ctx   [in/out] unbzip2 struct context
 * #2: s     [in]     input buffer
 * #3: len   [in]     input length
 * #4: flush [in]     is finish
 * #r:       [ret]
 *    0: no error, >0 IS_FLUSH: flush block, IS_END: flush block and end,
 *    <0: ERR_INCOMP ...
 */
static int32_t _unbzip2_block(struct unbzip2_ctx *ctx, const uint8_t *s,
		uint32_t len, int32_t flush)
{
	if (!ctx->flush) {
		ctx->s = s;
		ctx->s_len = len;
	} else { /* continue */
		ctx->flush = 0;
		ctx->len = 0;
	}

	uint32_t v, t;
	do {
		switch (ctx->state) {
			case 0:
				BITS_FILL(ctx, flush);
				BITS_DUMP(ctx, &v, 16);
				if (v == 0x3141) {
					ctx->state = 1;
				} else if (v == 0x1772) {
					ctx->state = 2;
				} else {
					return UNBZIP2_ERR_HEAD;
				}
				ctx->block_count++; /* block counter */
				break;
			case 1:
				BITS_FILL(ctx, flush);
				BITS_DUMP(ctx, &v, 32);
				if (v != 0x59265359)
					return UNBZIP2_ERR_HEAD;

				ctx->state = 4;
				break;
			case 2:
				BITS_FILL(ctx, flush);
				BITS_DUMP(ctx, &v, 32);
				if (v != 0x45385090)
					return UNBZIP2_ERR_HEAD;

				ctx->state = 3;
			case 3: /* end header */
				BITS_FILL(ctx, flush);
				BITS_DUMP(ctx, &v, 32);
				if (v != ctx->combined_crc)
					return UNBZIP2_ERR_END_HEAD;

				ctx->last = 1;
				return UNBZIP2_IS_END;
			case 4: /* block header */
				BITS_FILL(ctx, flush);
				BITS_DUMP(ctx, &v, 32);
				ctx->block_crc2 = v;
				ctx->block_crc = 0xffffffff;
				ctx->state = 5;
			case 5:
				BITS_FILL(ctx, flush);
				BITS_DUMP(ctx, &v, 1); /* random flag */
				BITS_DUMP(ctx, &v, 24);

				ctx->orig_index = v;
				if (v >= ctx->block_max)
					return UNBZIP2_ERR_ORIG_INDEX;

				ctx->t_i = 0;
				ctx->state = 6;
			case 6: /* inuse */
				BITS_FILL(ctx, flush);
				while (ctx->t_i < 16) {
					BITS_DUMP(ctx, &v, 1);
					ctx->inuse16[ctx->t_i++] = v;
				}

				conch_memset(ctx->inuse, 0,
					sizeof(ctx->inuse));

				ctx->t_i = 0;
				ctx->t_k = 0;
				ctx->state = 7;
			case 7:
				ctx->state = 10;
				while (ctx->t_i < 16) {
					if (ctx->inuse16[ctx->t_i++]) {
						ctx->state = 8;
						break;
					} else {
						ctx->t_k += 16;
					}
				}
				break;
			case 8: /* non-zero */
				BITS_FILL(ctx, flush);

				ctx->t_j = 0;
				while (ctx->t_j < 16) {
					BITS_DUMP(ctx, &v, 1);
					ctx->inuse[ctx->t_k++] = v;
					ctx->t_j++;
				}

				ctx->state = 7;
				break;
			case 10:
				BITS_FILL(ctx, flush);

				BITS_DUMP(ctx, &v, 3);
				ctx->ngroups = v;
				if (v > UNBZIP2_NGROUPS)
					return UNBZIP2_ERR_NGROUPS;

				BITS_DUMP(ctx, &v, 15);
				ctx->nselectors = v;
				if (v > UNBZIP2_NSELECTORS)
					return UNBZIP2_ERR_NSELECTORS;

				conch_memset(ctx->selector_mtf, 0,
					sizeof(ctx->selector_mtf));

				ctx->t_k = 0;
				ctx->state = 11;
			case 11: /* selectors mtf */
				ctx->state = 13;
				if (ctx->t_k < ctx->nselectors)
					ctx->state = 12;
				break;
			case 12:
				BITS_FILL(ctx, flush);

				ctx->t_j = 0;
				for (; ctx->t_j < (UNBZIP2_NGROUPS + 1);
						ctx->t_j++) {
					BITS_DUMP(ctx, &v, 1);
					ctx->selector_mtf[ctx->t_k] += v;
					if (!v)
						break;
				}

				if (ctx->t_j > UNBZIP2_NGROUPS)
					return UNBZIP2_ERR_NSELECTORS_MTF;

				ctx->t_k++;
				ctx->state = 11;
				break;
			case 13:
				_gen_se(ctx);


				t = 0;
				for (uint32_t i = 0; i < 256; i++) {
					if (ctx->inuse[i])
						t++;
				}
				ctx->mtf_e = t + 1;
				ctx->t_i = 0;
				ctx->state = 14;
			case 14: /* huffman length */
				BITS_FILL(ctx, flush);
				BITS_DUMP(ctx, &v, 5);
				ctx->t_k = v;
				ctx->t_j = 0;
				ctx->state = 16;
			case 16:
				BITS_FILL(ctx, flush);

				while (ctx->t_k < 20) {
					BITS_DUMP(ctx, &v, 1);
					if (!v)
						break;

					BITS_DUMP(ctx, &v, 1);
					if (v) {
						ctx->t_k--;
					} else {
						ctx->t_k++;
					}

					if (ctx->t_k < 1)
						return UNBZIP2_ERR_HUFFMAN_CODE;
				}

				ctx->huf_len[ctx->t_i][ctx->t_j++] = ctx->t_k;
				if (ctx->t_j > ctx->mtf_e) {
					ctx->state = 14;
					ctx->t_i++;
					if (ctx->t_i >= ctx->ngroups)
						ctx->state = 17;
				}
				break;
			case 17: /* decoding */
				ctx->mtf_n = 0;
				ctx->t_n = 0;	
				_build_symbol(ctx, ctx->mtf_e + 1);
			case 18:
				ctx->t_k = 0;
				ctx->t_i = ctx->selector[ctx->t_n++];
				ctx->state = 19;
			case 19:
				BITS_FILL(ctx, flush);

				ctx->t_j = ctx->huf_min[ctx->t_i];
				BITS_DUMP(ctx, &v, ctx->t_j);
				t = v;
				while (1) {
					if (ctx->t_j > 20)
						return UNBZIP2_ERR_HUFFMAN_CODE;
					if (t <= ctx->huf_limit[ctx->t_i][ctx->t_j])
						break;

					BITS_DUMP(ctx, &v, 1);
					t = (t << 1) | v;
					ctx->t_j++;
				}

				t -= ctx->huf_base[ctx->t_i][ctx->t_j];
				if (t >= 258)
					return UNBZIP2_ERR_HUFFMAN_CODE;

				t = ctx->huf_perm[ctx->t_i][t];
				ctx->mtf_v[ctx->mtf_n++] = t;
				ctx->t_k++;

				if (t == ctx->mtf_e) {
					ctx->state = 20;
				} else if (ctx->t_k == UNBZIP2_SGROUPS_SIZE) {
					ctx->state = 18;
					if (ctx->t_n >= ctx->nselectors)
						ctx->state = 20;
				}
				break;
			case 20:
				ctx->block_len = mtfd(ctx->buf,
					ctx->mtf_v,
					ctx->mtf_n - 1,
					ctx->inuse);
				bwt_inverse_lf_mapping(ctx->block,
					ctx->buf,
					ctx->block_len,
					ctx->orig_index,
					ctx->rank_tmp);
				ctx->state = 21;
			case 21:
				ctx->t_n = 0;
				ctx->rle_inchr = 256;
				ctx->rle_inlen = 0;
				ctx->state = 22;
			case 22:
				if (ctx->len > (UNBZIP2_BLOCKSIZE_MAX - 512)) {
					ctx->flush = 1;
					ctx->block_crc = conch_crc32_msb(ctx->crc_t,
						ctx->block_crc, ctx->buf, ctx->len);
					return UNBZIP2_IS_FLUSH;
				}
				if (ctx->t_n == ctx->block_len) { /* end-block */
					if (ctx->rle_inlen && ctx->rle_inchr < 256)
						_add_to_buffer(ctx, 0);
					ctx->state = 23;
					ctx->flush = 1;
					ctx->block_crc = conch_crc32_msb(ctx->crc_t,
						ctx->block_crc, ctx->buf, ctx->len);
					return UNBZIP2_IS_FLUSH;
				}

				v = ctx->block[ctx->t_n++];
				if (ctx->rle_inlen == 4) {
					_add_to_buffer(ctx, v);
					ctx->rle_inchr = 256;
					ctx->rle_inlen = 0;
				} else if (v != ctx->rle_inchr) {
					if (ctx->rle_inchr < 256)
						_add_to_buffer(ctx, 0);
					ctx->rle_inchr = v;
					ctx->rle_inlen = 1;
				} else {
					ctx->rle_inlen++;
				}
				break;
			case 23:
				ctx->block_crc = ~ctx->block_crc;
				ctx->combined_crc = (ctx->combined_crc << 1)
					| (ctx->combined_crc >> 31);
				ctx->combined_crc ^= ctx->block_crc;

				if (ctx->block_crc2 != ctx->block_crc)
					return UNBZIP2_ERR_HEAD_CRC;

				ctx->state = 0;
				break;
			default: /* state error */
				break;
		}
	} while (1);
e:

	return 0;
}

/* @func: conch_unbzip2_init
 * #desc:
 *    unbzip2 initialization.
 *
 * #1: ctx [out] unbzip2 struct context
 * #2: lev [in]  compress level
 * #r:     [ret] 0: no error, -1: level error
 */
int32_t conch_unbzip2_init(struct unbzip2_ctx *ctx, int32_t lev)
{
	if (!(lev >= 1 && lev <= 9))
		return -1;

	ctx->block_max = 100000 * lev;
	ctx->mtf_v = (uint16_t *)ctx->rank_tmp; /* reuse */

	/* crc */
	ctx->combined_crc = 0;
	ctx->crc_t = conch_crc32_table(CRC32_DEFAULT_MSB_TYPE);

	BITS_GET_INIT(&ctx->bits_ctx);
	ctx->block_count = 0;
	ctx->last = 0;
	ctx->state = 0;
	ctx->flush = 0;
	ctx->len = 0;

	return 0;
}

/* @func: conch_unbzip2
 * #desc:
 *    bzip2 decompression function.
 *
 * #1: ctx   [in/out] unbzip2 struct context
 * #2: s     [in]     input buffer
 * #3: len   [in]     input length
 * #4: flush [in]     is finish
 * #r:       [ret]
 *    0: no error, >0 IS_FLUSH: flush block, IS_END: flush block and end,
 *    <0: ERR_INCOMP ...
 */
int32_t conch_unbzip2(struct unbzip2_ctx *ctx, const uint8_t *s,
		uint32_t len, int32_t flush)
{
	if ((!len && !flush) || ctx->last)
		return 0;

	return _unbzip2_block(ctx, s, len, flush);
}


static void _usage(void)
{
	conch_printf(
		"Usage: unbzip2 [OPTION...]\n"
		" unbzip2 decompression utility.\n"
		"\n"
		" -v    show compress radio\n"
		" -h    display help\n"
		"\n"
		"  Use x_stdin as the input stream.\n"
		);
}

static UNBZIP2_NEW(ctx);

static char *_unbzip2_strerr(int32_t n)
{
	switch (n) {
		default:
			return "Unknown";
	}

	return NULL;
}

static int32_t _decompress(xFILE *rfp, xFILE *wfp, int32_t is_v)
{
	uint8_t buf[8192];
	size_t total_len = 0, send_len = 0, len;
	int32_t r;

	if (conch_fread(buf, 1, 4, rfp) != 4)
		return -1;

	conch_unbzip2_init(&ctx, buf[3] - '0');

	total_len += 4;

	while ((len = conch_fread(buf, 1, sizeof(buf), rfp))) {
		total_len += len;
		do {
			r = conch_unbzip2(&ctx, buf, len, 0);
			if (r < 0) {
				conch_fprintf(x_stderr, "unbzip2() %s error!\n",
					_unbzip2_strerr(r));
				return -1;
			}
			if (r) {
				send_len += UNBZIP2_LEN(&ctx);
				conch_fwrite(UNBZIP2_BUF(&ctx),
					1, UNBZIP2_LEN(&ctx), wfp);
				if (r == UNBZIP2_IS_END)
					goto e;
			}
		} while (r);
	}

	do {
		r = conch_unbzip2(&ctx, NULL, 0, 1);
		if (r < 0) {
			conch_fprintf(x_stderr, "unbzip2() %s error!\n",
				_unbzip2_strerr(r));
			return -1;
		}
		if (r) {
			send_len += UNBZIP2_LEN(&ctx);
			conch_fwrite(UNBZIP2_BUF(&ctx),
				1, UNBZIP2_LEN(&ctx), wfp);
			if (r == UNBZIP2_IS_END)
				goto e;
		}
	} while (r);
e:

	if (is_v) {
		conch_fprintf(x_stderr, "%u block, %zu (%zuK) / %zu (%zuK) = %.2f%%"
				" (%08x %08x)\n",
			UNBZIP2_COUNT(&ctx),
			total_len, (total_len / 1024),
			send_len, (send_len / 1024),
			(((double)send_len - total_len) / send_len) * 100);
	}

	conch_fflush(wfp);

	return 0;
}

int main(int argc, char *argv[])
{
	int32_t r, ind = 1;
	char *arg = NULL;
	int32_t is_v = 0;

	while ((r = conch_getopt_r(argc, argv, "hv", &arg, &ind)) != -1) {
		switch (r) {
			case 'v':
				is_v = 1;
				break;
			case 'h':
				_usage();
				return 0;
			default:
				conch_printf("unknown '%c' option!\n",
					OPT_ARGC(arg, r));
				return 1;
		}
	}

	if (_decompress(x_stdin, x_stdout, is_v))
		return 1;

	return 0;
}
