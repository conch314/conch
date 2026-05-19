/* @file: z_bzip2.c
 * #desc:
 *    The implementations of bzip2 compression algorithm.
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
#include <conch/z_bzip2.h>
#include <conch/crc.h>


/* block sorting */
#define QS_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define QS_SWAP(a, b) \
	do { \
		uint32_t ___tmp = a; \
		a = b; \
		b = ___tmp; \
	} while (0)
#define QS_VSWAP(suffix, a, b, n) \
	do { \
		int32_t ___tmp1 = a; \
		int32_t ___tmp2 = b; \
		int32_t ___tmp3 = n; \
		for (; ___tmp3 > 0; ___tmp3--) { \
			QS_SWAP(suffix[___tmp1], suffix[___tmp2]); \
			___tmp1++; \
			___tmp2++; \
		} \
	} while (0)
#define QS_PUSH(stack, sp, lo, hi) \
	do { \
		stack[sp] = lo; \
		stack[sp + 1] = hi; \
		sp += 2; \
	} while (0)
#define QS_POP(stack, sp, lo, hi) \
	do { \
		sp -= 2; \
		lo = stack[sp]; \
		hi = stack[sp + 1]; \
	} while (0)

#define BH_SET(x, n) (x)[(n) >> 5] |= 1U << ((n) & 31)
#define BH_CLEAR(x, n) (x)[(n) >> 5] &= ~(1U << ((n) & 31))
#define BH_ISSET(x, n) ((x)[(n) >> 5] & (1U << ((n) & 31)))
#define BH_WORD(x, n) ((x)[(n) >> 5])
#define BH_UNALIGNED(n) ((n) & 0x1f)

/* huffman coding */
#define HUF_MAX(a, b) ((a) > (b) ? (a) : (b))
#define HUF_WEIGHT_OF(x) ((x) & 0xffffff00)
#define HUF_DEPTH_OF(x) ((x) & 0x000000ff)
#define HUF_ADD_WEIGHTS(a, b) \
	((HUF_WEIGHT_OF(a) + HUF_WEIGHT_OF(b)) \
	| (1 + HUF_MAX(HUF_DEPTH_OF(a), HUF_DEPTH_OF(b))))
#define HUF_UPHEAP(heap, weight, k) \
	{ \
		int32_t ___tmp_k = k; \
		int32_t ___tmp_v = heap[___tmp_k]; \
		while (weight[___tmp_v] < weight[heap[___tmp_k >> 1]]) { \
			heap[___tmp_k] = heap[___tmp_k >> 1]; \
			___tmp_k >>= 1; \
		} \
		heap[___tmp_k] = ___tmp_v; \
	} while (0)
#define HUF_DOWNHEAP(heap, weight, n, k) \
	{ \
		int32_t ___tmp_n = n; \
		int32_t ___tmp_k = k; \
		int32_t ___tmp_v = heap[___tmp_k]; \
		int32_t ___tmp_j = ___tmp_k << 1; \
		while (___tmp_j <= ___tmp_n) { \
			int32_t ___tmp_l = heap[___tmp_j]; \
			int32_t ___tmp_r = heap[___tmp_j + 1]; \
			if (___tmp_j < ___tmp_n && weight[___tmp_r] \
					< weight[___tmp_l]) { \
				___tmp_j++; \
				___tmp_l = heap[___tmp_j]; \
			} \
			if (weight[___tmp_v] < weight[___tmp_l]) \
				break; \
			heap[___tmp_k] = heap[___tmp_j]; \
			___tmp_k = ___tmp_j; \
			___tmp_j <<= 1; \
		} \
		heap[___tmp_k] = ___tmp_v; \
	} while (0)

#define SEND_BYTE(s, x) _send_bits(s, x, 8)
#define SEND_BITS(s, x, n) _send_bits(s, x, n)
#define SEND_FINISH(s) _send_bits_finish(s)


