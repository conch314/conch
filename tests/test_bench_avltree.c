/* @file: test_bench_avltree.c
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
#include <stdlib.h>
#include <time.h>
#include <conch/c_stdint.h>
#include <conch/c_stdlib.h>
#include <conch/ds_avltree.h>


#define SIZE 1000000
#define A_SIZE 10000

struct T {
	int32_t key;
	struct avl_node node;
};

int32_t cmp(void *n, void *a)
{
	int32_t key_a = AVL_ENTRY(a, struct T, node)->key;
	int32_t key_b = AVL_ENTRY(n, struct T, node)->key;

	return (key_a > key_b) ? 1 : ((key_a < key_b) ? -1 : 0);
}

void test_avltree(void)
{
	clock_t start, end;
	double time;
	RANDOM_TYPE0_NEW(ran, 123456);

	struct T *node = malloc(sizeof(struct T) * SIZE);
	struct avl_node *p;
	AVL_ROOT_NEW(root);

	for (int32_t i = 0; i < SIZE; i++)
		conch_random_r(&ran, &node[i].key);

	/* insert */
	start = clock();
	for (int32_t i = 0; i < SIZE; i++) {
		if (!conch_avl_insert(&root, &node[i].node, cmp))
			printf("collision i:%d k:%d\n", i, node[i].key);
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("insert: %d -- %.6fs (%.2f/s) %.2f ns/op\n",
		SIZE, time,
		(double)SIZE / time,
		(double)(time * 1000000000) / SIZE);

	/* first and last */
	p = conch_avl_first(&root);
	printf("first: k:%d %p\n", AVL_ENTRY(p, struct T, node)->key, p);

	p = conch_avl_last(&root);
	printf("last: k:%d %p\n", AVL_ENTRY(p, struct T, node)->key, p);

	/* search */
	start = clock();
	for (int32_t i = 0; i < SIZE; i++) {
		if (!conch_avl_search(&root, &node[i].node, cmp))
			printf("not found: i:%d k:%d\n", i, node[i].key);
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("search: %d -- %.6fs (%.2f/s) %.2f ns/op\n",
		SIZE, time,
		(double)SIZE / time,
		(double)(time * 1000000000) / SIZE);

	/* erase access */
	start = clock();
	for (int32_t i = 0; i < A_SIZE; i++) {
		if (!conch_avl_erase2(&root, &node[i].node, cmp))
			printf("not found: i:%d k:%d\n", i, node[i].key);
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("erase access: %d -- %.6fs (%.2f/s) %.2f ns/op\n",
		A_SIZE, time,
		(double)A_SIZE / time,
		(double)(time * 1000000000) / A_SIZE);

	/* erase */
	start = clock();
	for (int32_t i = A_SIZE; i < SIZE; i++) {
		if (!conch_avl_erase2(&root, &node[i].node, cmp))
			printf("not found: i:%d k:%d\n", i, node[i].key);
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("erase: %d -- %.6fs (%.2f/s) %.2f ns/op\n",
		SIZE - A_SIZE, time,
		(double)(SIZE - A_SIZE) / time,
		(double)(time * 1000000000) / (SIZE - A_SIZE));
}

int main(void)
{
	test_avltree();

	return 0;
}
