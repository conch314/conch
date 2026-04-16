/* @file: c_setjmp.h
 * #desc:
 *    The definitions of stack environment.
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

#ifndef _CONCH_C_SETJMP_H
#define _CONCH_C_SETJMP_H

#include <conch/config.h>
#include <conch/c_stdint.h>


struct jmpbuf {
	unsigned long reg[128];
};


#ifdef __cplusplus
extern "C" {
#endif

/* c_setjmp.S */
extern
int32_t conch_setjmp(struct jmpbuf *env)
;
extern
void conch_longjmp(struct jmpbuf *env, int32_t val)
;

#ifdef __cplusplus
}
#endif


#endif
