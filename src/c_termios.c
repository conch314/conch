/* @file: c_termios.c
 * #desc:
 *    The implementations of terminal i/o interface.
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
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>




xspeed_t conch_cfgetospeed(const struct xtermios *tio)
{
	return tio->c_cflag & X_CBAUD;
}

int32_t conch_cfsetospeed(struct xtermios *tio, xspeed_t speed)
{
	if (speed & ~CBAUD) {
		errno = X_EINVAL;
		return -1;
	}
	tio->c_cflag &= ~X_CBAUD;
	tio->c_cflag |= speed;

	return 0;
}

xspeed_t conch_cfgetispeed(const struct xtermios *tio)
{
	return (tio->c_cflag & X_CIBAUD) / (X_CIBAUD / X_CBAUD);
}

int32_t conch_cfsetispeed(struct xtermios *tio, xspeed_t speed)
{
	if (speed & ~X_CBAUD) {
		errno = X_EINVAL;
		return -1;
	}

	tio->c_cflag &= ~X_CIBAUD;
	tio->c_cflag |= speed * (X_CIBAUD / X_CBAUD);

	return 0;
}

int32_t conch_cfsetspeed(struct xtermios *tio, xspeed_t speed)
{
	int32_t r = conch_cfsetospeed(tio, speed);
	if (!r)
		conch_cfsetispeed(tio, 0);

	return r;
}

int32_t conch_tcflow(int32_t fd, int32_t act)
{
	return conch_ioctl(fd, X_TCXONC, act);
}

/* @func: conch_tcflush
 * #desc:
 *    clear buffer function.
 *
 * #1: fd  [in]  file descriptor
 * #2: qu  [in]  queue selector
 * #r:     [ret] 0: no error, -1: errno
 */
int32_t conch_tcflush(int32_t fd, int32_t qu)
{
	return conch_ioctl(fd, X_TCFLSH, qu);
}

/* @func: conch_tcdrain
 * #desc:
 *    output buffer control.
 *
 * #1: fd  [in]  file descriptor
 * #r:     [ret] 0: no error, -1: errno
 */
int32_t conch_tcdrain(int32_t fd)
{
	return conch_ioctl(fd, X_TCSBRK, 1);
}

/* @func: conch_tcsendbreak
 * #desc:
 *    send break signal to serial port.
 *
 * #1: fd  [in]  file descriptor
 * #2: dur [out] duration
 * #r:     [ret] 0: no error, -1: errno
 */
int32_t conch_tcsendbreak(int32_t fd, int32_t dur)
{
	return conch_ioctl(fd, X_TCSBRK, dur);
}

/* @func: conch_tcgetattr
 * #desc:
 *    get the terminal attribute.
 *
 * #1: fd  [in]  file descriptor
 * #2: tio [out] termios struct
 * #r:     [ret] 0: no error, -1: errno
 */
int32_t conch_tcgetattr(int32_t fd, struct xtermios *tio)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	return conch_ioctl(fd, X_TCGETS, tio);

# else
#  error "!!!unknown CONCH_PLATFORM!!!"
# endif
#else
# error "!!!undefined CONCH_PLATFORM!!!"
#endif
}

/* @func: conch_tcsetattr
 * #desc:
 *    set the terminal attribute.
 *
 * #1: fd   [in] file descriptor
 * #2: act  [in] optional actions
 * #3: tiio [in] termios struct
 * #r:     [ret] 0: no error, -1: errno
 */
int32_t conch_tcsetattr(int32_t fd, int32_t act, const struct xtermios *tio)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	if (act < 0 || act > 2) {
		x_errno = X_EINVAL;
		return -1;
	}

	return conch_ioctl(fd, X_TCSETS + act, tio);

# else
#  error "!!!unknown CONCH_PLATFORM!!!"
# endif
#else
# error "!!!undefined CONCH_PLATFORM!!!"
#endif
}

/* @func: conch_tcgetwinsize
 * #desc:
 *    get the terminal window size.
 *
 * #1: fd  [in]  file descriptor
 * #2: wsz [out] window size
 * #r:     [ret] 0: no error, -1: errno
 */
int32_t conch_tcgetwinsize(int32_t fd, struct xwinsize *wsz)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	return conch_ioctl(fd, X_TIOCGWINSZ, wsz);

# else
#  error "!!!unknown CONCH_PLATFORM!!!"
# endif
#else
# error "!!!undefined CONCH_PLATFORM!!!"
#endif
}

/* @func: conch_tcsetwinsize
 * #desc:
 *    set the terminal window size.
 *
 * #1: fd  [in]  file descriptor
 * #2: wsz [in]  window size
 * #r:     [ret] 0: no error, -1: errno
 */
int32_t conch_tcsetwinsize(int32_t fd, const struct xwinsize *wsz)
{
#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

	return conch_ioctl(fd, X_TIOCSWINSZ, wsz);

# else
#  error "!!!unknown CONCH_PLATFORM!!!"
# endif
#else
# error "!!!undefined CONCH_PLATFORM!!!"
#endif
}
