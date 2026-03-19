/* @file: pbkdf2_sha1.c
 * #desc:
 *    The implementations of password-based key derivation function (sha1).
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
#include <conch/pbkdf2.h>


/* @func: conch_pbkdf2_sha1
 * #desc:
 *    pbkdf2-sha1 key derivation function.
 *
 * #1: pass     [in]  password
 * #2: pass_len [in]  password length
 * #3: salt     [in]  salt
 * #4: salt_len [in]  salt length
 * #5: odk      [out] output key
 * #6: len      [in]  key length
 * #7: k        [in]  iterations number
 */
void conch_pbkdf2_sha1(const uint8_t *pass, uint32_t pass_len,
		const uint8_t *salt, uint32_t salt_len, uint8_t *odk,
		uint32_t len, uint32_t k)
{
	HMAC_SHA1_NEW(ctx);
	uint8_t tmp[SHA1_LEN], u[SHA1_LEN], count[4];
	uint32_t f = 0;

	uint32_t n = (len + SHA1_LEN - 1) / SHA1_LEN;
	for (uint32_t i = 1; i <= n; i++) {
		count[0] = (i >> 24) & 0xff;
		count[1] = (i >> 16) & 0xff;
		count[2] = (i >> 8) & 0xff;
		count[3] = i & 0xff;

		conch_hmac_sha1_init(&ctx, pass, pass_len);
		conch_hmac_sha1_process(&ctx, salt, salt_len);
		conch_hmac_sha1_process(&ctx, count, 4);
		conch_hmac_sha1_finish(&ctx, salt_len + 4);
		conch_memcpy(u, &(HMAC_SHA1_STATE(&ctx, 0)),
			SHA1_LEN);
		conch_memcpy(tmp, &(HMAC_SHA1_STATE(&ctx, 0)),
			SHA1_LEN);

		for (uint32_t j = 1; j < k; j++) {
			conch_hmac_sha1_init(&ctx, pass, pass_len);
			conch_hmac_sha1_process(&ctx, u, SHA1_LEN);
			conch_hmac_sha1_finish(&ctx, SHA1_LEN);
			conch_memcpy(u, &(HMAC_SHA1_STATE(&ctx, 0)),
				SHA1_LEN);

			for (int32_t l = 0; l < SHA1_LEN; l++)
				tmp[l] ^= HMAC_SHA1_STATE(&ctx, l);
		}

		conch_memcpy(odk + f, tmp, ((f + SHA1_LEN) > len) ?
			(len % SHA1_LEN) : SHA1_LEN);
		f += SHA1_LEN;
	}
}
