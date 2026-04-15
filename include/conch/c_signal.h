/* @file: c_signal.h
 * #desc:
 *    The definitions of posix signals.
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

#ifndef _CONCH_C_SIGNAL_H
#define _CONCH_C_SIGNAL_H

#include <conch/config.h>
#include <conch/c_stdint.h>
#include <conch/c_sys_types.h>


#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

#define X_NSIG 64
#define X_SI_MAX_SIZE 128

#define X_SIGHUP     1 /* hang up */
#define X_SIGINT     2 /* interrupt */
#define X_SIGQUIT    3 /* quit */
#define X_SIGILL     4 /* illegal instruction */
#define X_SIGTRAP    5 /* trace/breakpoint trap */
#define X_SIGABRT    6 /* abort */
#define X_SIGBUS     7 /* bus/address error */
#define X_SIGFPE     8 /* floating-point exception */
#define X_SIGKILL    9 /* kill */
#define X_SIGUSR1   10 /* user-defined signal */
#define X_SIGSEGV   11 /* segmentation violation */
#define X_SIGUSR2   12 /* user-defined signal */
#define X_SIGPIPE   13 /* broken pipe */
#define X_SIGALRM   14 /* alarm clock */
#define X_SIGTERM   15 /* termination */
#define X_SIGSTKFLT 16 /* stack fault on coprocessor */
#define X_SIGCHLD   17 /* child status changed */
#define X_SIGCONT   18 /* continue */
#define X_SIGSTOP   19 /* stop */
#define X_SIGTSTP   20 /* terminal stop */
#define X_SIGTTIN   21 /* background read from tty */
#define X_SIGTTOU   22 /* background write to tty */
#define X_SIGURG    23 /* urgent data available */
#define X_SIGXCPU   24 /* cpu time limit exceeded */
#define X_SIGXFSZ   25 /* file size limit exceeded */
#define X_SIGVTALRM 26 /* virtual alarm clock */
#define X_SIGPROF   27 /* profiling timer expired */
#define X_SIGWINCH  28 /* window resize */
#define X_SIGPOLL   29 /* pollable event possible */
#define X_SIGPWR    30 /* power failure */
#define X_SIGSYS    31 /* bad system call */
#define X_SIGRTMIN  32
#define X_SIGRTMAX  X_NSIG

/* do not generate SIGCHLD */
#define X_SA_NOCLDSTOP 0x01
/* do not generate zombie processes */
#define X_SA_NOCLDWAIT 0x02
/* use sa_sigaction info */
#define X_SA_SIGINFO 0x04

/* execute on an alternate signal stack */
#define X_SA_ONSTACK 0x08000000
/* restart an interrupted system call (EINTR) */
#define X_SA_RESTART 0x10000000
/* set non-blocking signal wait (default wait) */
#define X_SA_NODEFER 0x40000000
/* one-time signal */
#define X_SA_RESETHAND 0x80000000
/* use custom sigreturn */
#define X_SA_RESTORER  0x04000000

/* NOTE: blocking signal queued only while callback is executing */

/* add blocking signals */
#define X_SIG_BLOCK 0
/* unblocking signals */
#define X_SIG_UNBLOCK 1
/* set new signal mask */
#define X_SIG_SETMASK 2

typedef struct {
	uint8_t _bits[X_NSIG / 8];
} xsigset_t;

union xsigval { /* signal message transmission */
	int32_t sival_int;
	void *sival_ptr;
};

struct xsiginfo_payload {
	int32_t signo;
	int32_t errno;
	int32_t code;
	union {
		struct {
			xpid_t pid;
			xuid_t uid;
		} kill;
		struct {
			int32_t timerid;
			int32_t overrun;
			union xsigval value;
		} timer;
		struct {
			xpid_t pid;
			xuid_t uid;
			union xsigval sigval;
		} rt;
		struct {
			xpid_t pid;
			xuid_t uid;
			int32_t status;
			xclock_t utime;
			xclock_t stime;
		} sigchld;
		struct {
			void *addr;
			union {
				int32_t trapno;
				short addr_lsb;
				struct {
					size_t _pad;
					void *lower;
					void *upper;
				} bnd;
				struct {
					size_t _pad;
					uint32_t pkey;
				} pkey;
				struct {
					size_t data;
					uint32_t type;
					uint32_t flags;
				} perf;
			} u;
		} sigfault;
		struct {
			long band;
			int32_t fd;
		} sigpoll;
		struct {
			void *call_addr;
			int32_t syscall;
			uint32_t arch;
		} sigsys;
	} u;
};

