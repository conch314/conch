/* @file: c_thread_tls.S
 * #desc:
 *    The implementations of thread-local storage.
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
	.global conch_thread_tls_get
	.global conch_thread_tls_set

/* @func: conch_thread_tls_get
 * #desc:
 *    get the thread-local storage pointer.
 *
 * #r: (void*) [ret] thread pointer
 */
conch_thread_tls_get:
#ifdef CONCH_MARCH_TYPE
# if (CONCH_MARCH_TYPE == CONCH_MARCH_X86_32)

	movl %gs:0, %eax
	ret

# elif (CONCH_MARCH_TYPE == CONCH_MARCH_X86_64)

	movq %fs:0, %rax
	ret

# elif (CONCH_MARCH_TYPE == CONCH_MARCH_ARM_32)

	mrc p15, 0, r0, c13, c0, 3
	bx lr

# elif (CONCH_MARCH_TYPE == CONCH_MARCH_ARM_64)

	mrs x0, tpidr_el0
	ret

# elif (CONCH_MARCH_TYPE == CONCH_MARCH_RISCV_32 \
	|| CONCH_MARCH_TYPE == CONCH_MARCH_RISCV_64)

	mv a0, tp
	ret

# else
#  error "!!!unknown CONCH_MARCH_TYPE!!!"
# endif
#else
# error "!!!undefined CONCH_MARCH_TYPE!!!"
#endif

/* @func: conch_thread_tls_set
 * #desc:
 *    set the thread-local storage pointer.
 *
 * #1: (void*) [in] thread pointer
 */
conch_thread_tls_set:
#ifdef CONCH_MARCH_TYPE
# if (CONCH_MARCH_TYPE == CONCH_MARCH_X86_32)
#  warning "!!!please use clone() to set up tls!!!"

	xorl %eax, %eax
	ret

# elif (CONCH_MARCH_TYPE == CONCH_MARCH_X86_64)
#  warning "!!!please use clone() to set up tls!!!"

	xorq %rax, %rax
	ret

# elif (CONCH_MARCH_TYPE == CONCH_MARCH_ARM_32)
#  warning "!!!please use clone() to set up tls!!!"

	// mcr p15, 0, r0, c13, c0, 3
	mov r0, #0
	bx lr

# elif (CONCH_MARCH_TYPE == CONCH_MARCH_ARM_64)

	msr tpidr_el0, x0
	ret

# elif (CONCH_MARCH_TYPE == CONCH_MARCH_RISCV_32 \
	|| CONCH_MARCH_TYPE == CONCH_MARCH_RISCV_64)

	mv tp, a0
	ret

# else
#  error "!!!unknown CONCH_MARCH_TYPE!!!"
# endif
#else
# error "!!!undefined CONCH_MARCH_TYPE!!!"
#endif
