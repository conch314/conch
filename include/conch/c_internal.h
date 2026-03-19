/* @file: c_internal.h
 * #desc:
 *    The definitions of.
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

#ifndef _CONCH_C_INTERNAL_H
#define _CONCH_C_INTERNAL_H

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>


struct stdio_file {
	int32_t flags;
	int32_t fd;
	uint8_t *buf;
	size_t buf_size;
	uint8_t *rpos;
	uint8_t *rend;
	uint8_t *wbase;
	uint8_t *wpos;
	uint8_t *wend;
};

#define STDIO_FILE_FG_EOF 0x01
#define STDIO_FILE_FG_ERR 0x02
#define STDIO_FILE_FG_NORD 0x04
#define STDIO_FILE_FG_NOWR 0x08
#define STDIO_FILE_FG_SEEK 0x10
#define STDIO_FILE_FG_TEXT 0x20



#endif