/* @func: _send_bits (static)
 * #desc:
 *    send bits to buffer.
 *
 * #1: ctx [in/out] bzip2 struct context
 * #2: v   [in]     bits value
 * #3: len [in]     bits length
 */
static void _send_bits(struct bzip2_ctx *ctx, uint32_t v, uint32_t len)
{
	while (ctx->be_len >= 8) {
		ctx->buf[ctx->len++] = (uint8_t)(ctx->be_val >> 24);
		ctx->be_val <<= 8;
		ctx->be_len -= 8;
	}

	ctx->be_val |= v << (32 - ctx->be_len - len);
	ctx->be_len += len;
}

/* @func: _send_bits_finish (static)
 * #desc:
 *    flush bits to buffer.
 *
 * #1: ctx [in/out] bzip2 struct context
 */
static void _send_bits_finish(struct bzip2_ctx *ctx)
{
	while (ctx->be_len > 0) {
		ctx->buf[ctx->len++] = (uint8_t)(ctx->be_val >> 24);
		ctx->be_val <<= 8;
		ctx->be_len -= 8;
	}
}

/* @func: _fallback_qsort3 (static)
 * #desc:
 *    3-way quicksort function in bucket.
 *
 * #1: sa [in/out] suffix array
 * #2: rk [in/out] rank buffer
 * #3: l  [in]     left boundary
 * #4: r  [in]     right boundary
 */
static void _fallback_qsort3(uint32_t *sa, uint32_t *rk, int32_t l,
		int32_t r)
{
	int32_t stack[100 * 2];
	int32_t lo, hi, sp = 0;
	int32_t un_lo, un_hi, lt_lo, gt_hi, n, m;
	uint32_t t, tt, q = 0;

	QS_PUSH(stack, sp, l, r);

	while (sp) {
		QS_POP(stack, sp, lo, hi);
		if (lo == hi)
			continue;

		if ((hi - lo) < 10) { /* insertion sort */
			if ((hi - lo) > 3) {
				for (int32_t i = hi - 4; i >= lo; i--) {
					t = sa[i];
					tt = rk[t];
					int32_t j = i + 4;
					for (; j <= hi && tt > rk[sa[j]];
							j += 4)
						sa[j - 4] = sa[j];
					sa[j - 4] = t;
				}
			}

			for (int32_t i = hi - 1; i >= lo; i--) {
				t = sa[i];
				tt = rk[t];
				int32_t j = i + 1;
				for (; j <= hi && tt > rk[sa[j]]; j++)
					sa[j - 1] = sa[j];
				sa[j - 1] = t;
			}

			continue;
		}

		/* random partitioning */
		q = ((q * 7621) + 1) % 32768;
		t = q % 3;
		if (t == 0) {
			tt = rk[sa[lo]];
		} else if (t == 1) {
			tt = rk[sa[(lo + hi) >> 1]];
		} else {
			tt = rk[sa[hi]];
		}

		un_lo = lt_lo = lo;
		un_hi = gt_hi = hi;

		/*
		 *    lo == pivot
		 * --------------
		 * lt_lo  < pivot
		 * --------------
		 *    unprocessed
		 * --------------
		 * gt_hi  > pivot
		 * --------------
		 *    hi == pivot
		 */

		while (1) {
			/* left side scan */
			while (un_lo <= un_hi) {
				t = rk[sa[un_lo]];
				if (t > tt) { /* > pivot */
					break;
				} else if (t < tt) { /* < pivot */
					un_lo++;
					continue;
				}

				/* == pivot, collection */
				QS_SWAP(sa[un_lo], sa[lt_lo]);
				lt_lo++;
				un_lo++;
			}

			/* right side scan */
			while (un_lo <= un_hi) {
				t = rk[sa[un_hi]];
				if (t < tt) { /* < pivot */
					break;
				} else if (t > tt) { /* > pivot */
					un_hi--;
					continue;
				}

				/* == pivot, collection */
				QS_SWAP(sa[un_hi], sa[gt_hi]);
				gt_hi--;
				un_hi--;
			}

			if (un_lo > un_hi)
				break;

			/* swap unprocessed */
			QS_SWAP(sa[un_lo], sa[un_hi]);
			un_lo++;
			un_hi--;
		}
		if (gt_hi < lt_lo)
			continue;

		/* < pivot | == pivot | > pivot */

		n = QS_MIN(lt_lo - lo, un_lo - lt_lo);
		QS_VSWAP(sa, lo, un_lo - n, n);

		m = QS_MIN(hi - gt_hi, gt_hi - un_hi);
		QS_VSWAP(sa, un_lo, hi - m + 1, m);

		/* divide */
		n = lo + (un_lo - lt_lo) - 1;
		m = hi - (gt_hi - un_hi) + 1;

		if ((n - lo) > (hi - m)) {
			QS_PUSH(stack, sp, lo, n);
			QS_PUSH(stack, sp, m, hi);
		} else {
			QS_PUSH(stack, sp, m, hi);
			QS_PUSH(stack, sp, lo, n);
		}
	}
}

