/* @file: ds_rbtree.h
 * #desc:
 *    The definitions of Red-Black self-balancing binary tree.
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

#ifndef _CONCH_DS_RBTREE_H
#define _CONCH_DS_RBTREE_H

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>


struct rb_node {
#if 0
	/* $ m4 -DRB_COLOR='$1->color' \
	 * -DRB_SET_COLOR='$1->color = $2' \
	 * -DRB_PARENT='$1->parent' \
	 * -DRB_SET_PARENT='$1->parent = $2' \
	 * -DRB_PCOLOR_SET0=''
	 */
	int32_t color;
	struct rb_node *parent;
#else
	unsigned long pcolor;
#endif
	struct rb_node *left, *right;
};

struct rb_root {
	struct rb_node *node;
};

#define RB_ROOT_NEW(x) struct rb_root x = { NULL }
#define RB_ROOT_INIT(x) (x)->node = NULL

#define RB_ENTRY(x, type, member) container_of(x, type, member)

#define RB_PCOLOR_SET0(x) (x)->pcolor = 0
#define RB_PARENT(x) ((struct rb_node *)((x)->pcolor & ~3UL))
#define RB_COLOR(x) ((x)->pcolor & 1)

#define RB_SET_PARENT(x, p) \
	(x)->pcolor = ((unsigned long)(p) | RB_COLOR(x))
#define RB_SET_COLOR(x, c) \
	(x)->pcolor = ((unsigned long)RB_PARENT(x) | ((c) & 1))

#define RB_RED 0
#define RB_BLACK 1


#ifdef __cplusplus
extern "C" {
#endif

/* ds_rbtree.c */
extern
void conch_rb_insert_fix(struct rb_root *root, struct rb_node *node)
;
extern
void conch_rb_erase_fix(struct rb_root *root, struct rb_node *node)
;
extern
struct rb_node *conch_rb_insert(struct rb_root *root,
		struct rb_node *_new, int32_t (*cmp)(void *, void *))
;
extern
struct rb_node *conch_rb_search(struct rb_root *root,
		void *arg, int32_t (*cmp)(void *, void *))
;
extern
struct rb_node *conch_rb_erase(struct rb_root *root,
		struct rb_node *old)
;
extern
struct rb_node *conch_rb_erase2(struct rb_root *root,
		void *arg, int32_t (*cmp)(void *, void *))
;
extern
struct rb_node *conch_rb_first(struct rb_root *root)
;
extern
struct rb_node *conch_rb_last(struct rb_root *root)
;

#ifdef __cplusplus
}
#endif


#endif
