/* @file: test_bits_addget.c
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
#include <conch/c_stdint.h>
#include <conch/bits_add.h>
#include <conch/bits_get.h>


void test_bits_add(void)
{
	BITS_ADD_NEW(ctx);

	/* msb-first */

	/* 0b110 */
	conch_bits_add(&ctx, 0x06, 3);
	/* 0b0001 */
	conch_bits_add(&ctx, 0x01, 4);
	/* 0b01011 */
	conch_bits_add(&ctx, 0x0b, 5);

	/* 0b10001110 */
	/* 0b00000101 */

	BITS_ADD_SKIP(&ctx);

	/* 0b1'10001011'01001011 */
	conch_bits_add(&ctx, 0x18b4b, 17);

	/* 0b01001011 */
	/* 0b10001011 */
	/* 0b00000001 */

	BITS_ADD_SKIP(&ctx);

	printf("0x8e (0b10001110 msb) => 0x%02x\n", BITS_ADD_BUF(&ctx)[0]);
	printf("0x05 (0b00000101 msb) => 0x%02x\n", BITS_ADD_BUF(&ctx)[1]);
	printf("0x4b (0b01001011 msb) => 0x%02x\n", BITS_ADD_BUF(&ctx)[2]);
	printf("0x8b (0b10001011 msb) => 0x%02x\n", BITS_ADD_BUF(&ctx)[3]);
	printf("0x01 (0b00000001 msb) => 0x%02x\n", BITS_ADD_BUF(&ctx)[4]);

	printf("len: %u\n", BITS_ADD_GETSIZE(&ctx));
}

void test_bits_beadd(void)
{
	BITS_ADD_NEW(ctx);

	/* lsb-first */

	/* 0b110 */
	conch_bits_beadd(&ctx, 0x06, 3);
	/* 0b0001 */
	conch_bits_beadd(&ctx, 0x01, 4);
	/* 0b01011 */
	conch_bits_beadd(&ctx, 0x0b, 5);

	/* 0b11000010 */
	/* 0b10110000 */

	BITS_ADD_SKIP(&ctx);

	/* 0b1'10001011'01001011 */
	conch_bits_beadd(&ctx, 0x18b4b, 17);

	/* 0b11000101 */
	/* 0b10100101 */
	/* 0b10000000 */

	BITS_ADD_SKIP(&ctx);

	printf("0xc2 (0b11000010 lsb) => 0x%02x\n", BITS_ADD_BUF(&ctx)[0]);
	printf("0xb0 (0b10110000 lsb) => 0x%02x\n", BITS_ADD_BUF(&ctx)[1]);
	printf("0xc5 (0b11000101 lsb) => 0x%02x\n", BITS_ADD_BUF(&ctx)[2]);
	printf("0xa5 (0b10100101 lsb) => 0x%02x\n", BITS_ADD_BUF(&ctx)[3]);
	printf("0x80 (0b10000000 lsb) => 0x%02x\n", BITS_ADD_BUF(&ctx)[4]);

	printf("len: %u\n", BITS_ADD_GETSIZE(&ctx));
}

void test_bits_get(void)
{
	BITS_GET_NEW(ctx);
	uint32_t r;

	/* msb-first */

	conch_bits_getfill(&ctx, (uint8_t *)"\x8e\x05\x4b\x8b\x01", 5);

	/* 0b110 */
	conch_bits_get(&ctx, &r, 3, 0);
	printf("0x06 (0b110 msb) => 0x%02x\n", r);

	/* 0b0001 */
	conch_bits_get(&ctx, &r, 4, 0);
	printf("0x01 (0b0001 msb) => 0x%02x\n", r);

	/* 0b01011 */
	conch_bits_get(&ctx, &r, 5, 0);
	printf("0x0b (0b01011 msb) => 0x%02x\n", r);

	/* 0b10001110 */
	/* 0b00000101 */

	BITS_GET_SKIP(&ctx);

	/* 0b1'10001011'01001011 */
	conch_bits_get(&ctx, &r, 17, 0);
	printf("0x18b4b (0b11000101101001011 msb) => 0x%05x\n", r);

	/* 0b01001011 */
	/* 0b10001011 */
	/* 0b00000001 */

	r = conch_bits_get(&ctx, &r, 32, 1);
	printf("skip (7) unob (25) => %u %u\n", 32 - r, r);

	BITS_GET_SKIP(&ctx);
}

void test_bits_beget(void)
{
	BITS_GET_NEW(ctx);
	uint32_t r;

	/* msb-first */

	conch_bits_getfill(&ctx, (uint8_t *)"\xc2\xb0\xc5\xa5\x80", 5);

	/* 0b110 */
	conch_bits_beget(&ctx, &r, 3, 0);
	printf("0x06 (0b110 msb) => 0x%02x\n", r);

	/* 0b0001 */
	conch_bits_beget(&ctx, &r, 4, 0);
	printf("0x01 (0b0001 msb) => 0x%02x\n", r);

	/* 0b01011 */
	conch_bits_beget(&ctx, &r, 5, 0);
	printf("0x0b (0b01011 msb) => 0x%02x\n", r);

	/* 0b11000010 */
	/* 0b10110000 */

	BITS_GET_SKIP(&ctx);

	/* 0b1'10001011'01001011 */
	conch_bits_beget(&ctx, &r, 17, 0);
	printf("0x18b4b (0b11000101101001011 msb) => 0x%05x\n", r);

	/* 0b11000101 */
	/* 0b10100101 */
	/* 0b10000000 */

	r = conch_bits_beget(&ctx, &r, 32, 1);
	printf("skip (7) unob (25) => %u %u\n", 32 - r, r);

	BITS_GET_SKIP(&ctx);
}

int main(void)
{
	test_bits_add();
	test_bits_beadd();
	test_bits_get();
	test_bits_beget();

	return 0;
}
