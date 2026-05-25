/* @file: util_unbzip2.c
 * #desc:
 *
 * #copy:
 *    Copyright (C) 1970 Public Free Software.
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
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>
#include <conch/c_string.h>
#include <conch/c_getopt.h>
#include <conch/z_unbzip2.h>


static void _usage(void)
{
	printf(
		"Usage: unbzip2 [OPTION...]\n"
		" bzip2 decompression utility.\n"
		"\n"
		" -v    show compress radio\n"
		" -h    display help\n"
		"\n"
		"  Use stdin as the input stream.\n"
		);
}

static UNBZIP2_NEW(ctx);

static char *_unbzip2_strerr(int32_t n)
{
	switch (n) {
		case UNBZIP2_ERR_INCOMP:
			return "data incomplete";
		case UNBZIP2_ERR_HEAD:
			return "block header ";
		case UNBZIP2_ERR_HEAD_CRC:
			return "block header crc";
		case UNBZIP2_ERR_END_CRC:
			return "end header crc";
		case UNBZIP2_ERR_ORIG_INDEX:
			return "primary index";
		case UNBZIP2_ERR_NGROUPS:
			return "ngroups size";
		case UNBZIP2_ERR_NSELECTORS:
			return "nselectors size";
		case UNBZIP2_ERR_SELECTOR_MTF:
			return "selector mtf";
		case UNBZIP2_ERR_HUFFMAN_LEN:
			return "huffman length";
		case UNBZIP2_ERR_HUFFMAN_CODE:
			return "huffman code";
		default:
			return "Unknown";
	}

	return NULL;
}

static int32_t _decompress(FILE *rfp, FILE *wfp, int32_t is_v)
{
	uint8_t buf[8192];
	size_t total_len = 0, send_len = 0, len;
	int32_t r;

	if (fread(buf, 1, 4, rfp) != 4) {
		fprintf(stderr, "bzip2 header error!\n");
		return -1;
	}
	total_len += 4;

	if (conch_unbzip2_init(&ctx, buf[3] - '0')) {
		fprintf(stderr, "unbzip2_init() initialize error!\n");
		return -1;
	}

	while ((len = fread(buf, 1, sizeof(buf), rfp))) {
		total_len += len;
		do {
			r = conch_unbzip2(&ctx, buf, len, 0);
			if (r < 0) {
				fprintf(stderr, "unbzip2() %s error!\n",
					_unbzip2_strerr(r));
				return -1;
			}
			if (r) {
				send_len += UNBZIP2_LEN(&ctx);
				fwrite(UNBZIP2_BUF(&ctx),
					1, UNBZIP2_LEN(&ctx), wfp);
				if (r == UNBZIP2_IS_END)
					goto e;
			}
		} while (r);
	}

	do {
		r = conch_unbzip2(&ctx, NULL, 0, 1);
		if (r < 0) {
			fprintf(stderr, "unbzip2() %s error!\n",
				_unbzip2_strerr(r));
			return -1;
		}
		if (r) {
			send_len += UNBZIP2_LEN(&ctx);
			fwrite(UNBZIP2_BUF(&ctx),
				1, UNBZIP2_LEN(&ctx), wfp);
			if (r == UNBZIP2_IS_END)
				goto e;
		}
	} while (r);
e:

	if (is_v) {
		fprintf(stderr, "%u block, %zu (%zuK) / %zu (%zuK) = %.2f%%\n",
			UNBZIP2_COUNT(&ctx),
			total_len, (total_len / 1024),
			send_len, (send_len / 1024),
			(((double)send_len - total_len) / send_len) * 100);
	}

	fflush(wfp);

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
				printf("unknown '%c' option!\n",
					OPT_ARGC(arg, r));
				return 1;
		}
	}

	if (_decompress(stdin, stdout, is_v))
		return 1;

	return 0;
}
