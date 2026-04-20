/* @file: c_start_main.c
 * #desc:
 *    The implementations of c runtime entry.
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
#include <conch/c_stdlib.h>


char **__conch_envp = NULL;

extern
int main(int argc, char *const *argv, char *const *envp)
;


/* @func: _start_main
 * #desc:
 *    c runtime entry.
 *
 * #1: sp [in/out] stack pointer
 */
void _start_main(long *sp)
{
	int argc = (int)sp[0];
	char **argv = (char **)&sp[1];
	__conch_envp = (char **)&sp[argc + 2];

	int r = main(argc, argv, __conch_envp);

	conch_Exit(r);
}