/* @func: _fallback_sort (static)
 * #desc:
 *    fallback for cyclic suffix array sorting.
 *
 * #1: in  [in]  input string
 * #2: len [in]  length
 * #3: sa  [out] suffix array
 * #4: tmp [out] temporary buffer
 */
static void _fallback_sort(const uint8_t *in, uint32_t len, uint32_t *sa,
		uint32_t *tmp)
{
	uint32_t count[256];
	uint32_t *rk = tmp;
	uint32_t *bh = tmp += len;
	uint32_t l, r, t, c, cc, n;

	/* single character sorting */
	conch_memset(count, 0, sizeof(count));
	for (uint32_t i = 0; i < len; i++)
		count[in[i]]++;
	for (uint32_t i = 1; i < 256; i++)
		count[i] += count[i - 1];
	for (uint32_t i = 0; i < len; i++) { /* counting sort initial sa */
		c = --count[in[i]];
		sa[c] = i;
	}

	/* build bucket boundary */
	conch_memset(bh, 0, sizeof(uint32_t) * (len / 32 + 3));
	for (uint32_t i = 0; i < 256; i++)
		BH_SET(bh, count[i]);
	for (uint32_t i = 0; i < 32; i++) {
		BH_SET(bh, len + 2 * i);
		BH_CLEAR(bh, len + 2 * i + 1);
	}

	/* doubling method, bucket partitioning, and 3-way quicksort */

	for (uint32_t k = 1; k <= len; k <<= 1) {
		c = 0; /* update rank */
		for (uint32_t i = 0; i < len; i++) {
			if (BH_ISSET(bh, i))
				c = i;
			t = sa[i] + len - k;
			rk[(t >= len) ? (t - len) : t] = c;
		}

		n = 0;
		t = 0;

		while (1) {
			/* left boundary */
			while (BH_ISSET(bh, t) && BH_UNALIGNED(t))
				t++;
			if (BH_ISSET(bh, t)) {
				while (BH_WORD(bh, t) == 0xffffffff)
					t += 32;
				while (BH_ISSET(bh, t))
					t++;
			}
			l = t - 1;
			if (l >= len)
				break;

			/* right boundary */
			while (!BH_ISSET(bh, t) && BH_UNALIGNED(t))
				t++;
			if (!BH_ISSET(bh, t)) {
				while (BH_WORD(bh, t) == 0x00000000)
					t += 32;
				while (!BH_ISSET(bh, t))
					t++;
			}
			r = t - 1;
			if (r >= len)
				break;

			/* bucket sort */
			if (r > l) {
				n += r - l + 1;
				_fallback_qsort3(sa, rk, l, r);

				c = (uint32_t)-1; /* divide */
				for (uint32_t i = l; i <= r; i++) {
					cc = rk[sa[i]];
					if (c != cc) {
						BH_SET(bh, i);
						c = cc;
					}
				}
			}
		}

		if (!n)
			break;
	}
}

