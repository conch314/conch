/* @file: c_termios.h
 * #desc:
 *    The definitions of terminal i/o interface.
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

#ifndef _CONCH_C_TERMIOS_H
#define _CONCH_C_TERMIOS_H

#include <conch/config.h>
#include <conch/c_stddef.h>
#include <conch/c_stdint.h>


#ifdef CONCH_PLATFORM
# if (CONCH_PLATFORM == CONCH_PLATFORM_LINUX)

#define X_NCCS 19

typedef unsigned int xtcflag_t;
typedef unsigned char xcc_t;
typedef unsigned int xspeed_t;

struct xtermios {
	xtcflag_t c_iflag;  /* input modes */
	xtcflag_t c_oflag;  /* output modes */
	xtcflag_t c_cflag;  /* control modes */
	xtcflag_t c_lflag;  /* local modes */
	xcc_t c_line;       /* line discipline */
	xcc_t c_cc[X_NCCS]; /* control character */
	xspeed_t c_ispeed;  /* input speed */
	xspeed_t c_ospeed;  /* output speed */
};

/* c_iflag */
#define X_IGNBRK 0x001 /* Ignore break condition */
#define X_BRKINT 0x002 /* Signal interrupt on break */
#define X_IGNPAR 0x004 /* Ignore characters with parity errors */
#define X_PARMRK 0x008 /* Mark parity and framing errors */
#define X_INPCK  0x010 /* Enable input parity check */
#define X_ISTRIP 0x020 /* Strip 8th bit off characters */
#define X_INLCR  0x040 /* Map NL to CR on input */
#define X_IGNCR  0x080 /* Ignore CR */
#define X_ICRNL  0x100 /* Map CR to NL on input */
#define X_IXANY  0x800 /* Any character will restart after stop */

/* c_oflag */
#define X_OPOST  0x01 /* Perform output processing */
#define X_OCRNL  0x08
#define X_ONOCR  0x10
#define X_ONLRET 0x20
#define X_OFILL  0x40
#define X_OFDEL  0x80

/* c_cflag bit meaning */
/* Common CBAUD rates */
#define X_B0     0x00 /* hang up */
#define X_B50    0x01
#define X_B75    0x02
#define X_B110   0x03
#define X_B134   0x04
#define X_B150   0x05
#define X_B200   0x06
#define X_B300   0x07
#define X_B600   0x08
#define X_B1200  0x09
#define X_B1800  0x0a
#define X_B2400  0x0b
#define X_B4800  0x0c
#define X_B9600  0x0d
#define X_B19200 0x0e
#define X_B38400 0x0f
#define X_EXTA   X_B19200
#define X_EXTB   X_B38400

#define X_ADDRB   0x20000000 /* address bit */
#define X_CMSPAR  0x40000000 /* mark or space (stick) parity */
#define X_CRTSCTS 0x80000000 /* flow control */

#define X_IBSHIFT 16 /* Shift from CBAUD to CIBAUD */

/* tcflow() ACTION argument and TCXONC use these */
#define TCOOFF 0 /* Suspend output */
#define TCOON  1 /* Restart suspended output */
#define TCIOFF 2 /* Send a STOP character */
#define TCION  3 /* Send a START character */

/* tcflush() QUEUE_SELECTOR argument and TCFLSH use these */
#define TCIFLUSH  0 /* Discard data received but not yet read */
#define TCOFLUSH  1 /* Discard data written but not yet sent */
#define TCIOFLUSH 2 /* Discard all pending data */

/* c_cc */
#define X_VINTR    0
#define X_VQUIT    1
#define X_VERASE   2
#define X_VKILL    3
#define X_VEOF     4
#define X_VTIME    5
#define X_VMIN     6
#define X_VSWTC    7
#define X_VSTART   8
#define X_VSTOP    9
#define X_VSUSP    10
#define X_VEOL     11
#define X_VREPRINT 12
#define X_VDISCARD 13
#define X_VWERASE  14
#define X_VLNEXT   15
#define X_VEOL2    16

