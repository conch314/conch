/* @file: c_string.h
 * #desc:
 *    The definitions of string operations.
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

#ifndef _CONCH_C_STRING_H
#define _CONCH_C_STRING_H

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* c_string.c */
extern
void *conch_memcpy(void *t, const void *s, size_t len)
;
extern
void *conch_memmove(void *t, const void *s, size_t len)
;
extern
void *conch_memset(void *t, int8_t c, size_t len)
;
extern
void *conch_memchr(const void *s, uint8_t c, size_t len)
;
extern
void *conch_memrchr(const void *s, uint8_t c, size_t len)
;
extern
int32_t conch_memcmp(const void *s1, const void *s2, size_t len)
;
extern
size_t conch_strlen(const char *s)
;
extern
size_t conch_strnlen(const char *s, size_t len)
;
extern
char *conch_strcpy(char *t, const char *s)
;
extern
char *conch_strncpy(char *t, const char *s, size_t len)
;
extern
char *conch_strcat(char *t, const char *s)
;
extern
char *conch_strncat(char *t, const char *s, size_t len)
;
extern
char *conch_strchr(const char *s, uint8_t c)
;
extern
char *conch_strrchr(const char *s, uint8_t c)
;
extern
int32_t conch_strcmp(const char *s1, const char *s2)
;
extern
int32_t conch_strncmp(const char *s1, const char *s2, size_t len)
;
extern
char *conch_strstr(const char *s1, const char *s2)
;
extern
char *conch_strpbrk(const char *s, const char *cs)
;
extern
size_t conch_strspn(const char *s, const char *cs)
;
extern
size_t conch_strcspn(const char *s, const char *cs)
;
extern
char *conch_strtok_r(char *s, const char *sp, char **sl)
;

#ifdef __cplusplus
}
#endif


#endif