/* @func: _blocksort (static)
 * #desc:
 *    block sorting function.
 *
 * #1: ctx [in/out] bzip2 struct context
 */
static void _blocksort(struct bzip2_ctx *ctx)
{
	uint32_t *tmp = ctx->sort_tmp;
	uint32_t *sa = ctx->sort_sa;
	uint8_t *in = ctx->block;
	uint32_t len = ctx->block_len;

	/* build suffix-array */
	_fallback_sort(in, len, sa, tmp);

	for (uint32_t i = 0; i < len; i++) {
		if (!sa[i])
			ctx->orig_index = i; /* primary index */
	}
}

/* @func: _gen_mtfval (static)
 * #desc:
 *    generate move-to-front coding values.
 *
 * #1: ctx [in/out] bzip2 struct context
 */
static void _gen_mtfval(struct bzip2_ctx *ctx)
{
	uint8_t tab[256], seq[256], c;
	uint32_t pos, n = 0, w = 0;
	uint16_t *out = ctx->mtf_v;
	uint8_t *in = ctx->block;
	uint32_t len = ctx->block_len;

	conch_memset(ctx->mtf_freq, 0, sizeof(ctx->mtf_freq));

	/* make a sequence table */
	for (uint32_t i = 0; i < 256; i++) {
		if (ctx->inuse[i]) {
			seq[i] = (uint8_t)n;
			n++;
		}
	}
	ctx->mtf_e = n + 1;

	/* characters set */
	for (uint32_t i = 0; i < n; i++)
		tab[i] = (uint8_t)i;

	n = 0;
	for (uint32_t i = 0; i < len; i++) {
		if (!ctx->sort_sa[i]) { /* bwt string */
			pos = len - 1;
			c = seq[in[pos]];
		} else {
			pos = ctx->sort_sa[i] - 1;
			c = seq[in[pos]];
		}

		if (tab[0] == c) {
			n++;
		} else {
			if (n > 0) { /* zero rle */
				n--;
				while (1) {
					out[w++] = n & 1;
					ctx->mtf_freq[n & 1]++;
					if (n < 2)
						break;
					n = (n - 2) >> 1;
				}
				n = 0;
			}

			/* character position */
			for (pos = 0; tab[pos] != c; pos++);
			out[w++] = pos + 1; /* zero rle + 1 */
			ctx->mtf_freq[pos + 1]++;

			for (; pos > 0; pos--) /* move */
				tab[pos] = tab[pos - 1];
			tab[0] = c;
		}
	}

	if (n > 0) { /* zero rle */
		n--;
		while (1) {
			out[w++] = n & 1;
			ctx->mtf_freq[n & 1]++;
			if (n < 2)
				break;
			n = (n - 2) >> 1;
		}
		n = 0;
	}

	out[w++] = ctx->mtf_e; /* end */
	ctx->mtf_freq[ctx->mtf_e]++;
	ctx->mtf_n = w;
}

/* @func: _gen_lens (static)
 * #desc:
 *    generate codes length based on freq.
 *
 * #1: lens       [out] codes length
 * #2: freq       [in]  codes freq
 * #3: alpha_size [in]  symbol size
 * #4: max_len    [in]  max length
 */
