/* @file: test_json_tree.c
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

#include <conch/c_stddef.h>
#include <conch/c_stdint.h>
#include <conch/c_string.h>
#include <conch/c_stdio.h>
#include <conch/c_stdlib.h>
#include <conch/cfg_json.h>


void _print_array(struct json_array *a, int32_t ind);
void _print_object(struct json_object *o, int32_t ind);

void _print_array(struct json_array *a, int32_t ind)
{
	struct json_value *v;

	for (; a; a = a->next) {
		v = &a->value;
		switch (v->type) {
			case JSON_ARRAY_TYPE:
				conch_printf("%*s[\n", ind, "");
				_print_array(v->u.array, ind + 2);
				conch_printf("%*s]", ind, "");
				break;
			case JSON_OBJECT_TYPE:
				conch_printf("%*s{\n", ind, "");
				_print_object(v->u.object, ind + 2);
				conch_printf("%*s}", ind, "");
				break;
			case JSON_STRING_TYPE:
				conch_printf("%*s\"%s\"", ind, "", v->u.str);
				break;
			case JSON_NUMBER_DEC_TYPE:
				conch_printf("%*s%lld", ind, "", v->u.i);
				break;
			case JSON_NUMBER_HEX_TYPE:
				conch_printf("%*s%llx", ind, "", v->u.i);
				break;
			case JSON_NUMBER_FLT_TYPE:
				conch_printf("%*s%f", ind, "", v->u.f);
				break;
			case JSON_NUMBER_INF_TYPE:
				conch_printf("%*s%f", ind, "", v->u.f);
				break;
			case JSON_NUMBER_NAN_TYPE:
				conch_printf("%*s%f", ind, "", v->u.f);
				break;
			case JSON_NULL_TYPE:
				conch_printf("%*snull", ind, "");
				break;
			case JSON_TRUE_TYPE:
				conch_printf("%*strue", ind, "");
				break;
			case JSON_FALSE_TYPE:
				conch_printf("%*sfalse", ind, "");
				break;
			default:
				return;
		}
		if (a->next)
			conch_printf(",");
		conch_printf("\n");
	}
}

void _print_object(struct json_object *o, int32_t ind)
{
	struct json_value *v;

	for (; o; o = o->next) {
		v = &o->value;
		switch (v->type) {
			case JSON_ARRAY_TYPE:
				conch_printf("%*s\"%s\": [\n", ind, "", o->name);
				_print_array(v->u.array, ind + 2);
				conch_printf("%*s]", ind, "");
				break;
			case JSON_OBJECT_TYPE:
				conch_printf("%*s\"%s\": {\n", ind, "", o->name);
				_print_object(v->u.object, ind + 2);
				conch_printf("%*s}", ind, "");
				break;
			case JSON_STRING_TYPE:
				conch_printf("%*s\"%s\": \"%s\"", ind, "", o->name, v->u.str);
				break;
			case JSON_NUMBER_DEC_TYPE:
				conch_printf("%*s\"%s\": %lld", ind, "", o->name, v->u.i);
				break;
			case JSON_NUMBER_HEX_TYPE:
				conch_printf("%*s\"%s\": %llx", ind, "", o->name, v->u.i);
				break;
			case JSON_NUMBER_FLT_TYPE:
				conch_printf("%*s\"%s\": %f", ind, "", o->name, v->u.f);
				break;
			case JSON_NUMBER_INF_TYPE:
				conch_printf("%*s\"%s\": %f", ind, "", o->name, v->u.f);
				break;
			case JSON_NUMBER_NAN_TYPE:
				conch_printf("%*s\"%s\": %f", ind, "", o->name, v->u.f);
				break;
			case JSON_NULL_TYPE:
				conch_printf("%*s\"%s\": null", ind, "", o->name);
				break;
			case JSON_TRUE_TYPE:
				conch_printf("%*s\"%s\": true", ind, "", o->name);
				break;
			case JSON_FALSE_TYPE:
				conch_printf("%*s\"%s\": false", ind, "", o->name);
				break;
			default:
				return;
		}
		if (o->next)
			conch_printf(",");
		conch_printf("\n");
	}
}

void _print(struct json_tree *t, int32_t ind)
{
	if (t->type == JSON_OBJECT_TYPE) {
		conch_printf("{\n");
		_print_object(t->u.object, ind);
		conch_printf("}\n");
	} else {
		conch_printf("[\n");
		_print_array(t->u.array, ind);
		conch_printf("]\n");
	}
}

int main(void)
{
	JSON_TREE_NEW(tree);
	char *s = conch_malloc(4 << 20);
	conch_memset(s, 0, 4 << 20);
	conch_fread(s, 1, ((4 << 20) - 1), x_stdin);

	conch_json_tree_parse(&tree, s
	/*	"[ true, { '': [ 1.23, {} ], '': { '': false } }, [ ] ]"*/);

	_print(&tree, 2);
	conch_json_tree_free(&tree);
	conch_free(s);

	return 0;
}
