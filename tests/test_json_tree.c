/* @file: test_json_tree.c
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

#include <conch/c_stddef.h>
#include <conch/c_stdint.h>
#include <conch/c_string.h>
#include <conch/c_stdio.h>
#include <conch/c_stdlib.h>
#include <conch/cfg_json.h>


void _print_array(struct json_value *v, int32_t ind);
void _print_object(struct json_value *v, int32_t ind);


void _print_array(struct json_value *v, int32_t ind)
{
	while (v) {
		switch (v->type) {
			case JSON_ARRAY_TYPE:
				conch_printf("%*s[\n",
					ind, "");
				_print_array(v->u.ao, ind + 2);
				conch_printf("%*s]",
					ind, "");
				break;
			case JSON_OBJECT_TYPE:
				conch_printf("%*s{\n",
					ind, "");
				_print_object(v->u.ao, ind + 2);
				conch_printf("%*s}",
					ind, "");
				break;
			case JSON_STRING_TYPE:
				conch_printf("%*s\"%s\"",
					ind, "", v->u.str);
				break;
			case JSON_NUMBER_DEC_TYPE:
				conch_printf("%*s%lld",
					ind, "", v->u.i);
				break;
			case JSON_NUMBER_HEX_TYPE:
				conch_printf("%*s0x%llx",
					ind, "", v->u.i);
				break;
			case JSON_NUMBER_FLT_TYPE:
				conch_printf("%*s%f",
					ind, "", v->u.f);
				break;
			case JSON_NUMBER_INF_TYPE:
				conch_printf("%*sInfinity",
					ind, "");
				break;
			case JSON_NUMBER_NAN_TYPE:
				conch_printf("%*sNaN",
					ind, "");
				break;
			case JSON_NULL_TYPE:
				conch_printf("%*snull",
					ind, "");
				break;
			case JSON_TRUE_TYPE:
				conch_printf("%*strue",
					ind, "");
				break;
			case JSON_FALSE_TYPE:
				conch_printf("%*sfalse",
					ind, "");
				break;
			default:
				return;
		}

		v = v->next;
		if (v)
			conch_printf(",");
		conch_printf("\n");
	}
}

void _print_object(struct json_value *v, int32_t ind)
{
	while (v) {
		switch (v->type) {
			case JSON_ARRAY_TYPE:
				conch_printf("%*s\"%s\": [\n",
					ind, "", v->name);
				_print_array(v->u.ao, ind + 2);
				conch_printf("%*s]",
					ind, "");
				break;
			case JSON_OBJECT_TYPE:
				conch_printf("%*s\"%s\": {\n",
					ind, "", v->name);
				_print_object(v->u.ao, ind + 2);
				conch_printf("%*s}",
					ind, "");
				break;
			case JSON_STRING_TYPE:
				conch_printf("%*s\"%s\": \"%s\"",
					ind, "", v->name, v->u.str);
				break;
			case JSON_NUMBER_DEC_TYPE:
				conch_printf("%*s\"%s\": %lld",
					ind, "", v->name, v->u.i);
				break;
			case JSON_NUMBER_HEX_TYPE:
				conch_printf("%*s\"%s\": 0x%llx",
					ind, "", v->name, v->u.i);
				break;
			case JSON_NUMBER_FLT_TYPE:
				conch_printf("%*s\"%s\": %f",
					ind, "", v->name, v->u.f);
				break;
			case JSON_NUMBER_INF_TYPE:
				conch_printf("%*s\"%s\": Infinity",
					ind, "", v->name);
				break;
			case JSON_NUMBER_NAN_TYPE:
				conch_printf("%*s\"%s\": NaN",
					ind, "", v->name);
				break;
			case JSON_NULL_TYPE:
				conch_printf("%*s\"%s\": null",
					ind, "", v->name);
				break;
			case JSON_TRUE_TYPE:
				conch_printf("%*s\"%s\": true",
					ind, "", v->name);
				break;
			case JSON_FALSE_TYPE:
				conch_printf("%*s\"%s\": false",
					ind, "", v->name);
				break;
			default:
				return;
		}

		v = v->next;
		if (v)
			conch_printf(",");
		conch_printf("\n");
	}
}

void _print(struct json_tree *t, int32_t ind)
{
	if (t->type == JSON_OBJECT_TYPE) {
		conch_printf("{\n");
		_print_object(t->ao, ind);
		conch_printf("}\n");
	} else {
		conch_printf("[\n");
		_print_array(t->ao, ind);
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