static void _gen_lens(uint8_t *lens, const uint32_t *freq,
		int32_t alpha_size, int32_t max_len)
{
	uint32_t weight[258 * 2];
	int32_t heap[258 + 2];
	int32_t parent[258 * 2];
	int32_t heap_size, node, n, m, too_long;

	for (int32_t i = 0; i < alpha_size; i++)
		weight[i + 1] = (freq[i] ? freq[i] : 1) << 8;

	while (1) {
		heap_size = 0;
		node = alpha_size;

		heap[0] = 0;
		weight[0] = 0;
		parent[0] = -2;

		for (int32_t i = 1; i <= alpha_size; i++) {
			parent[i] = -1;
			heap[++heap_size] = i;
			HUF_UPHEAP(heap, weight, heap_size);
		}

		while (heap_size > 1) {
			n = heap[1];
			heap[1] = heap[heap_size--];
			HUF_DOWNHEAP(heap, weight, heap_size, 1);

			m = heap[1];
			heap[1] = heap[heap_size--];
			HUF_DOWNHEAP(heap, weight, heap_size, 1);

			parent[n] = parent[m] = ++node;
			weight[node] = HUF_ADD_WEIGHTS(weight[n], weight[m]);
			parent[node] = -1;

			heap[++heap_size] = node;
			HUF_UPHEAP(heap, weight, heap_size);
		}

		too_long = 0;
		for (int32_t i = 1; i <= alpha_size; i++) {
			n = i;
			for (m = 0; parent[n] >= 0; m++)
				n = parent[n];

			lens[i - 1] = m;
			if (m > max_len)
				too_long = 1;
		}
		if (!too_long)
			break;

		for (int32_t i = 1; i <= alpha_size; i++) {
			uint32_t t = weight[i] >> 8;
			t = 1 + (t >> 1);
			weight[i] = t << 8;
		}
	}
}

static void _gen_codes(uint32_t *code, const uint8_t *lens,
		int32_t alpha_size, int32_t min_len, int32_t max_len)
{
	uint32_t v = 0;
	for (int32_t n = min_len; n <= max_len; n++) {
		for (int32_t i = 0; i < alpha_size; i++) {
			if (lens[i] == n) {
				code[i] = v;
				v++;
			}
		}
		v <<= 1;
	}
}

