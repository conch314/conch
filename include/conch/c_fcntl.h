/* @file: c_fcntl.h
 * #desc:
 *    The definitions of file control options.
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

#ifndef _CONCH_C_FCNTL_H
#define _CONCH_C_FCNTL_H

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>
#include <conch/c_sys_types.h>


#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

/* duplicate file descriptor */
#define X_F_DUPFD 0
/* get file descriptor flags */
#define X_F_GETFD 1
/* set file descriptor flags */
#define X_F_SETFD 2
/* get file status flags and file access modes */
#define X_F_GETFL 3
/* set file status flags */
#define X_F_SETFL 4

/* get record locking info */
#define X_F_GETLK 5
/* get record locking info */
#define X_F_SETLK 6
/* set record locking info(wait if blocked) */
#define X_F_SETLKW 7

/* set process or process group id to receive SIGIO/SIGURG signal */
#define X_F_SETOWN 8
/* get process or process group id to receive SIGIO/SIGURG signal */
#define X_F_GETOWN 9

/* duplicate file descriptor with the close-on-exec flag */
#define X_F_DUPFD_CLOEXEC (1024 + 6)

/* close file descriptors when executing the exec */
#define X_FD_CLOEXEC 1

struct xflock {
	short l_type;   /* type of lock F_*LCK */
	short l_whence; /* offset type (LSEEK_*) */
	xoff_t l_start; /* relative offset */
	xoff_t l_len;   /* size (if 0 then until EOF) */
	xpid_t l_pid;   /* process id of the process holding the lock */
};

/* unlock */
#define X_F_UNLCK 0
/* read lock */
#define X_F_RDLCK 1
/* write lock */
#define X_F_WRLCK 2

/* read only */
#define X_O_RDONLY   0x00
/* write only */
#define X_O_WRONLY   0x01
/* read and write */
#define X_O_RDWR     0x02
/* mask of read and write */
#define X_O_ACCMODE  0x03

#ifdef CONCH_MARCH_TYPE
# if (CONCH_MARCH_TYPE == CONCH_MARCH_ARM_32 \
	|| CONCH_MARCH_TYPE == CONCH_MARCH_ARM_64)

#define X_O_SEARCH    0x4000
#define X_O_NOFOLLOW  0x8000
#define X_O_DIRECT    0x10000
#define X_O_LARGEFILE 0x20000

# endif
#else
# error "!!!undefined CONCH_MARCH_TYPE!!!"
#endif

/* create file if it does not exist */
#define X_O_CREAT    0x40
/* exclusive flag */
#define X_O_EXCL     0x80

/* do not assign controlling terminal (SIGTTOU/SIGTTIN signal) */
#define X_O_NOCTTY   0x100
/* truncate flag */
#define X_O_TRUNC    0x200
/* set append mode */
#define X_O_APPEND   0x400

/* non-blocking mode */
#define X_O_NONBLOCK 0x800
/* synchronized writing */
#define X_O_DSYNC    0x1000

/* write according to synchronized I/O data integrity */
#define X_O_SYNC     (X_O_DSYNC | 0x100000)

/* direct disk access hint */
#ifndef X_O_DIRECT
# define X_O_DIRECT     0x4000
#endif
/* large file support (>4GiB) */
#ifndef X_O_LARGEFILE
# define X_O_LARGEFILE  0x8000
#endif

/* open directory for search only (*at() functions) */
#ifndef X_O_SEARCH
# define X_O_SEARCH   0x10000
#endif
/* do not follow symbolic links */
#ifndef X_O_NOFOLLOW
# define X_O_NOFOLLOW 0x20000
#endif

/* close file descriptors when executing the exec */
#define X_O_CLOEXEC  0x80000

#define X_AT_FDCWD -100

# else
#  error "!!!unknown CONCH_PLATFORM!!!"
# endif
#else
# error "!!!undefined CONCH_PLATFORM!!!"
#endif


#ifdef __cplusplus
extern "C" {
#endif

/* c_fcntl.c */
extern
int32_t conch_fcntl(int32_t fd, int32_t cmd, ...)
;
extern
int32_t conch_openat(int32_t fd, const char *path, int32_t flags, ...)
;
extern
int32_t conch_open(const char *path, int32_t flags, ...)
;
extern
int32_t conch_creat(const char *path, xmode_t mode)
;

#ifdef __cplusplus
}
#endif


#endif
