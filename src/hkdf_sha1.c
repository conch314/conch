/* @file: hkdf_sha1.c
 * #desc:
 *    The implementations of hmac-base key derivation function (sha1).
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

#include <conch/config.h>
#include <conch/c_stdint.h>
#include <conch/c_string.h>
#include <conch/hmac.h>
#include <conch/hkdf.h>


/* @func: conch_hkdf_sha1
 * #desc:
 *    hkdf-sha1 key derivation function.
 *
 * #1: ikm      [in]  ikm
 * #2: ikm_len  [in]  ikm length
 * #3: salt     [in]  salt
 * #4: salt_len [in]  salt length
 * #5: info     [in]  info
 * #6: info_len [in]  info length
 * #7: okm      [out] output key
 * #8: len      [in]  key length
 * #r:          [ret] 0: no error, -1: key length error
 */
int32_t conch_hkdf_sha1(const uint8_t *ikm, uint32_t ikm_len,
		const uint8_t *salt, uint32_t salt_len,
		const uint8_t *info, uint32_t info_len,
		uint8_t *okm, uint32_t len)
{
	HMAC_SHA1_NEW(ctx);
	uint8_t tmp[SHA1_LEN], prk[SHA1_LEN], count[1];
	uint32_t f = 0, tmp_len = 0;

	if (len < 1 || len > (255 * SHA1_LEN))
		return -1;

	conch_hmac_sha1_init(&ctx, salt, salt_len);
	conch_hmac_sha1_process(&ctx, ikm, ikm_len);
	conch_hmac_sha1_finish(&ctx, ikm_len);
	conch_memcpy(prk, &(HMAC_SHA1_STATE(&ctx, 0)), SHA1_LEN);

	uint32_t n = (len + SHA1_LEN - 1) / SHA1_LEN;
	for (uint32_t i = 1; i <= n; i++) {
		count[0] = i & 0xff;

		conch_hmac_sha1_init(&ctx, prk, SHA1_LEN);
		conch_hmac_sha1_process(&ctx, tmp, tmp_len);
		conch_hmac_sha1_process(&ctx, info, info_len);
		conch_hmac_sha1_process(&ctx, count, 1);
		conch_hmac_sha1_finish(&ctx, tmp_len + info_len + 1);
		tmp_len = SHA1_LEN;

		conch_memcpy(tmp, &(HMAC_SHA1_STATE(&ctx, 0)), SHA1_LEN);
		conch_memcpy(okm + f, tmp, ((f + SHA1_LEN) > len) ?
			(len % SHA1_LEN) : SHA1_LEN);
		f += SHA1_LEN;
	}

	return 0;
}
