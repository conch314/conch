/* @file: sys/stat.h
 * #desc:
 *    The definitions of file stat structure.
 *
 * #copy:
 *    Copyright (C) 1970 Public Free Software.
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

#ifndef _CONCH_SYS_STAT_H
#define _CONCH_SYS_STAT_H

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>
#include <conch/c_sys_types.h>
#include <conch/c_time.h>


#if defined(CONCH_PLATFORM_LINUX)

#define X_S_IRWXU  0700 /* read, write, execute/search by owner */
#define X_S_IRUSR  0400 /* read permission, owner */
#define X_S_IWUSR  0200 /* write permission, owner */
#define X_S_IXUSR  0100 /* execute/search permission, owner */

#define X_S_IRWXG   070 /* read, write, execute/search by group */
#define X_S_IRGRP   040 /* read permission, group */
#define X_S_IWGRP   020 /* write permission, group */
#define X_S_IXGRP   010 /* execute/search permission, group */

#define X_S_IRWXO    07 /* read, write, execute/search by others */
#define X_S_IROTH    04 /* read permission, others */
#define X_S_IWOTH    02 /* write permission, others */
#define X_S_IXOTH    01 /* execute/search permission, others */

#define X_S_ISUID 04000 /* set-user-id on execution */
#define X_S_ISGID 02000 /* set-group-id on execution */
#define X_S_ISVTX 01000 /* on directories, restricted deletion flag */

#define X_S_IFMT   0170000 /* type mask */
#define X_S_IFSOCK 0140000 /* socket */
#define X_S_IFLNK  0120000 /* symbolic link */
#define X_S_IFREG  0100000 /* regular file */
#define X_S_IFDIR  0040000 /* directory */
#define X_S_IFIFO  0010000 /* fifo special */
#define X_S_IFCHR  0020000 /* character special */
#define X_S_IFBLK  0060000 /* block special */

#define X_S_ISREG(x) (((x) & X_S_IFMT) == X_S_IFREG)
#define X_S_ISDIR(x) (((x) & X_S_IFMT) == X_S_IFDIR)
#define X_S_ISLNK(x) (((x) & X_S_IFMT) == X_S_IFLNK)
#define X_S_ISCHR(x) (((x) & X_S_IFMT) == X_S_IFCHR)
#define X_S_ISBLK(x) (((x) & X_S_IFMT) == X_S_IFBLK)
#define X_S_ISFIFO(x) (((x) & X_S_IFMT) == X_S_IFIFO)
#define X_S_ISSOCK(x) (((x) & X_S_IFMT) == X_S_IFSOCK)

struct xstat {
#if (defined(CONCH_MARCH_X86_32) || defined(CONCH_MARCH_ARM_32))

	uint64_t st_dev;          /* device id */
	uint8_t __pad0[4];
	uint32_t __st_ino;        /* file serial number */
	uint32_t st_mode;         /* file mode */
	uint32_t st_nlink;        /* link count (referenced inode) */
	uint32_t st_uid;          /* user id of file */
	uint32_t st_gid;          /* group id of file */
	uint64_t st_rdev;         /* device id (character or block special) */
	uint8_t __pad3[4];
	uint64_t st_size;         /* size of file, in bytes */
	uint32_t st_blksize;      /* size of block */
	uint64_t st_blocks;       /* block count */
	struct xtimespec st_atim; /* access timestamp */
	struct xtimespec st_mtim; /* modification timestamp */
	struct xtimespec st_ctim; /* change timestamp */
	uint64_t st_ino;          /* file serial number */

#elif defined(CONCH_MARCH_X86_64)

	uint64_t st_dev;          /* device id */
	uint64_t st_ino;          /* file serial number */
	uint64_t st_nlink;        /* link count (referenced inode) */
	uint32_t st_mode;         /* file mode */
	uint32_t st_uid;          /* user id of file */
	uint32_t st_gid;          /* group id of file */
	uint32_t __pad0;
	uint64_t st_rdev;         /* device id (character or block special) */
	uint64_t st_size;         /* size of file, in bytes */
	uint64_t st_blksize;      /* size of block */
	uint64_t st_blocks;       /* block count */
	struct xtimespec st_atim; /* access timestamp */
	struct xtimespec st_mtim; /* modification timestamp */
	struct xtimespec st_ctim; /* change timestamp */
	uint64_t __unused[3];

#elif (defined(CONCH_MARCH_ARM_64) \
	|| defined(CONCH_MARCH_RISCV_32) || defined(CONCH_MARCH_RISCV_64))

	uint64_t st_dev;          /* device id (regular file) */
	uint64_t st_ino;          /* file serial number */
	uint32_t st_mode;         /* file mode */
	uint32_t st_nlink;        /* link count (referenced inode) */
	uint32_t st_uid;          /* user id of file */
	uint32_t st_gid;          /* group id of file */
	uint64_t st_rdev;         /* device id (character or block special) */
	uint64_t __pad1;
	uint64_t st_size;         /* size of file, in bytes */
	uint32_t st_blksize;      /* size of block */
	uint32_t __pad2;
	uint64_t st_blocks;       /* block count */
	struct xtimespec st_atim; /* access timestamp */
	struct xtimespec st_mtim; /* modification timestamp */
	struct xtimespec st_ctim; /* change timestamp */
	uint32_t __unused4;
	uint32_t __unused5;

#else
# error "!!!unknown architecture!!!"
#endif
};

#undef st_atime
#undef st_atimensec
#undef st_atime_nsec
#define st_atime st_atim.tv_sec
#define st_atimensec st_atim.tv_nsec
#define st_atime_nsec st_mtim.tv_nsec

#undef st_mtime
#undef st_mtimensec
#undef st_mtime_nsec
#define st_mtime st_mtim.tv_sec
#define st_mtimensec st_mtim.tv_nsec
#define st_mtime_nsec st_mtim.tv_nsec

#undef st_ctime
#undef st_ctimensec
#undef st_ctime_nsec
#define st_ctime st_ctim.tv_sec
#define st_ctimensec st_ctim.tv_nsec
#define st_ctime_nsec st_ctim.tv_nsec

#else
# error "!!!unknown platform!!!"
#endif


#ifdef __cplusplus
extern "C" {
#endif

/* c_sys_stat.c */
extern
xmode_t conch_umask(xmode_t mode)
;
extern
int32_t conch_fchmodat(int32_t fd, const char *path, xmode_t mode,
		int32_t flags)
;
extern
int32_t conch_chmod(const char *path, xmode_t mode)
;
extern
int32_t conch_fchmod(int32_t fd, xmode_t mode)
;
extern
int32_t conch_fstatat(int32_t fd, const char *path, struct xstat *st,
		int32_t flags)
;
extern
int32_t conch_stat(const char *path, struct xstat *st)
;
extern
int32_t conch_fstat(int32_t fd, struct xstat *st)
;
extern
int32_t conch_lstat(const char *path, struct xstat *st)
;
extern
int32_t conch_mkdirat(int32_t fd, const char *path, xmode_t mode)
;
extern
int32_t conch_mkdir(const char *path, xmode_t mode)
;
extern
int32_t conch_mknodat(int32_t fd, const char *path, xmode_t mode,
		xdev_t dev)
;
extern
int32_t conch_mknod(const char *path, xmode_t mode, xdev_t dev)
;
extern
int32_t conch_utimensat(int32_t fd, const char *path,
		const struct xtimespec ts[2], int32_t flags)
;
extern
int32_t conch_futimens(int32_t fd, const struct xtimespec ts[2])
;

#ifdef __cplusplus
}
#endif


#endif