typedef struct {
	union {
		struct xsiginfo_payload payload;
		uint32_t _pad[X_SI_MAX_SIZE / sizeof(uint32_t)];
	} u;
} xsiginfo_t;

#define X_SI_USER     0    /* sent by kill */
#define X_SI_KERNEL   0x80 /* sent by kernel */
#define X_SI_QUEUE    -1   /* sent by sigqueue */
#define X_SI_TIMER    -2   /* sent by expiration timer */
#define X_SI_MESGQ    -3   /* sent by empty message queue */
#define X_SI_ASYNCIO  -4   /* sent by asynchronous i/o request */
#define X_SI_SIGIO    -5   /* sent by queued SIGIO */
#define X_SI_TKILL    -6   /* sent by tkill system call */
#define X_SI_DETHREAD -7   /* sent by execve() killing subsidiary threads */
#define X_SI_ASYNCNL  -60  /* sent by glibc async name lookup completion */

#define X_ILL_ILLOPC 1 /* illegal opcode */
#define X_ILL_ILLOPN 2 /* illegal operand */
#define X_ILL_ILLADR 3 /* illegal addressing mode */
#define X_ILL_ILLTRP 4 /* illegal trap */
#define X_ILL_PRVOPC 5 /* privileged opcode */
#define X_ILL_PRVREG 6 /* privileged register */
#define X_ILL_COPROC 7 /* coprocessor error */
#define X_ILL_BADSTK 8 /* internal stack error */

#define X_FPE_INTDIV 1 /* integer divide by zero */
#define X_FPE_INTOVF 2 /* integer overflow */
#define X_FPE_FLTDIV 3 /* floating-point divide by zero */
#define X_FPE_FLTOVF 4 /* floating-point overflow */
#define X_FPE_FLTUND 5 /* floating-point underflow */
#define X_FPE_FLTRES 6 /* floating-point inexact result */
#define X_FPE_FLTINV 7 /* invalid floating-point operation */
#define X_FPE_FLTSUB 8 /* subscript out of range */

#define X_SEGV_MAPERR 1 /* address not mapped to object */
#define X_SEGV_ACCERR 2 /* invalid permissions for mapped object */
#define X_SEGV_BNDERR 3 /* failed address bound checks */

#define X_BUS_ADRALN 1 /* invalid address alignment */
#define X_BUS_ADRERR 2 /* nonexistent physical address */
#define X_BUS_OBJERR 3 /* object-specific hardware error */

#define X_TRAP_BRKPT  1 /* process breakpoint */
#define X_TRAP_TRACE  2 /* process trace trap */
#define X_TRAP_BRANCH 3	/* process taken branch trap */
#define X_TRAP_HWBKPT 4 /* hardware breakpoint/watchpoint */
#define X_TRAP_UNK    5 /* undiagnosed trap */
#define X_TRAP_PERF   6 /* perf event with sigtrap=1 */

#define X_CLD_EXITED    1 /* child exited */
#define X_CLD_KILLED    2 /* child exited and did not create a core file */
#define X_CLD_DUMPED    3 /* child exited and created a core file */
#define X_CLD_TRAPPED   4 /* traced child has trapped */
#define X_CLD_STOPPED   5 /* child has stopped */
#define X_CLD_CONTINUED 6 /* stopped child has continued */

#define X_POLL_IN  1 /* data input available */
#define X_POLL_OUT 2 /* output buffers available */
#define X_POLL_MSG 3 /* input message available */
#define X_POLL_ERR 4 /* i/o error */
#define X_POLL_PRI 5 /* high priority input available */
#define X_POLL_HUP 6 /* device disconnected */

#define X_SYS_SECCOMP       1 /* seccomp triggered */
#define X_SYS_USER_DISPATCH 2 /* syscall user dispatch triggered */

