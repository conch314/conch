/* @file: c_time.h
 * #desc:
 *    The definitions of time functions.
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

#ifndef _CONCH_C_TIME_H
#define _CONCH_C_TIME_H

#include <conch/config.h>
#include <conch/c_stdint.h>
#include <conch/c_sys_types.h>


#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

struct xtimespec {
	xtime_t tv_sec; /* seconds */
	long tv_nsec;  /* nanoseconds */
};

struct xtimeval {
	xtime_t tv_sec;       /* seconds */
	xsuseconds_t tv_usec; /* microseconds */
};

struct xitimerval {
	struct xtimeval it_interval; /* timer interval */
	struct xtimeval it_value;    /* first start time */
};

#define X_CLOCKS_PER_SEC 1000000

/* calendar/date time (system time) */
#define X_CLOCK_REALTIME 0
/* monotonic clock (not affected by system time) */
#define X_CLOCK_MONOTONIC 1
/* process cpu time */
#define X_CLOCK_PROCESS_CPUTIME_ID 2
/* thread cpu time */
#define X_CLOCK_THREAD_CPUTIME_ID 3

# else
#  error "!!!unknown CONCH_PLATFORM!!!"
# endif
#else
# error "!!!undefined CONCH_PLATFORM!!!"
#endif


#ifdef __cplusplus
extern "C" {
#endif

/* c_time_clock.c */
extern
int32_t conch_clock_gettime(int32_t cid, struct xtimespec *ts)
;
extern
int32_t conch_clock_settime(int32_t cid, const struct xtimespec *ts)
;
extern
xclock_t conch_clock(void)
;
extern
xtime_t conch_time(xtime_t *r)
;

/* c_time_sleep.c */
extern
int32_t conch_nanosleep(const struct xtimespec *req, struct xtimespec *rem)
;

#ifdef __cplusplus
}
#endif


#endif