static void _send_block(struct bzip2_ctx *ctx)
{
	int32_t alpha_size = ctx->mtf_e + 1;
	int32_t ngroups = 1, ge, gs, nselectors;
	uint16_t *mtf_v = ctx->mtf_v;
	uint16_t cost[6];
	int32_t fave[6];

	if (ctx->mtf_n < 200) {
		ngroups = 2;
	} else if (ctx->mtf_n < 600) {
		ngroups = 3;
	} else if (ctx->mtf_n < 1200) {
		ngroups = 4;
	} else if (ctx->mtf_n < 2400) {
		ngroups = 5;
	} else {
		ngroups = 6;
	}

	for (int32_t i = 0; i < 6; i++) {
		for (int32_t j = 0; j < alpha_size; j++)
			ctx->huf_len[i][j] = 15;
	}

	int32_t rem_freq = ctx->mtf_n;
	gs = 0;

	for (int32_t k = ngroups; k > 0; k--) {
		int32_t t = rem_freq / k;
		int32_t a = 0;
		ge = gs - 1;

		while (a < t && ge < (alpha_size - 1)) {
			ge++;
			a += ctx->mtf_freq[ge];
		}

		if (ge > gs && k != ngroups && k != 1
				&& ((ngroups - k) % 2) == 1) {
			a -= ctx->mtf_freq[ge];
			ge--;
		}

		for (int32_t j = 0; j < alpha_size; j++) {
			if (j >= gs && j <= ge) {
				ctx->huf_len[k - 1][j] = 0;
			} else {
				ctx->huf_len[k - 1][j] = 15;
			}
		}

		gs = ge + 1;
		rem_freq -= a;
	}

	for (int32_t k = 0; k < 4; k++) {
		for (int32_t i = 0; i < 6; i++)
			fave[i] = 0;
		for (int32_t i = 0; i < ngroups; i++) {
			for (int32_t j = 0; j < alpha_size; j++)
				ctx->huf_rfreq[i][j] = 0;
		}

		nselectors = 0;
		int32_t totc = 0;
		gs = 0;
		while (gs < ctx->mtf_n) {
			ge = gs + 50 - 1;
			if (ge >= ctx->mtf_n)
				ge = ctx->mtf_n - 1;

			for (int32_t i = 0; i < 6; i++)
				cost[i] = 0;

			for (int32_t i = gs; i <= ge; i++) {
				uint16_t v = mtf_v[i];
				for (int32_t j = 0; j < ngroups; j++)
					cost[j] += ctx->huf_len[j][v];
			}

			int32_t bc = 999999999;
			int32_t bt = -1;
			for (int32_t j = 0; j < ngroups; j++) {
				if (cost[j] < bc) {
					bc = cost[j];
					bt = j;
				}
			}

			totc += bc;
			fave[bt]++;
			ctx->selector[nselectors] = bt;
			nselectors++;

			for (int32_t i = gs; i <= ge; i++)
				ctx->huf_rfreq[bt][mtf_v[i]]++;
			gs = ge + 1;
		}

		for (int32_t i = 0; i < ngroups; i++) {
			_gen_lens(ctx->huf_len[i],
					ctx->huf_rfreq[i],
					alpha_size,
					17);
		}
	}

	uint8_t tab[6];
	for (int32_t i = 0; i < ngroups; i++)
		tab[i] = i;

	for (int32_t i = 0; i < nselectors; i++) {
		int32_t c = ctx->selector[i];
		int32_t pos = 0;

		for (; tab[pos] != c; pos++);
		ctx->selector_mtf[i] = pos;

		for (; pos > 0; pos--) /* move */
			tab[pos] = tab[pos - 1];
		tab[0] = c;
	}

	for (int32_t i = 0; i < ngroups; i++) {
		int32_t min_len = 32;
		int32_t max_len = 0;
		for (int32_t j = 0; j < alpha_size; j++) {
			if (ctx->huf_len[i][j] > max_len)
				max_len = ctx->huf_len[i][j];
			if (ctx->huf_len[i][j] < min_len)
				min_len = ctx->huf_len[i][j];
		}

		_gen_codes(ctx->huf_code[i],
			ctx->huf_len[i],
			alpha_size,
			min_len,
			max_len);
	}

	SEND_BYTE(ctx, 0x31);
	SEND_BYTE(ctx, 0x41);
	SEND_BYTE(ctx, 0x59);
	SEND_BYTE(ctx, 0x26);
	SEND_BYTE(ctx, 0x53);
	SEND_BYTE(ctx, 0x59);
	SEND_BYTE(ctx, (uint8_t)(ctx->block_crc >> 24));
	SEND_BYTE(ctx, (uint8_t)(ctx->block_crc >> 16));
	SEND_BYTE(ctx, (uint8_t)(ctx->block_crc >> 8));
	SEND_BYTE(ctx, (uint8_t)ctx->block_crc);
	SEND_BITS(ctx, 0, 1);
	SEND_BITS(ctx, ctx->orig_index, 24);

	uint8_t inuse16[16];
	for (int32_t i = 0; i < 16; i++) {
		inuse16[i] = 0;
		for (int32_t j = 0; j < 16; j++) {
			if (ctx->inuse[i * 16 + j])
				inuse16[i] = 1;
		}
	}
	for (int32_t i = 0; i < 16; i++)
		SEND_BITS(ctx, inuse16[i] & 1, 1);

	for (int32_t i = 0; i < 16; i++) {
		if (!inuse16[i])
			continue;
		for (int32_t j = 0; j < 16; j++)
			SEND_BITS(ctx, ctx->inuse[i * 16 + j] & 1, 1);
	}

	SEND_BITS(ctx, ngroups, 3);
	SEND_BITS(ctx, nselectors, 15);

	for (int32_t i = 0; i < nselectors; i++) {
		for (int32_t j = 0; j < ctx->selector_mtf[i]; j++)
			SEND_BITS(ctx, 1, 1);
		SEND_BITS(ctx, 0, 1);
	}

	for (int32_t i = 0; i < ngroups; i++) {
		int32_t curr = ctx->huf_len[i][0];
		SEND_BITS(ctx, curr, 5);
		for (int32_t j = 0; j < alpha_size; j++) {
			while (curr < ctx->huf_len[i][j]) {
				SEND_BITS(ctx, 2, 2);
				curr++;
			}
			while (curr > ctx->huf_len[i][j]) {
				SEND_BITS(ctx, 3, 2);
				curr--;
			}
			SEND_BITS(ctx, 0, 1);
		}
	}

	int32_t selctr = 0;
	gs = 0;
	while (1) {
		if (gs >= ctx->mtf_n)
			break;

		ge = gs + 50 - 1;
		if (ge >= ctx->mtf_n)
			ge = ctx->mtf_n - 1;

		for (int32_t i = gs; i <= ge; i++) {
			SEND_BITS(ctx,
				ctx->huf_code[ctx->selector[selctr]][mtf_v[i]],
				ctx->huf_len[ctx->selector[selctr]][mtf_v[i]]);
		}

		gs = ge + 1;
		selctr++;
	}
}

