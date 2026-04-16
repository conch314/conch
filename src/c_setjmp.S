/* @file: c_setjmp.S
 * #desc:
 *    The implementations of stack environment.
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
	.global conch_setjmp
	.global conch_longjmp

/* @func: conch_setjmp
 * #desc:
 *    set a non-local goto.
 *
 * #1: (long*) [out] registers buffer
 * #r: (int)   [ret] returns the longjmp value
 */
conch_setjmp:
#ifdef CONCH_MARCH_TYPE
# if (CONCH_MARCH_TYPE == CONCH_MARCH_X86_32)

	movl 4(%esp), %eax
	movl %ebx, (%eax)
	movl %esi, 4(%eax)
	movl %edi, 8(%eax)
	movl %ebp, 12(%eax)
	leal 4(%esp), %ecx
	movl %ecx, 16(%eax)
	movl (%esp), %ecx
	movl %ecx, 20(%eax)
	xorl %eax, %eax
	ret

# elif (CONCH_MARCH_TYPE == CONCH_MARCH_X86_64)

	movq %rbx, (%rdi)
	movq %rbp, 8(%rdi)
	movq %r12, 16(%rdi)
	movq %r13, 24(%rdi)
	movq %r14, 32(%rdi)
	movq %r15, 40(%rdi)
	leaq 8(%rsp), %rdx
	movq %rdx, 48(%rdi)
	movq (%rsp), %rdx
	movq %rdx, 56(%rdi)
	xorl %eax, %eax
	ret

# elif (CONCH_MARCH_TYPE == CONCH_MARCH_ARM_32)

	stmia r0!, { r4-r11 }
	mov r2, sp
	mov r3, lr
	stmia r0!, { r2, r3 }
	mov r0, #0
	bx lr

# elif (CONCH_MARCH_TYPE == CONCH_MARCH_ARM_64)

	stp x19, x20, [x0, #0]
	stp x21, x22, [x0, #16]
	stp x23, x24, [x0, #32]
	stp x25, x26, [x0, #48]
	stp x27, x28, [x0, #64]
	stp x29, x30, [x0, #80]
	mov x2, sp
	str x2, [x0, #104]
	stp d8, d9, [x0, #112]
	stp d10, d11, [x0, #128]
	stp d12, d13, [x0, #144]
	stp d14, d15, [x0, #160]
	mov x0, #0
	ret

# elif (CONCH_MARCH_TYPE == CONCH_MARCH_RISCV_32)

	sw s0, 0(a0)
	sw s1, 4(a0)
	sw s2, 8(a0)
	sw s3, 12(a0)
	sw s4, 16(a0)
	sw s5, 20(a0)
	sw s6, 24(a0)
	sw s7, 28(a0)
	sw s8, 32(a0)
	sw s9, 36(a0)
	sw s10, 40(a0)
	sw s11, 44(a0)
	sw sp, 48(a0)
	sw ra, 52(a0)
	li a0, 0
	ret

# elif (CONCH_MARCH_TYPE == CONCH_MARCH_RISCV_64)

	sd s0, 0(a0)
	sd s1, 8(a0)
	sd s2, 16(a0)
	sd s3, 24(a0)
	sd s4, 32(a0)
	sd s5, 40(a0)
	sd s6, 48(a0)
	sd s7, 56(a0)
	sd s8, 64(a0)
	sd s9, 72(a0)
	sd s10, 80(a0)
	sd s11, 88(a0)
	sd sp, 96(a0)
	sd ra, 104(a0)
	li a0, 0
	ret

# else
#  error "!!!unknown CONCH_MARCH_TYPE!!!"
# endif
#else
# error "!!!undefined CONCH_MARCH_TYPE!!!"
#endif

/* @func: conch_longjmp
 * #desc:
 *    performing a non-local goto.
 *
 * #1: (long*) [in] registers buffer
 * #2: (int)   [in] returns the value of setjmp
 */
conch_longjmp:
#ifdef CONCH_MARCH_TYPE
# if (CONCH_MARCH_TYPE == CONCH_MARCH_X86_32)

	movl 4(%esp), %edx
	movl 8(%esp), %eax
	cmpl $1, %eax
	adcl $0, %al
	movl (%edx), %ebx
	movl 4(%edx), %esi
	movl 8(%edx), %edi
	movl 12(%edx), %ebp
	movl 16(%edx), %esp
	jmp *20(%edx)

# elif (CONCH_MARCH_TYPE == CONCH_MARCH_X86_64)

	xorq %rax, %rax
	cmpl $1, %esi
 	adcl %esi, %rax
	movq (%rdi), %rbx
	movq 8(%rdi), %rbp
	movq 16(%rdi), %r12
	movq 24(%rdi), %r13
	movq 32(%rdi), %r14
	movq 40(%rdi), %r15
	movq 48(%rdi), %rsp
	jmp *56(%rdi)

# elif (CONCH_MARCH_TYPE == CONCH_MARCH_ARM_32)

	ldmia r0!, { r4-r11 }
	ldmia r0!, { r2, r3 }
	mov sp, r2
	mov lr, r3
	mov r0, r1
	cmp r0, #0
	moveq r0, #1
	bx lr

# elif (CONCH_MARCH_TYPE == CONCH_MARCH_ARM_64)

	ldp x19, x20, [x0, #0]
	ldp x21, x22, [x0, #16]
	ldp x23, x24, [x0, #32]
	ldp x25, x26, [x0, #48]
	ldp x27, x28, [x0, #64]
	ldp x29, x30, [x0, #80]
	ldr x2, [x0, #104]
	mov sp, x2
	ldp d8, d9, [x0, #112]
	ldp d10, d11, [x0, #128]
	ldp d12, d13, [x0, #144]
	ldp d14, d15, [x0, #160]
	cmp w1, #0
	csinc w0, w1, wzr, ne
	br x30

# elif (CONCH_MARCH_TYPE == CONCH_MARCH_RISCV_32)

	lw s0, 0(a0)
	lw s1, 4(a0)
	lw s2, 8(a0)
	lw s3, 12(a0)
	lw s4, 16(a0)
	lw s5, 20(a0)
	lw s6, 24(a0)
	lw s7, 28(a0)
	lw s8, 32(a0)
	lw s9, 36(a0)
	lw s10, 40(a0)
	lw s11, 44(a0)
	lw sp, 48(a0)
	lw ra, 52(a0)
	seqz a0, a1
	add a0, a0, a1
	ret

# elif (CONCH_MARCH_TYPE == CONCH_MARCH_RISCV_64)

	ld s0, 0(a0)
	ld s1, 8(a0)
	ld s2, 16(a0)
	ld s3, 24(a0)
	ld s4, 32(a0)
	ld s5, 40(a0)
	ld s6, 48(a0)
	ld s7, 56(a0)
	ld s8, 64(a0)
	ld s9, 72(a0)
	ld s10, 80(a0)
	ld s11, 88(a0)
	ld sp, 96(a0)
	ld ra, 104(a0)
	seqz a0, a1
	add a0, a0, a1
	ret

# else
#  error "!!!unknown CONCH_MARCH_TYPE!!!"
# endif
#else
# error "!!!undefined CONCH_MARCH_TYPE!!!"
#endif
