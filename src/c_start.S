/* @file: c_start.S
 * #desc:
 *    The implementations of userspace program entry.
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


	.text
	.globl _start
	.globl _start_main

/* @func: _start
 * #desc:
 *    userspace program entry.
 */
_start:
#ifdef CONCH_MARCH_TYPE
# if (CONCH_MARCH_TYPE == CONCH_MARCH_X86_32)

	xorl %ebp, %ebp
	movl %esp, %edi
	jmp _start_main
	ret

# elif (CONCH_MARCH_TYPE == CONCH_MARCH_X86_64)

	xorq %rbp, %rbp
	movq %rsp, %rdi
	jmp _start_main
	ret

# elif (CONCH_MARCH_TYPE == CONCH_MARCH_ARM_32)

	mov r0, sp
	mov fp, #0
	mov lr, #0
	b _start_main
	bx lr

# elif (CONCH_MARCH_TYPE == CONCH_MARCH_ARM_64)

	mov x0, sp
	mov x29, #0
	mov x30, #0
	b _start_main
	ret

# elif (CONCH_MARCH_TYPE == CONCH_MARCH_RISCV_32 \
	|| CONCH_MARCH_TYPE == CONCH_MARCH_RISCV_64)

	mv a0, sp
	mv s0, zero
	tail _start_main

# else
#  error "!!!unknown CONCH_MARCH_TYPE!!!"
# endif
#else
# error "!!!undefined CONCH_MARCH_TYPE!!!"
#endif
