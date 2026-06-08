/* @file: cfg_ini.h
 * #desc:
 *    The definitions of ini (initial configuration) parser.
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

#ifndef _CONCH_CFG_INI_H
#define _CONCH_CFG_INI_H

#include <conch/config.h>
#include <conch/c_stdint.h>


/* ini callback type */
#define INI_SECTION_TYPE 1
#define INI_KEY_TYPE 2
#define INI_VALUE_TYPE 3


#ifdef __cplusplus
extern "C" {
#endif

/* cfg_ini_parse.c */
extern
int32_t conch_ini_parse(const char *s, int32_t *err_line, void *arg,
		int32_t (*call)(int32_t, const char *, int32_t, void *))
;

#ifdef __cplusplus
}
#endif


#endif
