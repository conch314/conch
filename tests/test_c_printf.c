/* @file: test_c_printf.c
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
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>
#include <conch/c_limits.h>
#include <conch/c_float.h>
#include <conch/c_math.h>
#include <conch/c_stdio.h>


void test_printf(void)
{
#define A_PRINTF(...) \
	printf(__VA_ARGS__); \
	fflush(stdout);
/*#undef A_PRINTF
#define A_PRINTF(...)*/

#define B_PRINTF(...) \
	conch_printf(__VA_ARGS__); \
	conch_fflush(x_stdout);

	/* signed */
	A_PRINTF("0.1 A: hhd: %hhd %hhd\n", INT8_MIN, INT8_MAX);
	B_PRINTF("0.2 B: hhd: %hhd %hhd\n", INT8_MIN, INT8_MAX);
	A_PRINTF("1.1 A: hhd: %hhd %hhd\n", INT32_MIN, INT32_MAX);
	B_PRINTF("1.2 B: hhd: %hhd %hhd\n", INT32_MIN, INT32_MAX);

	A_PRINTF("2.1 A: hd: %hd %hd\n", INT16_MIN, INT16_MAX);
	B_PRINTF("2.2 B: hd: %hd %hd\n", INT16_MIN, INT16_MAX);
	A_PRINTF("3.1 A: hd: %hd %hd\n", INT32_MIN, INT32_MAX);
	B_PRINTF("3.2 B: hd: %hd %hd\n", INT32_MIN, INT32_MAX);

	A_PRINTF("4.1 A: d: %d %d\n", INT32_MIN, INT32_MAX);
	B_PRINTF("4.2 B: d: %d %d\n", INT32_MIN, INT32_MAX);
	A_PRINTF("5.1 A: d: %d %d\n", INT64_MIN, INT64_MAX);
	B_PRINTF("5.2 B: d: %d %d\n", INT64_MIN, INT64_MAX);

	A_PRINTF("6.1 A: ld: %ld %ld\n", INT64_MIN, INT64_MAX);
	B_PRINTF("6.2 B: ld: %ld %ld\n", INT64_MIN, INT64_MAX);

	A_PRINTF("7.1 A: lld: %lld %lld\n", INT64_MIN, INT64_MAX);
	B_PRINTF("7.2 B: lld: %lld %lld\n", INT64_MIN, INT64_MAX);

	/* unsigned */
	A_PRINTF("8.1 A: hhu: %hhu\n", UINT8_MAX);
	B_PRINTF("8.2 B: hhu: %hhu\n", UINT8_MAX);

	A_PRINTF("9.1 A: hhu: %hhu\n", UINT32_MAX);
	B_PRINTF("9.2 B: hhu: %hhu\n", UINT32_MAX);

	A_PRINTF("10.1 A: hu: %hu\n", UINT16_MAX);
	B_PRINTF("10.2 B: hu: %hu\n", UINT16_MAX);
	A_PRINTF("11.1 A: hu: %hu\n", UINT32_MAX);
	B_PRINTF("11.2 B: hu: %hu\n", UINT32_MAX);

	A_PRINTF("12.1 A: u: %u\n", UINT32_MAX);
	B_PRINTF("12.2 B: u: %u\n", UINT32_MAX);
	A_PRINTF("13.1 A: u: %u\n", UINT64_MAX);
	B_PRINTF("13.2 B: u: %u\n", UINT64_MAX);

	A_PRINTF("14.1 A: lu: %lu\n", UINT64_MAX);
	B_PRINTF("14.2 B: lu: %lu\n", UINT64_MAX);

	/* signed */
	A_PRINTF("15.1 A: hhx: %hhx %hhx\n", INT8_MIN, INT8_MAX);
	B_PRINTF("15.2 B: hhx: %hhx %hhx\n", INT8_MIN, INT8_MAX);
	A_PRINTF("16.1 A: hhx: %hhx %hhx\n", INT32_MIN, INT32_MAX);
	B_PRINTF("16.2 B: hhx: %hhx %hhx\n", INT32_MIN, INT32_MAX);

	A_PRINTF("17.1 A: hx: %hx %hx\n", INT16_MIN, INT16_MAX);
	B_PRINTF("17.2 B: hx: %hx %hx\n", INT16_MIN, INT16_MAX);
	A_PRINTF("18.1 A: hx: %hx %hx\n", INT32_MIN, INT32_MAX);
	B_PRINTF("18.2 B: hx: %hx %hx\n", INT32_MIN, INT32_MAX);

	A_PRINTF("19.1 A: x: %x %x\n", INT32_MIN, INT32_MAX);
	B_PRINTF("19.2 B: x: %x %x\n", INT32_MIN, INT32_MAX);
	A_PRINTF("20.1 A: x: %x %x\n", INT64_MIN, INT64_MAX);
	B_PRINTF("20.2 B: x: %x %x\n", INT64_MIN, INT64_MAX);

	A_PRINTF("21.1 A: lx: %lx %lx\n", INT64_MIN, INT64_MAX);
	B_PRINTF("21.2 B: lx: %lx %lx\n", INT64_MIN, INT64_MAX);

	A_PRINTF("22.1 A: llx: %llx %llx\n", INT64_MIN, INT64_MAX);
	B_PRINTF("22.2 B: llx: %llx %llx\n", INT64_MIN, INT64_MAX);

	/* unsigned hex */
	A_PRINTF("23.1 A: llu: %llu\n", UINT64_MAX);
	B_PRINTF("23.2 B: llu: %llu\n", UINT64_MAX);

	A_PRINTF("24.1 A: hhx: %hhx\n", UINT8_MAX);
	B_PRINTF("24.2 B: hhx: %hhx\n", UINT8_MAX);

	A_PRINTF("25.1 A: hhx: %hhx\n", UINT32_MAX);
	B_PRINTF("25.2 B: hhx: %hhx\n", UINT32_MAX);

	A_PRINTF("26.1 A: hx: %hx\n", UINT16_MAX);
	B_PRINTF("26.2 B: hx: %hx\n", UINT16_MAX);
	A_PRINTF("27.1 A: hx: %hx\n", UINT32_MAX);
	B_PRINTF("27.2 B: hx: %hx\n", UINT32_MAX);

	A_PRINTF("28.1 A: x: %x\n", UINT32_MAX);
	B_PRINTF("28.2 B: x: %x\n", UINT32_MAX);
	A_PRINTF("29.1 A: x: %x\n", UINT64_MAX);
	B_PRINTF("29.2 B: x: %x\n", UINT64_MAX);

	A_PRINTF("30.1 A: lx: %lx\n", UINT64_MAX);
	B_PRINTF("30.2 B: lx: %lx\n", UINT64_MAX);

	A_PRINTF("31.1 A: llx: %llx\n", UINT64_MAX);
	B_PRINTF("31.2 B: llx: %llx\n", UINT64_MAX);

	/* floating */
	A_PRINTF("32.1 A: f: %.800f\n", X_FP_DBL_MAX);
	B_PRINTF("32.2 B: f: %.800f\n", X_FP_DBL_MAX);

	A_PRINTF("33.1 A: f: %.380f\n", -X_FP_DBL_MAX);
	B_PRINTF("33.2 B: f: %.380f\n", -X_FP_DBL_MAX);

	A_PRINTF("34.1 A: f: %.800f\n", X_FP_DBL_MIN);
	B_PRINTF("34.2 B: f: %.800f\n", X_FP_DBL_MIN);

	A_PRINTF("35.1 A: f: %.380f\n", -X_FP_DBL_MIN);
	B_PRINTF("35.2 B: f: %.380f\n", -X_FP_DBL_MIN);

	A_PRINTF("36.1 A: f: %.380f\n", -X_FP_DBL_MIN);
	B_PRINTF("36.2 B: f: %.380f\n", -X_FP_DBL_MIN);

	/* character and string */
	A_PRINTF("37.1 A: c: %c\n", 'w');
	B_PRINTF("37.2 B: c: %c\n", 'w');

	A_PRINTF("38.1 A: c: %9c\n", 'w');
	B_PRINTF("38.2 B: c: %9c\n", 'w');

	A_PRINTF("39.1 A: s: %s\n", "Hello, World");
	B_PRINTF("39.2 B: s: %s\n", "Hello, World");

	A_PRINTF("40.1 A: s: %.6s\n", "Hello, World");
	B_PRINTF("40.2 B: s: %.6s\n", "Hello, World");

	A_PRINTF("41.1 A: s: %19s\n", "Hello, World");
	B_PRINTF("41.2 B: s: %19s\n", "Hello, World");

	A_PRINTF("42.1 A: s: %-19s-\n", "Hello, World");
	B_PRINTF("42.2 B: s: %-19s-\n", "Hello, World");

	A_PRINTF("43.1 A: s: %33s-\n", "Hello, World");
	B_PRINTF("43.2 B: s: %33s-\n", "Hello, World");

	A_PRINTF("44.1 A: s: %33s ω %s-\n", "Hello, World ω", NULL);
	B_PRINTF("44.2 B: s: %33s ω %s-\n", "Hello, World ω", NULL);

	A_PRINTF("45.1 A: s: %*s ω %s-\n", -33, "Hello, World ω", NULL);
	B_PRINTF("45.2 B: s: %*s ω %s-\n", -33, "Hello, World ω", NULL);

	/* align */
	A_PRINTF("46.1 A: %20lx$\n", INT64_MIN);
	B_PRINTF("46.2 B: %20lx$\n", INT64_MIN);
	A_PRINTF("47.1 A: %020lx$\n", INT64_MIN);
	B_PRINTF("47.2 B: %020lx$\n", INT64_MIN);

	A_PRINTF("48.1 A: %-20lx$\n", INT64_MIN);
	B_PRINTF("48.2 B: %-20lx$\n", INT64_MIN);
	A_PRINTF("49.1 A: %-020lx$\n", INT64_MIN);
	B_PRINTF("49.2 B: %-020lx$\n", INT64_MIN);

	A_PRINTF("50.1 A: %-.20lx$\n", INT64_MIN);
	B_PRINTF("50.2 B: %-.20lx$\n", INT64_MIN);
	A_PRINTF("51.1 A: %-0.20lx$\n", INT64_MIN);
	B_PRINTF("51.2 B: %-0.20lx$\n", INT64_MIN);

	A_PRINTF("52.1 A: %#.20lx$\n", INT64_MIN);
	B_PRINTF("52.2 B: %#.20lx$\n", INT64_MIN);
	A_PRINTF("53.1 A: %40.20lx$\n", INT64_MIN);
	B_PRINTF("53.2 B: %40.20lx$\n", INT64_MIN);

	A_PRINTF("54.1 A: %040c$\n", 'A');
	B_PRINTF("54.2 B: %040c$\n", 'A');

	A_PRINTF("55.1 A: %40c$\n", 'A');
	B_PRINTF("55.2 B: %40c$\n", 'A');

	A_PRINTF("56.1 A: %-40c$\n", 'A');
	B_PRINTF("56.2 B: %-40c$\n", 'A');

	/* floating */
	A_PRINTF("57.1 A: %.324f\n", 0.0);
	B_PRINTF("57.2 B: %.324f\n", 0.0);

	A_PRINTF("58.1 A: %.324f\n", 17.9729);
	B_PRINTF("58.2 B: %.324f\n", 17.9729);

	A_PRINTF("59.1 A: %.324f\n", 1.79729);
	B_PRINTF("59.2 B: %.324f\n", 1.79729);

	A_PRINTF("60.1 A: %.324f\n", 0.179729);
	B_PRINTF("60.2 B: %.324f\n", 0.179729);

	A_PRINTF("61.1 A: %.324f\n", X_FP_DBL_MIN);
	B_PRINTF("61.2 B: %.324f\n", X_FP_DBL_MIN);

	A_PRINTF("62.1 A: %.324f\n", X_FP_DBL_MAX);
	B_PRINTF("62.2 B: %.324f\n", X_FP_DBL_MAX);

	A_PRINTF("63.1 A: %.0f\n", X_FP_DBL_MIN);
	B_PRINTF("63.2 B: %.0f\n", X_FP_DBL_MIN);

	A_PRINTF("64.1 A: %.0f\n", X_FP_DBL_MAX);
	B_PRINTF("64.2 B: %.0f\n", X_FP_DBL_MAX);

	A_PRINTF("65.1 A: %.324f\n", 0.001897);
	B_PRINTF("65.2 B: %.324f\n", 0.001897);

	A_PRINTF("66.1 A: %.324f\n", 527.1481286241434);
	B_PRINTF("66.2 B: %.324f\n", 527.1481286241434);

	A_PRINTF("66.1 A: %f\n", NAN);
	B_PRINTF("66.2 B: %f\n", NAN);

	A_PRINTF("67.1 A: %f\n", INFINITY);
	B_PRINTF("67.2 B: %f\n", INFINITY);

	A_PRINTF("68.1 A: %f\n", 0.0);
	B_PRINTF("68.2 B: %f\n", 0.0);
	A_PRINTF("69.1 A: %f\n", 0.1);
	B_PRINTF("69.2 B: %f\n", 0.1);
	A_PRINTF("70.1 A: %f\n", 0.2);
	B_PRINTF("70.2 B: %f\n", 0.2);
	A_PRINTF("71.1 A: %f\n", 0.3);
	B_PRINTF("71.2 B: %f\n", 0.3);
	A_PRINTF("71.1 A: %.30f\n", 0.3);
	B_PRINTF("71.2 B: %.30f\n", 0.3);
	A_PRINTF("72.1 A: %f\n", 0.4);
	B_PRINTF("72.2 B: %f\n", 0.4);
	A_PRINTF("73.1 A: %f\n", 0.5);
	B_PRINTF("73.2 B: %f\n", 0.5);
	A_PRINTF("74.1 A: %f\n", 0.6);
	B_PRINTF("74.2 B: %f\n", 0.6);
	A_PRINTF("75.1 A: %f\n", 0.7);
	B_PRINTF("75.2 B: %f\n", 0.7);
	A_PRINTF("76.1 A: %f\n", 0.8);
	B_PRINTF("76.2 B: %f\n", 0.8);
	A_PRINTF("77.1 A: %f\n", 0.9);
	B_PRINTF("77.2 B: %f\n", 0.9);
	A_PRINTF("78.1 A: %f\n", 1.0);
	B_PRINTF("78.2 B: %f\n", 1.0);
	A_PRINTF("79.1 A: %f\n", 1.1);
	A_PRINTF("79.2 B: %f\n", 1.1);
	B_PRINTF("80.1 A: %f\n", 1.9);
	B_PRINTF("80.2 B: %f\n", 1.9);
	B_PRINTF("81.1 A: %f\n", 2.0);
	B_PRINTF("81.2 B: %f\n", 2.0);
	B_PRINTF("82.1 A: %f\n", 2.1);
	B_PRINTF("82.2 B: %f\n", 2.1);
}

int main(void)
{
	test_printf();

	return 0;
}
