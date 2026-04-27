/* @file: c_ctype.h
 * #desc:
 *    The definitions of character types.
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

#ifndef _CONCH_C_CTYPE_H
#define _CONCH_C_CTYPE_H

#include <conch/config.h>


/*
 * ASCII:
 *  |0   |00 |NUL |NULL
 *  |1   |01 |SOH |START OF HEADING
 *  |2   |02 |STX |START OF TEXT
 *  |3   |03 |ETX |END OF TEXT
 *  |4   |04 |EOT |END OF TRANSMISSION
 *  |5   |05 |ENQ |ENQUIRY
 *  |6   |06 |ACK |ACKNOWLEDGE
 *  |7   |07 |BEL |BELL
 *  |8   |08 |BS  |BACKSPACE
 *  |9   |09 |HT  |HORIZONTAL TABULATION
 *  |10  |0a |NL  |LINE FEED / NEW LINE
 *  |11  |0b |VT  |VERTICAL TABULATION
 *  |12  |0c |FF  |FORM FEED
 *  |13  |0d |CR  |CARRIAGE RETURN
 *  |14  |0e |SO  |SHIFT OUT
 *  |15  |0f |SI  |SHIFT IN
 *  |16  |10 |DLE |DATA LINK ESCAPE
 *  |17  |11 |DC1 |DEVICE CONTROL ONE
 *  |18  |12 |DC2 |DEVICE CONTROL TWO
 *  |19  |13 |DC3 |DEVICE CONTROL THREE
 *  |20  |14 |DC4 |DEVICE CONTROL FOUR
 *  |21  |15 |NAK |NEGATIVE ACKNOWLEDGE
 *  |22  |16 |SYN |SYNCHRONOUS IDLE
 *  |23  |17 |ETB |END OF TRANSMISSION BLOCK
 *  |24  |18 |CAN |CANCEL
 *  |25  |19 |EM  |END OF MEDIUM
 *  |26  |1a |SUB |SUBSTITUTE
 *  |27  |1b |ESC |ESCAPE
 *  |28  |1c |FS  |FILE SEPARATOR
 *  |29  |1d |GS  |GROUP SEPARATOR
 *  |30  |1e |RS  |RECORD SEPARATOR
 *  |31  |1f |US  |UNIT SEPARATOR
 *  |32  |20 |' ' |SPACE
 *  |33  |21 |!   |EXCLAMATION MARK
 *  |34  |22 |"   |QUOTATION MARK
 *  |35  |23 |#   |NUMBER SIGN
 *  |36  |24 |$   |DOLLAR SIGN
 *  |37  |25 |%   |PERCENT SIGN
 *  |38  |26 |&   |AMPERSAND
 *  |39  |27 |'   |APOSTROPHE
 *  |40  |28 |(   |LEFT PARENTHESIS
 *  |41  |29 |)   |RIGHT PARENTHESIS
 *  |42  |2a |*   |ASTERISK
 *  |43  |2b |+   |PLUS SIGN
 *  |44  |2c |,   |COMMA
 *  |45  |2d |-   |HYPHEN-MINUS
 *  |46  |2e |.   |FULL STOP
 *  |47  |2f |/   |SOLIDUS
 *  |48  |30 |0   |DIGIT ZERO
 *  |49  |31 |1   |DIGIT ONE
 *  |50  |32 |2   |DIGIT TWO
 *  |51  |33 |3   |DIGIT THREE
 *  |52  |34 |4   |DIGIT FOUR
 *  |53  |35 |5   |DIGIT FIVE
 *  |54  |36 |6   |DIGIT SIX
 *  |55  |37 |7   |DIGIT SEVEN
 *  |56  |38 |8   |DIGIT EIGHT
 *  |57  |39 |9   |DIGIT NINE
 *  |58  |3a |:   |COLON
 *  |59  |3b |;   |SEMICOLON
 *  |60  |3c |<   |LESS-THAN SIGN
 *  |61  |3d |=   |EQUALS SIGN
 *  |62  |3e |>   |GREATER-THAN SIGN
 *  |63  |3f |?   |QUESTION MARK
 *  |64  |40 |@   |COMMERCIAL AT
 *  |65  |41 |A   |LATIN CAPITAL LETTER A
 *  |66  |42 |B   |LATIN CAPITAL LETTER B
 *  |67  |43 |C   |LATIN CAPITAL LETTER C
 *  |68  |44 |D   |LATIN CAPITAL LETTER D
 *  |69  |45 |E   |LATIN CAPITAL LETTER E
 *  |70  |46 |F   |LATIN CAPITAL LETTER F
 *  |71  |47 |G   |LATIN CAPITAL LETTER G
 *  |72  |48 |H   |LATIN CAPITAL LETTER H
 *  |73  |49 |I   |LATIN CAPITAL LETTER I
 *  |74  |4a |J   |LATIN CAPITAL LETTER J
 *  |75  |4b |K   |LATIN CAPITAL LETTER K
 *  |76  |4c |L   |LATIN CAPITAL LETTER L
 *  |77  |4d |M   |LATIN CAPITAL LETTER M
 *  |78  |4e |N   |LATIN CAPITAL LETTER N
 *  |79  |4f |O   |LATIN CAPITAL LETTER O
 *  |80  |50 |P   |LATIN CAPITAL LETTER P
 *  |81  |51 |Q   |LATIN CAPITAL LETTER Q
 *  |82  |52 |R   |LATIN CAPITAL LETTER R
 *  |83  |53 |S   |LATIN CAPITAL LETTER S
 *  |84  |54 |T   |LATIN CAPITAL LETTER T
 *  |85  |55 |U   |LATIN CAPITAL LETTER U
 *  |86  |56 |V   |LATIN CAPITAL LETTER V
 *  |87  |57 |W   |LATIN CAPITAL LETTER W
 *  |88  |58 |X   |LATIN CAPITAL LETTER X
 *  |89  |59 |Y   |LATIN CAPITAL LETTER Y
 *  |90  |5a |Z   |LATIN CAPITAL LETTER Z
 *  |91  |5b |[   |LEFT SQUARE BRACKET
 *  |92  |5c |\   |REVERSE SOLIDUS
 *  |93  |5d |]   |RIGHT SQUARE BRACKET
 *  |94  |5e |^   |CIRCUMFLEX ACCENT
 *  |95  |5f |_   |LOW LINE
 *  |96  |60 |`   |GRAVE ACCENT
 *  |97  |61 |a   |LATIN SMALL LETTER A
 *  |98  |62 |b   |LATIN SMALL LETTER B
 *  |99  |63 |c   |LATIN SMALL LETTER C
 *  |100 |64 |d   |LATIN SMALL LETTER D
 *  |101 |65 |e   |LATIN SMALL LETTER E
 *  |102 |66 |f   |LATIN SMALL LETTER F
 *  |103 |67 |g   |LATIN SMALL LETTER G
 *  |104 |68 |h   |LATIN SMALL LETTER H
 *  |105 |69 |i   |LATIN SMALL LETTER I
 *  |106 |6a |j   |LATIN SMALL LETTER J
 *  |107 |6b |k   |LATIN SMALL LETTER K
 *  |108 |6c |l   |LATIN SMALL LETTER L
 *  |109 |6d |m   |LATIN SMALL LETTER M
 *  |110 |6e |n   |LATIN SMALL LETTER N
 *  |111 |6f |o   |LATIN SMALL LETTER O
 *  |112 |70 |p   |LATIN SMALL LETTER P
 *  |113 |71 |q   |LATIN SMALL LETTER Q
 *  |114 |72 |r   |LATIN SMALL LETTER R
 *  |115 |73 |s   |LATIN SMALL LETTER S
 *  |116 |74 |t   |LATIN SMALL LETTER T
 *  |117 |75 |u   |LATIN SMALL LETTER U
 *  |118 |76 |v   |LATIN SMALL LETTER V
 *  |119 |77 |w   |LATIN SMALL LETTER W
 *  |120 |78 |x   |LATIN SMALL LETTER X
 *  |121 |79 |y   |LATIN SMALL LETTER Y
 *  |122 |7a |z   |LATIN SMALL LETTER Z
 *  |123 |7b |{   |LEFT CURLY BRACKET
 *  |124 |7c |'|' |VERTICAL LINE
 *  |125 |7d |}   |RIGHT CURLY BRACKET
 *  |126 |7e |~   |TILDE
 *  |127 |7f |DEL |DELETE
 */


#ifdef __cplusplus
extern "C" {
#endif

/* c_ctype.c */
extern
int conch_isalnum(int c)
;
extern
int conch_isalpha(int c)
;
extern
int conch_isblank(int c)
;
extern
int conch_iscntrl(int c)
;
extern
int conch_isdigit(int c)
;
extern
int conch_isgraph(int c)
;
extern
int conch_islower(int c)
;
extern
int conch_isprint(int c)
;
extern
int conch_ispunct(int c)
;
extern
int conch_isspace(int c)
;
extern
int conch_isupper(int c)
;
extern
int conch_isxdigit(int c)
;
extern
int conch_tolower(int c)
;
extern
int conch_toupper(int c)
;

#ifdef __cplusplus
}
#endif


#endif
