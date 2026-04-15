/* @file: cfg_json.h
 * #desc:
 *    The definitions of json (javascript object notation) configuration.
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

#ifndef _CONCH_CFG_JSON_H
#define _CONCH_CFG_JSON_H

#include <conch/config.h>
#include <conch/c_stdint.h>


/* json callback type */
#define JSON_ARRAY_TYPE 1
#define JSON_OBJECT_TYPE 2
#define JSON_OBJKEY_TYPE 3
#define JSON_STRING_TYPE 4
#define JSON_NUMBER_DEC_TYPE 5
#define JSON_NUMBER_HEX_TYPE 6
#define JSON_NUMBER_FLT_TYPE 7
#define JSON_NUMBER_INF_TYPE 8
#define JSON_NUMBER_NAN_TYPE 9
#define JSON_NULL_TYPE 10
#define JSON_TRUE_TYPE 11
#define JSON_FALSE_TYPE 12

/* json error code */
#define JSON_ERR_INVALID 1
#define JSON_ERR_START_TOKEN 2
#define JSON_ERR_OBJECT_TOKEN 3
#define JSON_ERR_OBJECT_END 4
#define JSON_ERR_OBJECT_OBJKEY 5
#define JSON_ERR_OBJECT_STRING 6
#define JSON_ERR_OBJECT_NUMBER 7
#define JSON_ERR_ARRAY_TOKEN 8
#define JSON_ERR_ARRAY_END 9
#define JSON_ERR_ARRAY_STRING 10
#define JSON_ERR_ARRAY_NUMBER 11
#define JSON_ERR_COMMENT 12

struct json_ctx {
	const char *str;
	int32_t len;
	int32_t err;
	void *arg;
	/* type, string, length, arg */
	int32_t (*call)(int32_t, const char *, int32_t, void *);
	/* type, arg */
	int32_t (*call_end)(int32_t, void *);
};

#define JSON_CTX_SET(_call, _call_end, _arg) \
	{ \
		.call = _call, \
		.call_end = _call_end, \
		.arg = _arg \
	}
#define JSON_NEW(name, _call, _call_end, _arg) \
	struct json_ctx name = JSON_CTX_SET(_call, _call_end, _arg)
#define JSON_INIT(x, _call, _call_end, _arg) \
	(x)->call = _call; \
	(x)->call_end = _call_end; \
	(x)->arg = _arg

#define JSON_STR(x) ((x)->str)
#define JSON_ERR(x) ((x)->err)
#define JSON_LEN(x) ((x)->len)


/* json tree */
struct json_value {
	int32_t type;
	union {
		int64_t i;
		double f;
		char *str;
		struct json_array *array;
		struct json_object *object;
	} u;
};

struct json_array {
	struct json_value value;
	struct json_array *next;
};

struct json_object {
	char *name;
	struct json_value value;
	struct json_object *next;
};

struct json_stack {
	int32_t type;
	char *name;
	union {
		struct json_array **array;
		struct json_object **object;
	} u;
	struct json_stack *next;
};

struct json_tree {
	int32_t type;
	union {
		struct json_array *array;
		struct json_object *object;
	} u;
	struct json_stack *stack;
	struct json_ctx ctx;
};

#define JSON_TREE_SET0 { 0, .u = { NULL }, NULL }
#define JSON_TREE_NEW(x) struct json_tree x = JSON_TREE_SET0
#define JSON_TREE_INIT(x) \
	(x)->type = 0; \
	(x)->u.array = NULL; \
	(x)->stack = NULL


#ifdef __cplusplus
extern "C" {
#endif

/* cfg_json_parse.c */
extern
int32_t conch_json_parse(struct json_ctx *ctx, const char *s)
;

/* cfg_json_tree.c */
extern
int32_t conch_json_tree_parse(struct json_tree *tree, const char *s)
;
extern
void conch_json_tree_free(struct json_tree *tree)
;

#ifdef __cplusplus
}
#endif


#endif