#define xsi_signo      u.payload.signo
#define xsi_errno      u.payload.errno
#define xsi_code       u.payload.code
#define xsi_pid        u.payload.u.kill.pid
#define xsi_uid        u.payload.u.kill.uid
#define xsi_tid        u.payload.u.timer.tid
#define xsi_overrun    u.payload.u.timer.overrun
#define xsi_status     u.payload.u.sigchld.status
#define xsi_utime      u.payload.u.sigchld.utime
#define xsi_stime      u.payload.u.sigchld.stime
#define xsi_value      u.payload.u.rt.sigval
#define xsi_int        u.payload.u.rt.sigval.sival_int
#define xsi_ptr        u.payload.u.rt.sigval.sival_ptr
#define xsi_addr       u.payload.u.sigfault.addr
#define xsi_trapno     u.payload.u.sigfault.u.trapno
#define xsi_addr_lsb   u.payload.u.sigfault.u.addr_lsb
#define xsi_lower      u.payload.u.sigfault.u.bnd.lower
#define xsi_upper      u.payload.u.sigfault.u.bnd.upper
#define xsi_pkey       u.payload.u.sigfault.u.pkey.pkey
#define xsi_perf_data  u.payload.u.sigfault.u.perf.data
#define xsi_perf_type  u.payload.u.sigfault.u.perf.type
#define xsi_perf_flags u.payload.u.sigfault.u.perf.flags
#define xsi_band       u.payload.u.sigpoll.band
#define xsi_fd         u.payload.u.sigpoll.fd
#define xsi_call_addr  u.payload.u.sigsys.call_addr
#define xsi_syscall    u.payload.u.sigsys.syscall
#define xsi_arch       u.payload.u.sigsys.arch

struct xsigaction {
	union { /* callback function */
		void (*_sa_handler)(int32_t);
		void (*_sa_sigaction)(int32_t, xsiginfo_t *, void *);
	} u;
#ifdef CONCH_MARCH_TYPE
# if (CONCH_MARCH_TYPE == CONCH_MARCH_X86_32 \
	|| CONCH_MARCH_TYPE == CONCH_MARCH_ARM_32)

	xsigset_t sa_mask;
	int32_t sa_flags;
	void (*sa_restorer)(void);

# elif (CONCH_MARCH_TYPE == CONCH_MARCH_X86_64)

	int32_t sa_flags;
	void (*sa_restorer)(void);
	xsigset_t sa_mask;

# elif (CONCH_MARCH_TYPE == CONCH_MARCH_ARM_64 \
	|| CONCH_MARCH_TYPE == CONCH_MARCH_RISCV_32 \
	|| CONCH_MARCH_TYPE == CONCH_MARCH_RISCV_64)

	int32_t sa_flags;
	xsigset_t sa_mask;

# else
#  error "!!!unknown CONCH_MARCH_TYPE!!!"
# endif
#else
# error "!!!undefined CONCH_MARCH_TYPE!!!"
#endif
};

#define xsa_handler   u._sa_handler
#define xsa_sigaction u._sa_sigaction

#define X_SIG_DFL ((void (*)(int32_t))0)  /* default signal handling */
#define X_SIG_IGN ((void (*)(int32_t))1)  /* ignore signal */
#define X_SIG_ERR ((void (*)(int32_t))-1) /* error return */

# else
#  error "!!!unknown CONCH_PLATFORM!!!"
# endif
#else
# error "!!!undefined CONCH_PLATFORM!!!"
#endif


#ifdef __cplusplus
extern "C" {
#endif

/* c_signal.c */
extern
int32_t conch_kill(xpid_t pid, int32_t sig)
;
extern
int32_t conch_killpg(xpid_t pg, int32_t sig)
;
extern
int32_t conch_sigaction(int32_t sig, const struct xsigaction *act,
		struct xsigaction *old)
;
extern
int32_t conch_sigprocmask(int32_t how, const xsigset_t *set,
		xsigset_t *old)
;
extern
int32_t conch_sigpending(xsigset_t *set)
;
extern
int32_t conch_sigsuspend(const xsigset_t *set)
;
extern
int32_t conch_sigaddset(xsigset_t *set, int32_t sig)
;
extern
int32_t conch_sigdelset(xsigset_t *set, int32_t sig)
;
extern
int32_t conch_sigismember(const xsigset_t *set, int32_t sig)
;
extern
int32_t conch_sigemptyset(xsigset_t *set)
;
extern
int32_t conch_sigfillset(xsigset_t *set)
;

#ifdef __cplusplus
}
#endif


#endif
