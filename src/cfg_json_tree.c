/* @file: cfg_json_tree.c
 * #desc:
 *    The implementations of json tree parser.
 *
 * #copy:
 *    Copyright (C) 1970 Public Free Software.
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


/* @func: _json_value_add (static)
 * #desc:
 *    add a new json object or array and returns a json value pointer.
 *
 * #1: s [in/out] stack struct of json tree
 * #r:   [ret]    json value pointer
 */
static struct json_value *_json_value_add(struct json_stack *s)
{
	struct json_value *v = conch_malloc(sizeof(struct json_value));

	if (s->type == JSON_OBJECT_TYPE) {
		v->name = s->name;
		s->name = NULL;
	} else {
		v->name = NULL;
	}

	v->prev = s->tmp_ao;
	v->next = NULL;
	if (*s->ptr_ao) {
		s->tmp_ao->next = v;
	} else {
		*s->ptr_ao = v;
	}
	s->tmp_ao = v;

	return v;
}

/* @func: _call (static)
 * #desc:
 *    callback function of json parse.
 *
 * #1: type [in]  json type
 * #2: str  [in]  input string
 * #3: len  [in]  input length
 * #4: arg  [in]  callback arg
 * #r:      [ret] 0: no error, -1: callback error
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
			ss->type = JSON_ARRAY_TYPE;
			ss->tmp_ao = NULL;

			if (!t->type) {
				t->type = JSON_ARRAY_TYPE;
				t->ao = NULL;
				ss->ptr_ao = &t->ao;
			} else {
				v = _json_value_add(s);
				v->type = JSON_ARRAY_TYPE;
				v->u.ao = NULL;
				ss->ptr_ao = &v->u.ao;
			}

			ss->next = s;
			t->stack = ss;
			break;
		case JSON_OBJECT_TYPE:
			ss = conch_malloc(sizeof(struct json_stack));
			ss->type = JSON_OBJECT_TYPE;
			ss->tmp_ao = NULL;

			if (!t->type) {
				t->type = JSON_OBJECT_TYPE;
				t->ao = NULL;
				ss->ptr_ao = &t->ao;
			} else {
				v = _json_value_add(s);
				v->type = JSON_OBJECT_TYPE;
				v->u.ao = NULL;
				ss->ptr_ao = &v->u.ao;
			}

			ss->next = s;
			t->stack = ss;
			break;
		case JSON_OBJKEY_TYPE:
			p = conch_strndup(str, (size_t)len);
			s->name = p;
			break;
		case JSON_STRING_TYPE:
			p = conch_strndup(str, (size_t)len);
			v = _json_value_add(s);
			v->type = JSON_STRING_TYPE;
			v->u.str = p;
			break;
		case JSON_NUMBER_DEC_TYPE:
			p = conch_strndup(str, (size_t)len);
			v = _json_value_add(s);
			v->type = JSON_NUMBER_DEC_TYPE;
			v->u.i = conch_strtoll(p, NULL, 10);
			conch_free(p);
			break;
		case JSON_NUMBER_HEX_TYPE:
			p = conch_strndup(str, (size_t)len);
			v = _json_value_add(s);
			v->type = JSON_NUMBER_HEX_TYPE;
			v->u.i = conch_strtoll(p, NULL, 16);
			conch_free(p);
			break;
		case JSON_NUMBER_FLT_TYPE:
			p = conch_strndup(str, (size_t)len);
			v = _json_value_add(s);
			v->type = JSON_NUMBER_FLT_TYPE;
			v->u.f = conch_strtod(p, NULL);
			conch_free(p);
			break;
		case JSON_NUMBER_INF_TYPE:
			v = _json_value_add(s);
			v->type = JSON_NUMBER_INF_TYPE;
			break;
		case JSON_NUMBER_NAN_TYPE:
			v = _json_value_add(s);
			v->type = JSON_NUMBER_NAN_TYPE;
			break;
		case JSON_NULL_TYPE:
			v = _json_value_add(s);
			v->type = JSON_NULL_TYPE;
			break;
		case JSON_TRUE_TYPE:
			v = _json_value_add(s);
			v->type = JSON_TRUE_TYPE;
			break;
		case JSON_FALSE_TYPE:
			v = _json_value_add(s);
			v->type = JSON_FALSE_TYPE;
			break;
		case JSON_ARRAY_END_TYPE:
		case JSON_OBJECT_END_TYPE:
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
 * #1: tree [in/out] json tree struct
 * #2: s    [in]     input string
 * #r:      [ret]    0: no error, -1: error, -2: callback error
 */
int32_t conch_json_tree_parse(struct json_tree *tree, const char *s)
{
	struct json_stack *_s, *ss;

	int32_t ret = conch_json_parse(s, &tree->err_len, &tree->err,
		tree, _call);
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

/* @func: _tree_free_value (static)
 * #desc:
 *    free the list of json tree value.
 *
 * #1: v [in/out] json value list
 */
static void _tree_free_value(struct json_value *v)
{
	struct json_value *t = v;
	while (v) {
		switch (v->type) {
			case JSON_ARRAY_TYPE:
				_tree_free_value(v->u.ao);
				break;
			case JSON_OBJKEY_TYPE:
				_tree_free_value(v->u.ao);
				conch_free(v->name);
				break;
			case JSON_STRING_TYPE:
				conch_free(v->u.str);
				break;
			default:
				break;
		}

		t = v;
		v = v->next;
		if (t)
			conch_free(t);
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
	_tree_free_value(tree->ao);
	tree->ao = NULL;
}
