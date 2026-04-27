/* @file: test_kfifo.c
 * #desc:
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
#include <conch/kfifo.h>


void test_kfifo(void)
{
	uint8_t buf[4096], out[4096];
	uint32_t r;
	KFIFO_NEW(head, buf, 4096);

	r = conch_kfifo_in(&head, (uint8_t *)"Hello, world!", 13);
	printf("in: %d:%u\n", 13, r);

	r = conch_kfifo_peek(&head, out, 4096, 2);
	printf("peek: %d-%d:%u\n", 4096, 2, r);
	out[r] = '\0';
	printf(": %s\n", out);

	r = conch_kfifo_peek(&head, out, 10, 4096);
	printf("peek: %d-%d:%u\n", 10, 4096, r);
	out[r] = '\0';
	printf(": %s\n", out);

	r = conch_kfifo_out(&head, out, 13 - 2);
	printf("out: %d:%u\n", 13 - 2, r);
	out[r] = '\0';
	printf(": %s\n", out);

	head.in += ((0xfffffff - 20) + 4095) & ~4095;
	head.out += ((0xfffffff - 20) + 4095) & ~4095;

	r = conch_kfifo_in(&head, (uint8_t *)"Hello, world!", 14);
	printf("in: %d:%u\n", 14, r);

	r = conch_kfifo_out(&head, out, 4096);
	printf("out: %d:%u\n", 4096, r);
	out[r] = '\0';
	printf(": %s\n", out);
}

int main(void)
{
	test_kfifo();

	return 0;
}
