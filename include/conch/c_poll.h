/* @file: c_poll.h
 * #desc:
 *    The definitions of synchronous i/o multiplexing.
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

#ifndef _CONCH_C_POLL_H
#define _CONCH_C_POLL_H

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>
#include <conch/c_time.h>
#include <conch/c_signal.h>


#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

#define POLLIN     0x01 /* data other than high-priority data may be read without blocking */
#define POLLPRI    0x02 /* high priority data may be read without blocking */
#define POLLOUT    0x04 /* normal data may be written without blocking */
#define POLLERR    0x08 /* an error has occurred (revents only) */
#define POLLHUP    0x10 /* device has been disconnected (revents only) */
#define POLLNVAL   0x20 /* invalid fd member (revents only) */

#define POLLRDNORM 0x40  /* normal data may be read without blocking */
#define POLLRDBAND 0x80  /* priority data may be read without blocking */
#define POLLWRNORM 0x100 /* equivalent to POLLOUT */
#define POLLWRBAND 0x200 /* priority data may be written */

struct xpollfd {
	int32_t fd;
	int16_t events;
	int16_t revents;
};

# else
#  error "!!!unknown CONCH_PLATFORM!!!"
# endif
#else
# error "!!!undefined CONCH_PLATFORM!!!"
#endif


#ifdef __cplusplus
extern "C" {
#endif

/* c_poll.c */
extern
int32_t conch_ppoll(struct xpollfd *fds, int32_t nfds,
		const struct xtimespec *ts, const xsigset_t *mask)
;
extern
int32_t conch_poll(struct xpollfd *fds, int32_t nfds, int32_t sec)
;

#ifdef __cplusplus
}
#endif


#endif
