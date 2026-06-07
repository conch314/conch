/* @file: z_unbzip2.c
 * #desc:
 *    The implementations of bzip2 decompression.
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
#include <conch/c_stdint.h>
#include <conch/c_string.h>
#include <conch/bits_get.h>
#include <conch/crc.h>
#include <conch/z_unbzip2.h>


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

/* @func: _bwt_inverse (static)
 * #desc:
 *    inverse burrows–wheeler transform.
 *
 * #1: out   [out] output buffer
 * #2: in    [in]  input buffer
 * #3: len   [in]  input length
 * #4: index [in]  primary index
 * #5: rank  [in]  rank buffer
 */
static void _bwt_inverse(uint8_t *out, const uint8_t *in, uint32_t len,
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

/* @func: _decode_mtfval (static)
 * #desc:
 *    decoding move-to-front value.
 *
 * #1: out   [out] output buffer
 * #2: in    [in]  input buffer
 * #3: len   [in]  input length
 * #4: inuse [in]  characters in use
 */
static uint32_t _decode_mtfval(uint8_t *out, uint16_t *in, uint32_t len,
		uint8_t *inuse)
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

/* @func: _decode_selector_mtfval (static)
 * #desc:
 *    decoding mtf value for selector.
 *
 * #1: ctx [in/out] unbzip2 struct context
 */
static void _decode_selector_mtfval(struct unbzip2_ctx *ctx)
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

/* @func: _build_sym (static)
 * #desc:
 *    build symbol description based on length.
 *
 * #1: desc [in/out] unbzip2 symbol description
 * #2: lens [in]     length of codes
 * #r:      [ret]    0: no error, -1: bits overflow
 */
static int32_t _build_sym(struct unbzip2_sym_desc *desc, const uint8_t *lens)
{
	uint16_t offs[UNBZIP2_BITS_MAX + 1];
	uint32_t elems = desc->elems;
	int32_t m, g, w;

	conch_memset(desc->count, 0, sizeof(desc->count));

	/* statistical bit-length */
	for (uint32_t i = 0; i < elems; i++)
		desc->count[lens[i]]++;

	/* max and min of the bits */
	for (m = 1; m <= UNBZIP2_BITS_MAX; m++) {
		if (desc->count[m])
			break;
	}
	for (g = UNBZIP2_BITS_MAX; g > 0; g--) {
		if (desc->count[g])
			break;
	}
	desc->bits_min = m;
	desc->bits_max = g;

	/* check bits overflow */
	for (w = 1 << m; m < g; w <<= 1, m++) {
		w -= desc->count[m];
		if (w < 0)
			return -1;
	}
	w -= desc->count[g];
	if (w < 0)
		return -1;

	/* symbol offset */
	offs[1] = 0;
	for (int32_t i = 1; i < UNBZIP2_BITS_MAX; i++)
		offs[i + 1] = offs[i] + desc->count[i];

	/* generate symbol */
	for (uint32_t i = 0; i < elems; i++) {
		int32_t len = lens[i];
		if (!len)
			continue;
		desc->sym[offs[len]++] = (uint16_t)i;
	}

	return 0;
}

/* @func: _decode_sym (static)
 * #desc:
 *    decoding the symbol code.
 *
 * #1: desc [in/out] unbzip2 symbol description
 * #2: v    [in]     input bits
 * #3: len  [in/out] input length and return the length used
 * #r:      [ret]    -1: decode error, >=0: symbol code
 */
static int32_t _decode_sym(const struct unbzip2_sym_desc *desc, uint32_t v,
		uint32_t *len)
{
	uint32_t base = 0, offs = 0, m = 1, g = *len, w;

	w = g - 1;
	for (; m <= g; m++) { /* msb */
		offs = (offs << 1) + ((v >> w) & 1);
		if (offs < desc->count[m])
			break;

		base += desc->count[m];
		offs -= desc->count[m];
		w--;
	}
	*len = m;

	offs += base;
	if (offs >= desc->elems)
		return -1;

	return desc->sym[offs];
}

/* @func: _build_symbol (static)
 * #desc:
 *    build symbol description based on length.
 *
 * #1: ctx        [in/out] unbzip2 struct context
 * #2: alpha_size [in]     alpha number
 */
static void _build_symbol(struct unbzip2_ctx *ctx, int32_t alpha_size)
{
	for (int32_t i = 0; i < ctx->ngroups; i++) {
		ctx->huf_desc[i].elems = alpha_size;
		_build_sym(&ctx->huf_desc[i], ctx->huf_lens[i]);
	}
}

/* @func: _add_to_buffer
 * #desc:
 *    add the run-length decoding and characters to the buffer.
 *
 * #1: ctx [in/out] unbzip2 struct context
 * #2: len [in]     run-length encoding length
 */
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

/* @func: _input_block_flush (static)
 * #desc:
 *    input block run-length processing finish.
 *
 * #1: ctx [in/out] unbzip2 struct context
 */
static void _input_block_flush(struct unbzip2_ctx *ctx)
{
	if (ctx->rle_inlen && ctx->rle_inchr < 256)
		_add_to_buffer(ctx, 0);
}

/* @func: _input_block (static)
 * #desc:
 *    input block run-length processing.
 *
 * #1: ctx [in/out] unbzip2 struct context
 * #2: s   [in]     input buffer
 * #3: len [in]     input length
 * #r:     [ret]    return the remaining length
 */
static uint32_t _input_block(struct unbzip2_ctx *ctx, const uint8_t *s,
		uint32_t len)
{
	while (len && ctx->len < (UNBZIP2_BLOCKSIZE_MAX - 512)) {
		uint32_t c = *s++;
		len--;

		if (ctx->rle_inlen == 4) {
			_add_to_buffer(ctx, c);
			ctx->rle_inchr = 256;
			ctx->rle_inlen = 0;
		} else if (c != ctx->rle_inchr) {
			if (ctx->rle_inchr < 256)
				_add_to_buffer(ctx, 0);

			ctx->rle_inchr = c;
			ctx->rle_inlen = 1;
		} else {
			ctx->rle_inlen++;
		}
	}

	return len;
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

	int32_t sym;
	uint32_t v, t, m;
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
					return UNBZIP2_ERR_END_CRC;

				ctx->last = 1;
				return UNBZIP2_IS_END;
			case 4: /* block header */
				BITS_FILL(ctx, flush);
				BITS_DUMP(ctx, &v, 32);
				ctx->block_head_crc = v;
				ctx->block_crc = 0xffffffff;
				ctx->state = 5;
			case 5:
				BITS_FILL(ctx, flush);
				BITS_DUMP(ctx, &v, 1); /* random flag */
				BITS_DUMP(ctx, &v, 24);
				if (v >= ctx->block_max)
					return UNBZIP2_ERR_ORIG_INDEX;

				ctx->orig_index = v;
				ctx->state = 6;
			case 6: /* inuse */
				BITS_FILL(ctx, flush);
				for (t = 0; t < 16; t++) {
					BITS_DUMP(ctx, &v, 1);
					ctx->inuse16[t] = v;
				}

				ctx->t_i = 0;
				ctx->t_j = 0;
				ctx->state = 7;
			case 7:
				ctx->state = 10;
				if (ctx->t_i < 16) {
					if (ctx->inuse16[ctx->t_i++]) {
						ctx->state = 8;
					} else {
						ctx->state = 9;
					}
				}
				break;
			case 8: /* non-zero */
				BITS_FILL(ctx, flush);
				for (t = 0; t < 16; t++) {
					BITS_DUMP(ctx, &v, 1);
					ctx->inuse[ctx->t_j++] = v;
				}

				ctx->state = 7;
				break;
			case 9:
				for (t = 0; t < 16; t++)
					ctx->inuse[ctx->t_j++] = 0;

				ctx->state = 7;
				break;
			case 10: /* ngroups and nselectors */
				BITS_FILL(ctx, flush);

				BITS_DUMP(ctx, &v, 3);
				ctx->ngroups = v;
				if (v > UNBZIP2_NGROUPS)
					return UNBZIP2_ERR_NGROUPS;

				BITS_DUMP(ctx, &v, 15);
				ctx->nselectors = v;
				if (v > UNBZIP2_NSELECTORS)
					return UNBZIP2_ERR_NSELECTORS;

				ctx->t_n = 0;
				ctx->state = 11;
			case 11: /* selector */
				BITS_FILL(ctx, flush);

				t = 0;
				for (m = 0; m < (UNBZIP2_NGROUPS + 1); m++) {
					BITS_DUMP(ctx, &v, 1);
					t += v;
					if (!v)
						break;
				}
				if (v)
					return UNBZIP2_ERR_SELECTOR_MTF;

				ctx->selector_mtf[ctx->t_n++] = t;
				if (ctx->t_n > ctx->nselectors) {
					return UNBZIP2_ERR_NSELECTORS;
				} else if (ctx->t_n == ctx->nselectors) {
					ctx->state = 12;
				}

				break;
			case 12:
				_decode_selector_mtfval(ctx);

				m = 0;
				for (t = 0; t < 256; t++) {
					if (ctx->inuse[t])
						m++;
				}
				ctx->mtf_e = m + 1;

				ctx->t_i = 0;
				ctx->state = 13;
			case 13: /* codes length */
				BITS_FILL(ctx, flush);
				BITS_DUMP(ctx, &v, 5);
				ctx->t_k = v;

				ctx->t_j = 0;
				ctx->state = 14;
			case 14:
				while (ctx->t_k < 20) {
					BITS_FILL(ctx, flush);
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
						return UNBZIP2_ERR_HUFFMAN_LEN;
				}

				t = ctx->t_k;
				if (t > UNBZIP2_BITS_MAX)
					return UNBZIP2_ERR_HUFFMAN_LEN;

				ctx->huf_lens[ctx->t_i][ctx->t_j++] = t;
				if (ctx->t_j > ctx->mtf_e) {
					ctx->state = 13;
					ctx->t_i++;
					if (ctx->t_i > ctx->ngroups)
						return UNBZIP2_ERR_NGROUPS;
					if (ctx->t_i == ctx->ngroups)
						ctx->state = 15;
				}

				break;
			case 15: /* build huffman */
				_build_symbol(ctx, ctx->mtf_e + 1);
				ctx->mtf_n = 0;
				ctx->t_n = 0;
			case 16:
				t = ctx->selector[ctx->t_n++];
				ctx->t_desc = &ctx->huf_desc[t];
				ctx->t_j = 0;
				ctx->state = 17;
			case 17:
				BITS_FILL(ctx, flush);
				BITS_PEEK2(ctx, &v, t,
					ctx->t_desc->bits_min,
					ctx->t_desc->bits_max);

				/* decoding */
				sym = _decode_sym(ctx->t_desc, v, &t);
				if (sym < 0 || sym >= UNBZIP2_ALPHA_SIZE)
					return UNBZIP2_ERR_HUFFMAN_CODE;

				BITS_DUMP(ctx, &v, t);

				ctx->mtf_v[ctx->mtf_n++] = sym;
				ctx->t_j++;
				if (sym == ctx->mtf_e) {
					ctx->state = 18;
				} else if (ctx->t_j == UNBZIP2_SGROUPS_SIZE) {
					ctx->state = 16;
					if (ctx->t_n >= ctx->nselectors)
						return UNBZIP2_ERR_NSELECTORS;
				}

				break;
			case 18: /* decoding mtf and inverse bwt */
				ctx->block_len = _decode_mtfval(
					ctx->buf,
					ctx->mtf_v,
					ctx->mtf_n - 1,
					ctx->inuse);
				_bwt_inverse(
					ctx->block,
					ctx->buf,
					ctx->block_len,
					ctx->orig_index,
					ctx->rank_tmp);

				ctx->t_len = 0;
				ctx->rle_inchr = 256;
				ctx->rle_inlen = 0;
				ctx->state = 19;
			case 19: /* input block */
				if (ctx->len >= (UNBZIP2_BLOCKSIZE_MAX - 512)) {
					ctx->block_crc = conch_crc32_msb(
						ctx->crc_t,
						ctx->block_crc,
						ctx->buf,
						ctx->len);

					ctx->flush = 1;
					return UNBZIP2_IS_FLUSH;
				} else if (ctx->t_len == ctx->block_len) {
					/* end-block */
					_input_block_flush(ctx);
					ctx->block_crc = conch_crc32_msb(
						ctx->crc_t,
						ctx->block_crc,
						ctx->buf,
						ctx->len);

					ctx->state = 20;
					ctx->flush = 1;
					return UNBZIP2_IS_FLUSH;
				}

				v = ctx->block_len - ctx->t_len;
				t = _input_block(ctx,
					ctx->block + ctx->t_len, v);
				ctx->t_len += v - t;

				break;
			case 20: /* end-block */
				ctx->block_crc = ~ctx->block_crc;
				ctx->combined_crc = (ctx->combined_crc << 1)
					| (ctx->combined_crc >> 31);
				ctx->combined_crc ^= ctx->block_crc;

				if (ctx->block_head_crc != ctx->block_crc)
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

	for (int32_t i = 0; i < UNBZIP2_NGROUPS; i++)
		ctx->huf_desc[i].sym = ctx->huf_sym[i];

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
