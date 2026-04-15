/* @file: c_unistd.h
 * #desc:
 *    The definitions of unix standard.
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

#ifndef _CONCH_C_UNISTD_H
#define _CONCH_C_UNISTD_H

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>
#include <conch/c_sys_types.h>


#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

#undef X_SEEK_SET
#undef X_SEEK_CUR
#undef X_SEEK_END
#define X_SEEK_SET 0 /* absolute */
#define X_SEEK_CUR 1 /* relative */
#define X_SEEK_END 2 /* tail start */

# else
#  error "!!!unknown CONCH_PLATFORM!!!"
# endif
#else
# error "!!!undefined CONCH_PLATFORM!!!"
#endif


#ifdef __cplusplus
extern "C" {
#endif

/* c_unistd_fork.c */
extern
xpid_t conch_fork(void)
;

/* c_unistd_fs.c */
extern
ssize_t conch_read(int32_t fd, void *buf, size_t len)
;
extern
ssize_t conch_write(int32_t fd, const void *buf, size_t len)
;
extern
xoff_t conch_lseek(int32_t fd, xoff_t off, int32_t whence)
;
extern
int32_t conch_close(int32_t fd)
;

/* c_unistd_pid.c */
extern
xpid_t conch_getpid(void)
;
extern
xpid_t conch_getppid(void)
;
extern
xpid_t conch_getpgid(xpid_t pid)
;
extern
int32_t conch_setpgid(xpid_t pid, xpid_t pg)
;
extern
xuid_t conch_getuid(void)
;
extern
xuid_t conch_geteuid(void)
;
extern
int32_t conch_setuid(xuid_t uid)
;
extern
int32_t conch_seteuid(xuid_t euid)
;
extern
xgid_t conch_getgid(void)
;
extern
xgid_t conch_getegid(void)
;
extern
int32_t conch_setgid(xgid_t gid)
;
extern
int32_t conch_setegid(xgid_t egid)
;
extern
int32_t conch_getgroups(int32_t size, xgid_t gids[])
;
extern
int32_t conch_setgroups(int32_t size, xgid_t gids[])
;

/* c_unistd_sleep.c */
extern
int32_t conch_sleep(uint32_t n)
;

#ifdef __cplusplus
}
#endif


#endif
