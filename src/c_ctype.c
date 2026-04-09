/* @file: c_ctype.c
 * #desc:
 *    The implementations of character types.
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
#include <conch/c_ctype.h>


/* @func: conch_isalnum
 * #desc:
 *    check if it is a letter or number.
 *
 * #1: c [in]  character
 * #r:   [ret] 1: true, 0: false
 */
int conch_isalnum(int c)
{
	if (c >= 'A' && c <= 'Z')
		return 1;
	if (c >= 'a' && c <= 'z')
		return 1;
	if (c >= '0' && c <= '9')
		return 1;

	return 0;
}

/* @func: conch_isalpha
 * #desc:
 *    check if it is a letter.
 *
 * #1: c [in]  character
 * #r:   [ret] 1: true, 0: false
 */
int conch_isalpha(int c)
{
	if (c >= 'A' && c <= 'Z')
		return 1;
	if (c >= 'a' && c <= 'z')
		return 1;

	return 0;
}

/* @func: conch_isblank
 * #desc:
 *    check if it is a horizontal blank space character.
 *
 * #1: c [in]  character
 * #r:   [ret] 1: true, 0: false
 */
int conch_isblank(int c)
{
	if (c == ' ' || c == '\t')
		return 1;

	return 0;
}

/* @func: conch_iscntrl
 * #desc:
 *    check if it is a non-printable control character.
 *
 * #1: c [in]  character
 * #r:   [ret] 1: true, 0: false
 */
int conch_iscntrl(int c)
{
	if (c >= 0x00 && c <= 0x1f)
		return 1;
	if (c == 0x7f)
		return 1;

	return 0;
}

/* @func: conch_isdigit
 * #desc:
 *    check if it is a number.
 *
 * #1: c [in]  character
 * #r:   [ret] 1: true, 0: false
 */
int conch_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return 1;

	return 0;
}

/* @func: conch_isgraph
 * #desc:
 *    check if it is a displayable character.
 *
 * #1: c [in]  character
 * #r:   [ret] 1: true, 0: false
 */
int conch_isgraph(int c)
{
	if (c > 0x20 && c < 0x7f)
		return 1;

	return 0;
}

/* @func: conch_islower
 * #desc:
 *    check if it is a lowercase letter.
 *
 * #1: c [in]  character
 * #r:   [ret] 1: true, 0: false
 */
int conch_islower(int c)
{
	if (c >= 'a' && c <= 'z')
		return 1;

	return 0;
}

/* @func: conch_isprint
 * #desc:
 *    check if it is a printable character.
 *
 * #1: c [in]  character
 * #r:   [ret] 1: true, 0: false
 */
int conch_isprint(int c)
{
	if (c > 0x1f && c < 0x7f)
		return 1;

	return 0;
}

/* @func: conch_ispunct
 * #desc:
 *    check if it is a punctuation mark.
 *
 * #1: c [in]  character
 * #r:   [ret] 1: true, 0: false
 */
int conch_ispunct(int c)
{
	if (c > 0x20 && c < 0x30)
		return 1;
	if (c > 0x39 && c < 0x41)
		return 1;
	if (c > 0x5a && c < 0x61)
		return 1;
	if (c > 0x7a && c < 0x7f)
		return 1;

	return 0;
}

/* @func: conch_isspace
 * #desc:
 *    check if it is a whitespace character.
 *
 * #1: c [in]  character
 * #r:   [ret] 1: true, 0: false
 */
int conch_isspace(int c)
{
	if (c == ' ' || c == '\t' || c == '\n'
			|| c == '\r' || c == '\f'
			|| c == '\v')
		return 1;

	return 0;
}

/* @func: conch_isupper
 * #desc:
 *    check if it is a uppercase letter.
 *
 * #1: c [in]  character
 * #r:   [ret] 1: true, 0: false
 */
int conch_isupper(int c)
{
	if (c >= 'A' && c <= 'Z')
		return 1;

	return 0;
}

/* @func: conch_isxdigit
 * #desc:
 *    check if it is a hexadecimal.
 *
 * #1: c [in]  character
 * #r:   [ret] 1: true, 0: false
 */
int conch_isxdigit(int c)
{
	if (c >= 'A' && c <= 'F')
		return 1;
	if (c >= 'a' && c <= 'f')
		return 1;
	if (c >= '0' && c <= '9')
		return 1;

	return 0;
}

/* @func: conch_tolower
 * #desc:
 *    convert uppercase letters to lowercase letters.
 *
 * #1: c [in]  character
 * #r:   [ret] result
 */
int conch_tolower(int c)
{
	if (c >= 'A' && c <= 'z')
		return c - 'A' + 'a';

	return c;
}

/* @func: conch_toupper
 * #desc:
 *    convert lowercase letters to uppercase letters.
 *
 * #1: c [in]  character
 * #r:   [ret] result
 */
int conch_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		return c - 'a' + 'A';

	return c;
}
