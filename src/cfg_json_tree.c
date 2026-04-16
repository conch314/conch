/* @file: cfg_json_tree.c
 * #desc:
 *    The implementations of json tree parser.
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
#include <conch/c_stdlib.h>
#include <conch/c_math.h>
#include <conch/cfg_json.h>


/* TODO: The implementation of json tree is not yet complete */

static void _tree_free_array(struct json_array *a);
static void _tree_free_object(struct json_object *o);


/* @func: _json_value_add (static)
 * #desc:
 *    add a new json object or array and returns a json value pointer.
 *
 * #1: s [in/out] stack struct of json tree
 * #r:   [ret]    json value pointer
 */
static struct json_value *_json_value_add(struct json_stack *s)
{
	struct json_value *v;
	struct json_array *a;
	struct json_object *o;

	if (s->type == JSON_OBJECT_TYPE) {
		o = conch_malloc(sizeof(struct json_object));
		if (!o)
			return NULL;
		o->name = s->name;
		s->name = NULL;
		v = &o->value;
		o->next = *s->u.object;
		*s->u.object = o;
	} else {
		a = conch_malloc(sizeof(struct json_array));
		if (!a)
			return NULL;
		v = &a->value;
		a->next = *s->u.array;
		*s->u.array = a;
	}

	return v;
}

/* @func: _call (static)
 * #desc:
 *    callback function of json parse.
 *
 * #1: type [in]  json type
 * #2: str  [in]  input string
 * #3: len  [in]  input length
 * #3: arg  [in]  callback arg
 * #r:      [ret] 0: no error, -1: call error
 */
static int32_t _call(int32_t type, const char *str, int32_t len, void *arg)
{
	struct json_tree *t = arg;
	struct json_stack *s = t->stack, *ss;
	struct json_value *v;
	char *p;

	switch (type) {
		case JSON_ARRAY_TYPE:
			ss = conch_malloc(sizeof(struct json_stack));
			if (!ss)
				return -1;
			ss->type = JSON_ARRAY_TYPE;

			if (!t->type) {
				t->type = JSON_ARRAY_TYPE;
				t->u.array = NULL;
				ss->u.array = &t->u.array;
			} else {
				v = _json_value_add(s);
				if (!v)
					return -1;
				v->type = JSON_ARRAY_TYPE;
				v->u.array = NULL;
				ss->u.array = &v->u.array;
			}

			ss->next = s;
			t->stack = ss;
			break;
		case JSON_OBJECT_TYPE:
			ss = conch_malloc(sizeof(struct json_stack));
			if (!ss)
				return -1;
			ss->type = JSON_OBJECT_TYPE;

			if (!t->type) {
				t->type = JSON_OBJECT_TYPE;
				t->u.object = NULL;
				ss->u.object = &t->u.object;
			} else {
				v = _json_value_add(s);
				if (!v)
					return -1;
				v->type = JSON_OBJECT_TYPE;
				v->u.object = NULL;
				ss->u.object = &v->u.object;
			}

			ss->next = s;
			t->stack = ss;
			break;
		case JSON_OBJKEY_TYPE:
			p = conch_strndup(str, (size_t)len);
			if (!p)
				return -1;
			s->name = p;
			break;
		case JSON_STRING_TYPE:
			p = conch_strndup(str, (size_t)len);
			if (!p)
				return -1;
			v = _json_value_add(s);
			if (!v)
				return -1;
			v->type = JSON_STRING_TYPE;
			v->u.str = p;
			break;
		case JSON_NUMBER_DEC_TYPE:
			p = conch_strndup(str, (size_t)len);
			if (!p)
				return -1;
			v = _json_value_add(s);
			if (!v) {
				conch_free(p);
				return -1;
			}
			v->type = JSON_NUMBER_DEC_TYPE;
			v->u.i = conch_strtoll(p, NULL, 10);
			conch_free(p);
			break;
		case JSON_NUMBER_HEX_TYPE:
			p = conch_strndup(str, (size_t)len);
			if (!p)
				return -1;
			v = _json_value_add(s);
			if (!v) {
				conch_free(p);
				return -1;
			}
			v->type = JSON_NUMBER_HEX_TYPE;
			v->u.i = conch_strtoll(p, NULL, 16);
			conch_free(p);
			break;
		case JSON_NUMBER_FLT_TYPE:
			p = conch_strndup(str, (size_t)len);
			if (!p)
				return -1;
			v = _json_value_add(s);
			if (!v) {
				conch_free(p);
				return -1;
			}
			v->type = JSON_NUMBER_FLT_TYPE;
			v->u.f = conch_strtod(p, NULL);
			conch_free(p);
			break;
		case JSON_NUMBER_INF_TYPE:
			v = _json_value_add(s);
			if (!v)
				return -1;
			v->type = JSON_NUMBER_INF_TYPE;
			break;
		case JSON_NUMBER_NAN_TYPE:
			v = _json_value_add(s);
			if (!v)
				return -1;
			v->type = JSON_NUMBER_NAN_TYPE;
			break;
		case JSON_NULL_TYPE:
			v = _json_value_add(s);
			if (!v)
				return -1;
			v->type = JSON_NULL_TYPE;
			break;
		case JSON_TRUE_TYPE:
			v = _json_value_add(s);
			if (!v)
				return -1;
			v->type = JSON_TRUE_TYPE;
			break;
		case JSON_FALSE_TYPE:
			v = _json_value_add(s);
			if (!v)
				return -1;
			v->type = JSON_FALSE_TYPE;
			break;
		default:
			return -1;
	}

	return 0;
}