/* @func: _init_block (static)
 * #desc:
 *    initialization block.
 *
 * #1: ctx [in/out] bzip2 struct context
 */
static void _init_block(struct bzip2_ctx *ctx)
{
	ctx->block_len = 0;
	ctx->block_crc = 0xffffffff;

	ctx->rle_inchr = 256;
	ctx->rle_inlen = 0;
	conch_memset(ctx->inuse, 0, sizeof(ctx->inuse));
}

/* @func: _add_pair_to_block (static)
 * #desc:
 *    add run-length encoding or characters to the block.
 *
 * #1: ctx [in/out] bzip2 struct context
 */
static void _add_pair_to_block(struct bzip2_ctx *ctx)
{
	uint32_t c = ctx->rle_inchr;
	ctx->inuse[c] = 1;

	switch (ctx->rle_inlen) {
		case 3:
			ctx->block[ctx->block_len++] = (uint8_t)c;
		case 2:
			ctx->block[ctx->block_len++] = (uint8_t)c;
		case 1:
			ctx->block[ctx->block_len++] = (uint8_t)c;
			break;
		default:
			ctx->block[ctx->block_len++] = (uint8_t)c;
			ctx->block[ctx->block_len++] = (uint8_t)c;
			ctx->block[ctx->block_len++] = (uint8_t)c;
			ctx->block[ctx->block_len++] = (uint8_t)c;

			c = ctx->rle_inlen - 4;
			ctx->inuse[c] = 1;
			ctx->block[ctx->block_len++] = (uint8_t)c;
			break;
	}
}

/* @func: _input_block_flush (static)
 * #desc:
 *    input block processing finish.
 *
 * #1: ctx [in/out] bzip2 struct context
 */
static void _input_block_flush(struct bzip2_ctx *ctx)
{
	if (ctx->rle_inlen && ctx->rle_inchr < 256)
		_add_pair_to_block(ctx);
}

/* @func: _input_block (static)
 * #desc:
 *    input block run-length processing.
 *
 * #1: ctx [in/out] bzip2 struct context
 */
static void _input_block(struct bzip2_ctx *ctx, const uint8_t *s,
		uint32_t len)
{
	while (len--) {
		uint32_t c = *s++;
		if (c != ctx->rle_inchr || ctx->rle_inlen == 255) {
			if (ctx->rle_inchr < 256)
				_add_pair_to_block(ctx);

			ctx->rle_inchr = c;
			ctx->rle_inlen = 1;
		} else {
			ctx->rle_inlen++;
		}
	}
}

/* @func: _bzip2_block (static)
 * #desc:
 *    bzip2 block compression function.
 *
 * #1: ctx   [in/out] bzip2 struct context
 * #2: s     [in]     input buffer
 * #3: len   [in]     input length
 * #4: flush [in]     is finish
 * #r:       [ret]
 *    0: no error, >0 IS_FLUSH: flush block, IS_END: flush block and end
 */