/* c_iflag */
#define X_IUCLC   0x0200
#define X_IXON    0x0400
#define X_IXOFF   0x1000
#define X_IMAXBEL 0x2000
#define X_IUTF8   0x4000

/* c_oflag */
#define X_OLCUC  0x02
#define X_ONLCR  0x04
#define X_NLDLY  0x100
#define X_NL0    0x00
#define X_NL1    0x100
#define X_CRDLY  0x600
#define X_CR0    0x00
#define X_CR1    0x200
#define X_CR2    0x400
#define X_CR3    0x600
#define X_TABDLY 0x1800
#define X_TAB0   0x00
#define X_TAB1   0x800
#define X_TAB2   0x1000
#define X_TAB3   0x1800
#define X_XTABS  0x1800
#define X_BSDLY  0x2000
#define X_BS0    0x00
#define X_BS1    0x2000
#define X_VTDLY  0x4000
#define X_VT0    0x00
#define X_VT1    0x4000
#define X_FFDLY  0x8000
#define X_FF0    0x00
#define X_FF1    0x8000

/* c_cflag bit meaning */
#define X_CBAUD    0x100f
#define X_CSIZE    0x30
#define X_CS5      0x00
#define X_CS6      0x10
#define X_CS7      0x20
#define X_CS8      0x30
#define X_CSTOPB   0x40
#define X_CREAD    0x80
#define X_PARENB   0x100
#define X_PARODD   0x200
#define X_HUPCL    0x400
#define X_CLOCAL   0x800
#define X_CBAUDEX  0x1000
#define X_BOTHER   0x1000
#define X_B57600   0x1001
#define X_B115200  0x1002
#define X_B230400  0x1003
#define X_B460800  0x1004
#define X_B500000  0x1005
#define X_B576000  0x1006
#define X_B921600  0x1007
#define X_B1000000 0x1008
#define X_B1152000 0x1009
#define X_B1500000 0x100a
#define X_B2000000 0x100b
#define X_B2500000 0x100c
#define X_B3000000 0x100d
#define X_B3500000 0x100e
#define X_B4000000 0x100f
#define X_CIBAUD   0x100f0000 /* input baud rate */

/* c_lflag */
#define X_ISIG    0x01
#define X_ICANON  0x02
#define X_XCASE   0x04
#define X_ECHO    0x08
#define X_ECHOE   0x10
#define X_ECHOK   0x20
#define X_ECHONL  0x40
#define X_NOFLSH  0x80
#define X_TOSTOP  0x100
#define X_ECHOCTL 0x200
#define X_ECHOPRT 0x400
#define X_ECHOKE  0x800
#define X_FLUSHO  0x1000
#define X_PENDIN  0x4000
#define X_IEXTEN  0x8000
#define X_EXTPROC 0x10000

/* tcsetattr uses these */
#define X_TCSANOW   0
#define X_TCSADRAIN 1
#define X_TCSAFLUSH 2

/* modem lines */
#define X_TIOCM_LE   0x01
#define X_TIOCM_DTR  0x02
#define X_TIOCM_RTS  0x04
#define X_TIOCM_ST   0x08
#define X_TIOCM_SR   0x10
#define X_TIOCM_CTS  0x20
#define X_TIOCM_CAR  0x40
#define X_TIOCM_RNG  0x80
#define X_TIOCM_DSR  0x100
#define X_TIOCM_CD   X_TIOCM_CAR
#define X_TIOCM_RI   X_TIOCM_RNG
#define X_TIOCM_OUT1 0x2000
#define X_TIOCM_OUT2 0x4000
#define X_TIOCM_LOOP 0x8000

struct xwinsize {
	uint16_t ws_row;
	uint16_t ws_col;
	uint16_t ws_xpixel;
	uint16_t ws_ypixel;
};

# else
#  error "!!!unknown CONCH_PLATFORM!!!"
# endif
#else
# error "!!!undefined CONCH_PLATFORM!!!"
#endif



#endif
