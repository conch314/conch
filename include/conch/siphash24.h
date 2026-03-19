/* @file: siphash24.h
 * #desc:
 *    The definitions of siphash message authentication code.
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

#ifndef _CONCH_SIPHASH24_H
#define _CONCH_SIPHASH24_H

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>


#define SIPHASH24_KEYLEN 16


#ifdef __cplusplus
extern "C" {
#endif

/* siphash24.c */
extern
uint64_t conch_siphash24(const uint8_t *s, size_t len, const uint8_t *key)
;
extern
uint64_t conch_siphashx24(const uint8_t *s, size_t len, const uint8_t *key,
		uint64_t *h2)
;

#ifdef __cplusplus
}
#endif


#endif
