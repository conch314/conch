/* @file: hmac_sha1.c
 * #desc:
 *    The implementations of hash message authentication code (sha1).
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
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>
#include <conch/c_string.h>
#include <conch/sha1.h>
#include <conch/hmac.h>


/* @func: conch_hmac_sha1_init
 * #desc:
 *    hmac-sha1 struct context initialization.
 *
 * #1: ctx     [out] hmac-sha1 struct context
 * #2: key     [in]  input key
 * #3: key_len [in]  key length
 */
void conch_hmac_sha1_init(struct hmac_sha1_ctx *ctx, const uint8_t *key,
		uint32_t key_len)
{
	SHA1_NEW(sha1_ctx);
	conch_sha1_init(&sha1_ctx);

	if (key_len > SHA1_BLOCKSIZE) {
		conch_sha1(&sha1_ctx, key, key_len);
		key = &(SHA1_STATE(&sha1_ctx, 0));
		key_len = SHA1_LEN;
	}

	conch_memset(ctx->ipad, 0x36, SHA1_BLOCKSIZE);
	conch_memset(ctx->opad, 0x5c, SHA1_BLOCKSIZE);
	for (uint32_t i = 0; i < key_len; i++) {
		ctx->ipad[i] ^= key[i];
		ctx->opad[i] ^= key[i];
	}

	conch_sha1_init(&ctx->ipad_ctx);
	conch_sha1_init(&ctx->opad_ctx);
	conch_sha1_process(&ctx->ipad_ctx,
		ctx->ipad, SHA1_BLOCKSIZE);

	ctx->state = &(SHA1_STATE(&ctx->opad_ctx, 0));
}

/* @func: conch_hmac_sha1_process
 * #desc:
 *    hmac-sha1 processing buffer function.
 *
 * #1: ctx [in/out] hmac-sha1 struct context
 * #2: s   [in]     input buffer
 * #3: len [in]     input length
 */
void conch_hmac_sha1_process(struct hmac_sha1_ctx *ctx, const uint8_t *s,
		size_t len)
{
	conch_sha1_process(&ctx->ipad_ctx, s, len);
}

/* @func: conch_hmac_sha1_finish
 * #desc:
 *    hmac-sha1 process the remaining bytes in the buffer and end.
 *
 * #1: ctx [in/out] hmac-sha1 struct context
 * #2: len [in]     total length
 */
void conch_hmac_sha1_finish(struct hmac_sha1_ctx *ctx, uint64_t len)
{
	conch_sha1_finish(&ctx->ipad_ctx, len + SHA1_BLOCKSIZE);

	conch_sha1_process(&ctx->opad_ctx,
		ctx->opad, SHA1_BLOCKSIZE);
	conch_sha1_process(&ctx->opad_ctx,
		&(SHA1_STATE(&ctx->ipad_ctx, 0)), SHA1_LEN);
	conch_sha1_finish(&ctx->opad_ctx, SHA1_BLOCKSIZE + SHA1_LEN);
}

/* @func: conch_hmac_sha1
 * #desc:
 *    hmac-sha1 single-time processing function.
 *
 * #1: ctx [in/out] hmac-sha1 struct context
 * #2: s   [in]     input buffer
 * #3: len [in]     input length
 */
void conch_hmac_sha1(struct hmac_sha1_ctx *ctx, const uint8_t *s,
		size_t len)
{
	conch_hmac_sha1_process(ctx, s, len);
	conch_hmac_sha1_finish(ctx, len);
}
