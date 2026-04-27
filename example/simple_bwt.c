/* @file: simple_bwt.c
 * #desc:
 *    The implementations of a simple Burrows–Wheeler transform.
 *
 * #copy:
 *    Copyright (C) 1970 Public Free Software
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program. If not,
 *    see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <conch/c_stdint.h>
#include <conch/c_stdlib.h>
#include <conch/c_string.h>


/*
 * str: banana
 *  1: b'anana$
 *  2: a'nana$b
 *  3: n'ana$ba
 *  4: a'na$ban
 *  5: n'a$bana
 *  6: a'$banan
 *  7: $'banana
 * dictionary sorting:
 *  7: $banan'a
 *  6: a$bana'n
 *  4: ana$ba'n
 *  2: anana$'b
 *  1: banana'$ => index 5
 *  5: na$ban'a
 *  3: nana$b'a
 *  ^ suffix array - 1 => bwt
 * bwt: annb$aa
 * inv:
 *  sort     1     2      3       4        5         6
 *  1: a  5: $  4: $b  7: $ba  3: $ban  6: $bana  2: $banan
 *  2: n  1: a  5: a$  4: a$b  7: a$ba  3: a$ban  6: a$bana
 *  3: n  6: a  2: an  1: ana  5: ana$  4: ana$b  7: ana$ba
 *  4: b  7: a  3: an  6: ana  2: anan  1: anana  5: anana$
 *  5: $  4: b  7: ba  3: ban  6: bana  2: banan  1: banana
 *  6: a  2: n  1: na  5: na$  4: na$b  7: na$ba  3: na$ban
 *  7: a  3: n  6: na  2: nan  1: nana  5: nana$  4: nana$b
 *     7
 *  1: $banana
 *  2: a$banan
 *  3: ana$ban
 *  4: anana$b
 *  5: banana$ <=
 *  6: na$bana
 *  7: nana$ba
 * res: banana$
 *
 * str: banana
 *  1: banana
 *  2: ananab
 *  3: nanaba
 *  4: anaban
 *  5: nabana
 *  6: abanan
 * dictionary sorting:
 *            a $
 *  6: abanan n
 *  4: anaban n
 *  2: ananab b
 *  1: banana $ => index 4
 *  5: nabana a
 *  3: nanaba a
 * bwt: nnbaaa
 * inv:
 *  1: abanan
 *  2: anaban
 *  3: ananab
 *  4: banana <=
 *  5: nabana
 *  6: nanaba
 * res: banana
 */

const uint8_t *unsafe_in;
uint32_t unsafe_len;

int32_t unsafe_cmp(const void *a, const void *b) {
	uint32_t i = *(uint32_t *)a;
	uint32_t j = *(uint32_t *)b;

	for (uint32_t k = 0; k < unsafe_len; k++) {
		int32_t c1 = unsafe_in[(i + k) % unsafe_len];
		int32_t c2 = unsafe_in[(j + k) % unsafe_len];
		if (c1 != c2)
			return c1 - c2;
	}

	return 0;
}

void bwt_transform_unsafe(uint8_t *out, const uint8_t *in, uint32_t len,
		uint32_t *index, uint32_t *suffix)
{
	unsafe_in = in;
	unsafe_len = len;

	for (uint32_t i = 0; i < len; i++)
		suffix[i] = i;

	/* build suffix-array */
	conch_qsort(suffix, len, sizeof(uint32_t), unsafe_cmp);

	for (uint32_t i = 0; i < len; i++) {
		if (!suffix[i])
			*index = i;
		out[i] = in[(suffix[i] + len - 1) % len];
	}
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

	/* prefix sum */
	for (int32_t i = 0; i < 256; i++) {
		c = count[i];
		count[i] = n;
		n += c;
	}

	/* build rank (last -> first) */
	for (uint32_t i = 0; i < len; i++) {
		rank[i] = count[in[i]] + occ[in[i]];
		occ[in[i]]++;
	}

	/* reverse */
	for (int32_t i = len - 1; i >= 0; i--) {
		out[i] = in[index];
		index = rank[index];
	}
}

int main(void)
{
	uint32_t rank[100], suffix[100], index, n;
	uint8_t res[100], bwt[100];
	char *s;

	s = "banana$";
	n = (uint32_t)conch_strlen(s);

	printf("str: %.*s\n", n, s);
	bwt_transform_unsafe(bwt, (uint8_t *)s, n, &index, suffix);
	printf("bwt: %.*s\n", n, bwt);
	bwt_inverse_lf_mapping(res, bwt, n, index, rank);
	printf("res: %.*s\n\n", n, res);

	s = "banana";
	n = (uint32_t)conch_strlen(s);

	printf("str: %.*s\n", n, s);
	bwt_transform_unsafe(bwt, (uint8_t *)s, n, &index, suffix);
	printf("bwt: %.*s\n", n, bwt);
	bwt_inverse_lf_mapping(res, bwt, n, index, rank);
	printf("res: %.*s\n\n", n, res);

	s = "bananaaaaaaa";
	n = (uint32_t)conch_strlen(s);

	printf("str: %.*s\n", n, s);
	bwt_transform_unsafe(bwt, (uint8_t *)s, n, &index, suffix);
	printf("bwt: %.*s\n", n, bwt);
	bwt_inverse_lf_mapping(res, bwt, n, index, rank);
	printf("res: %.*s\n\n", n, res);

	s = "bananannytt";
	n = (uint32_t)conch_strlen(s);

	printf("str: %.*s\n", n, s);
	bwt_transform_unsafe(bwt, (uint8_t *)s, n, &index, suffix);
	printf("bwt: %.*s\n", n, bwt);
	bwt_inverse_lf_mapping(res, bwt, n, index, rank);
	printf("res: %.*s\n\n", n, res);

	return 0;
}