/* @func: _call_end (static)
 * #desc:
 *    callback-end function of json parse.
 *
 * #1: type [in]  json type
 * #2: arg  [in]  callback arg
 * #r:      [ret] 0: no error, -1: call error
 */
static int32_t _call_end(int32_t type, void *arg)
{
	struct json_tree *t = arg;
	struct json_stack *s;

	switch (type) {
		case JSON_ARRAY_TYPE:
		case JSON_OBJECT_TYPE:
			s = t->stack;
			t->stack = s->next;
			conch_free(s);
			break;
		default:
			return -1;
	}

	return 0;
}

/* @func: conch_json_tree_parse
 * #desc:
 *    json tree parser.
 *
 * #1: t [in/out] json tree struct
 * #2: s [in]     input string
 * #r:   [ret]    0: no error, -1: error, -2: call error
 */
int32_t conch_json_tree_parse(struct json_tree *tree, const char *s)
{
	struct json_stack *_s, *ss;
	JSON_INIT(&tree->ctx, _call, _call_end, tree);

	int32_t ret = conch_json_parse(&tree->ctx, s);
	if (ret) {
		_s = tree->stack;
		while (_s) {
			ss = _s;
			if (_s->name)
				conch_free(_s->name);
			_s = _s->next;
			conch_free(ss);
		}
		tree->stack = NULL;
	}

	return ret;
}

/* @func: _tree_free_array (static)
 * #desc:
 *    free the list of json tree array.
 *
 * #1: a [in/out] json array list
 */
static void _tree_free_array(struct json_array *a)
{
	struct json_value *v;
	struct json_array *aa;

	while (a) {
		aa = a;
		v = &a->value;
		switch (v->type) {
			case JSON_ARRAY_TYPE:
				_tree_free_array(v->u.array);
				break;
			case JSON_OBJECT_TYPE:
				_tree_free_object(v->u.object);
				break;
			case JSON_STRING_TYPE:
				conch_free(v->u.str);
				break;
			default:
				break;
		}
		a = a->next;
		conch_free(aa);
	}
}

/* @func: _tree_free_object (static)
 * #desc:
 *    free the list of json tree object.
 *
 * #1: o [in/out] json object list
 */
static void _tree_free_object(struct json_object *o)
{
	struct json_value *v;
	struct json_object *oo;

	while (o) {
		oo = o;
		v = &o->value;
		switch (v->type) {
			case JSON_ARRAY_TYPE:
				_tree_free_array(v->u.array);
				break;
			case JSON_OBJECT_TYPE:
				_tree_free_object(v->u.object);
				break;
			case JSON_STRING_TYPE:
				conch_free(v->u.str);
				break;
			default:
				break;
		}
		conch_free(o->name);
		o = o->next;
		conch_free(oo);
	}
}

/* @func: conch_json_tree_free
 * #desc:
 *    free the json tree.
 *
 * #1: tree [in/out] json tree struct
 */
void conch_json_tree_free(struct json_tree *tree)
{
	if (tree->type == JSON_OBJECT_TYPE) {
		_tree_free_object(tree->u.object);
	} else {
		_tree_free_array(tree->u.array);
	}
	tree->u.array = NULL;
}
