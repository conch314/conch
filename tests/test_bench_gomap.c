/* @file: test_bench_gomap.c
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
#include <stdlib.h>
#include <time.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>
#include <conch/c_stdlib.h>
#include <conch/c_string.h>
#include <conch/ds_gomap.h>


#define TSIZE (5000000)
#define SIZE ((int32_t)(TSIZE * 0.90))
#define A_SIZE ((int32_t)(SIZE * 0.1))

struct T {
	int32_t key;
};

uint64_t hash(const void *a, size_t len)
{
	unsigned long hash = 5381;
	while (len--)
		hash = ((hash << 5) + hash) + ((char *)a)[len];

	return hash | (hash ^ 0x123456789) << 32;
}

int32_t cmp(void *a, const void *b, size_t len)
{
	int32_t key_a = ((struct T *)a)->key;
	int32_t key_b = *((int32_t *)b);
	(void)len;

	return key_a == key_b;
}

void test_gomap(void)
{
	clock_t start, end;
	double time;
	int32_t ran = 123456;

	union gomap_group *ctrl = malloc(sizeof(union gomap_group)
		* GOMAP_CTRL_ALIGN(TSIZE));
	struct T *array = malloc(sizeof(struct T)
		* GOMAP_ARRAY_ALIGN(TSIZE));
	struct T *p;

	conch_memset(array, 0,
		sizeof(struct T) * GOMAP_ARRAY_ALIGN(TSIZE));

	GOMAP_HEAD_NEW(head, ctrl, array, sizeof(struct T),
		GOMAP_ARRAY_ALIGN(TSIZE), hash, cmp);
	conch_gomap_ctrl_empty(&head);

	int32_t *array_key = conch_malloc(sizeof(int32_t) * SIZE);
	for (int32_t i = 0; i < SIZE; i++)
		array_key[i] = conch_rand_r(&ran);

	/* insert */
	start = clock();
	for (int32_t i = 0; i < SIZE; i++) {
		p = conch_gomap_insert(&head,
			&array_key[i], sizeof(int32_t));
		if (!p) {
			printf("no speac: i:%d\n", i);
		} else if (p->key) {
			printf("repeat: i:%d\n", i);
		} else {
			p->key = array_key[i];
		}
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("insert: %d (%.1f) -- %.6fs (%.2f/s) %.2f ns/op\n",
		SIZE, (double)GOMAP_FACTOR(&head) / 10,
		time,
		(double)SIZE / time,
		(double)(time * 1000000000) / SIZE);

	/* insert repeat */
	start = clock();
	for (int32_t i = 75; i < 85; i++) {
		p = conch_gomap_insert(&head,
			&array_key[i], sizeof(int32_t));
		if (!p) {
			printf("no speac: i:%d\n", i);
		} else if (p->key == array_key[i]) {
			printf("repeat: i:%d k:%d -- k:%d\n",
				i, array_key[i], p->key);
		} else {
			printf("key error: i:%d k:%d -- k:%d\n",
				i, array_key[i], p->key);
		}
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("insert repeat: %d (%.1f) -- %.6fs (%.2f/s) %.2f ns/op\n",
		10, (double)GOMAP_FACTOR(&head) / 10,
		time,
		(double)10 / time,
		(double)(time * 1000000000) / 10);

	/* find */
	start = clock();
	for (int32_t i = 0; i < SIZE; i++) {
		p = conch_gomap_find(&head,
				&array_key[i], sizeof(int32_t));
		if (!p) {
			printf("not found: i:%d\n", i);
		} else if (p->key != array_key[i]) {
			printf("key error: i:%d k:%d -- k:%d\n",
				i, array_key[i], p->key);
		}
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("find: %d (%.1f) -- %.6fs (%.2f/s) %.2f ns/op\n",
		SIZE, (double)GOMAP_FACTOR(&head) / 10,
		time,
		(double)SIZE / time,
		(double)(time * 1000000000) / SIZE);

	/* access */
	start = clock();
	for (int32_t i = 0; i < A_SIZE; i++) {
		p = conch_gomap_find(&head,
				&array_key[i], sizeof(int32_t));
		if (!p) {
			printf("not found: i:%d\n", i);
		} else if (p->key != array_key[i]) {
			printf("key error: i:%d k:%d -- k:%d\n",
				i, array_key[i], p->key);
		}
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("access: %d (%.1f) -- %.6fs (%.2f/s) %.2f ns/op\n",
		A_SIZE, (double)GOMAP_FACTOR(&head) / 10,
		time,
		(double)A_SIZE / time,
		(double)(time * 1000000000) / A_SIZE);

	/* delete access */
	start = clock();
	for (int32_t i = 0; i < A_SIZE; i++) {
		p = conch_gomap_delete(&head,
				&array_key[i], sizeof(int32_t));
		if (!p) {
			printf("not found: i:%d\n", i);
		} else if (p->key != array_key[i]) {
			printf("key error: i:%d k:%d -- k:%d\n",
				i, array_key[i], p->key);
		} else {
			p->key = 0;
		}
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("delete access: %d (%.1f) -- %.6fs (%.2f/s) %.2f ns/op\n",
		A_SIZE, (double)GOMAP_FACTOR(&head) / 10,
		time,
		(double)A_SIZE / time,
		(double)(time * 1000000000) / A_SIZE);

	/* delete */
	start = clock();
	for (int32_t i = A_SIZE; i < SIZE; i++) {
		p = conch_gomap_delete(&head,
				&array_key[i], sizeof(int32_t));
		if (!p) {
			printf("not found: i:%d\n", i);
		} else if (p->key != array_key[i]) {
			printf("key error: i:%d k:%d -- k:%d\n",
				i, array_key[i], p->key);
		} else {
			p->key = 0;
		}
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("delete: %d (%.1f) -- %.6fs (%.2f/s) %.2f ns/op\n",
		SIZE - A_SIZE, (double)GOMAP_FACTOR(&head) / 10,
		time,
		(double)(SIZE - A_SIZE) / time,
		(double)(time * 1000000000) / (SIZE - A_SIZE));

	/* insert access */
	start = clock();
	for (int32_t i = 0; i < A_SIZE; i++) {
		p = conch_gomap_insert(&head,
				&array_key[i], sizeof(int32_t));
		if (!p) {
			printf("no speac: i:%d\n", i);
		} else if (p->key) {
			printf("repeat: i:%d\n", i);
		} else {
			p->key = array_key[i];
		}
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("insert access: %d (%.1f) -- %.6fs (%.2f/s) %.2f ns/op\n",
		A_SIZE, (double)GOMAP_FACTOR(&head) / 10,
		time,
		(double)A_SIZE / time,
		(double)(time * 1000000000) / A_SIZE);
}

int main(void)
{
	test_gomap();

	return 0;
}
