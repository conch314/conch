/* @file: pw_bcrypt.h
 * #desc:
 *    The definitions of bcrypt password-hash.
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

#ifndef _CONCH_PW_BCRYPT_H
#define _CONCH_PW_BCRYPT_H

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>


#define BCRYPT_HASHPASS_LEN 24


#ifdef __cplusplus
extern "C" {
#endif

/* bcrypt.c */
extern
void conch_bcrypt_hashpass(const uint8_t *pass, uint32_t pass_len,
		const uint8_t *salt, uint32_t salt_len, uint8_t *ohp,
		uint32_t k)
;

#ifdef __cplusplus
}
#endif


#endif