static int32_t _bzip2_block(struct bzip2_ctx *ctx, const uint8_t *s,
		uint32_t len, int32_t flush)
{
	if (!ctx->flush) {
		ctx->s = s;
		ctx->s_len = len;
	} else { /* continue */
		ctx->len = 0;
		ctx->flush = 0;
	}

#undef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

	/* fill input block */
	while (ctx->block_len < ctx->block_max) {
		len = ctx->block_max - ctx->block_len;
		len = MIN(len, ctx->s_len);
		if (flush && !len) /* last */
			break;

		ctx->block_crc = conch_crc32_msb(ctx->crc_t,
			ctx->block_crc, ctx->s, len);

		_input_block(ctx, ctx->s, len);

		ctx->s += len;
		ctx->s_len -= len;
		if (!flush && !ctx->s_len) /* next input */
			return 0;
	}
	_input_block_flush(ctx);

	/* flush block */
	if (ctx->block_len) {
		ctx->block_crc = ~ctx->block_crc;
		ctx->combined_crc = (ctx->combined_crc << 1)
			| (ctx->combined_crc >> 31);
		ctx->combined_crc ^= ctx->block_crc;

		_blocksort(ctx);
		_gen_mtfval(ctx);
		_send_block(ctx);
		_init_block(ctx);
		ctx->flush = 1;
	}

	/* end */
	if (flush && !ctx->s_len) {
		SEND_BYTE(ctx, 0x17);
		SEND_BYTE(ctx, 0x72);
		SEND_BYTE(ctx, 0x45);
		SEND_BYTE(ctx, 0x38);
		SEND_BYTE(ctx, 0x50);
		SEND_BYTE(ctx, 0x90);
		SEND_BYTE(ctx, (uint8_t)(ctx->combined_crc >> 24));
		SEND_BYTE(ctx, (uint8_t)(ctx->combined_crc >> 16));
		SEND_BYTE(ctx, (uint8_t)(ctx->combined_crc >> 8));
		SEND_BYTE(ctx, (uint8_t)ctx->combined_crc);

		SEND_FINISH(ctx);
		return BZIP2_IS_END;
	}

	return BZIP2_IS_FLUSH;
}

/* @func: conch_bzip2_init
 * #desc:
 *    bzip2 compression initialization.
 *
 * #1: ctx [out] bzip2 struct context
 * #2: lev [in]  compress level
 * #r:     [ret] 0: no error, -1: level error
 */
int32_t conch_bzip2_init(struct bzip2_ctx *ctx, int32_t lev)
{
	if (!(lev >= 1 && lev <= 9))
		return -1;

	/* initialization */
	ctx->block_max = 100000 * lev - 19;
	ctx->mtf_v = (uint16_t *)ctx->sort_tmp; /* reuse */

	ctx->combined_crc = 0;
	ctx->crc_t = conch_crc32_table(CRC32_DEFAULT_MSB_TYPE);

	/* initialize block */
	_init_block(ctx);

	ctx->be_val = 0;
	ctx->be_len = 0;
	ctx->lev = lev;
	ctx->flush = 0;
	ctx->len = 0;

	return 0;
}

/* @func: conch_bzip2
 * #desc:
 *    bzip2 compression function.
 *
 * #1: ctx   [in/out] bzip2 struct context
 * #2: s     [in]     input buffer
 * #3: len   [in]     input length
 * #4: flush [in]     is finish
 * #r:       [ret]
 *    0: no error, >0 IS_FLUSH: flush block, IS_END: flush block and end
 */
int32_t conch_bzip2(struct bzip2_ctx *ctx, const uint8_t *s,
		uint32_t len, int32_t flush)
{
	if (!len && !flush)
		return 0;

	return _bzip2_block(ctx, s, len, flush);
}
