/* @file: c_stdarg.h
 * #desc:
 *    The definitions of variable argument list.
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

#ifndef _CONCH_C_STDARG_H
#define _CONCH_C_STDARG_H

#include <conch/config.h>


/* variable argument list */
#undef va_list
#ifdef CONCH_STDARG_TYPEDEF
 typedef __builtin_va_list va_list;
#else
# define va_list __builtin_va_list
#endif

#undef va_start
#undef va_copy
#undef va_arg
#undef va_end
#define va_start(ap, fmt) __builtin_va_start(ap, fmt)
#define va_copy(dest, src) __builtin_va_copy(dest, src)
#define va_arg(ap, type) __builtin_va_arg(ap, type)
#define va_end(ap) __builtin_va_end(ap)


#endif
