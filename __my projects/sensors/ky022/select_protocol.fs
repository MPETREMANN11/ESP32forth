\ *********************************************************************
\ Select a protocol
\    Filename:      ky022.fs
\    Date:          18 nov 2023
\    Updated:       18 nov 2023
\    File Version:  0.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    GNU General Public License
\ *********************************************************************


\ Specify which protocol(s) should be used for decoding.
\ If no protocol is defined, all protocols are active.

\ CREATE DECODE_DENON        \  Includes Sharp
\ CREATE DECODE_JVC
\ CREATE DECODE_KASEIKYO
\ CREATE DECODE_PANASONIC    \  the same as DECODE_KASEIKYO
CREATE DECODE_LG
\ CREATE DECODE_NEC          \  Includes Apple and Onkyo
\ CREATE DECODE_SAMSUNG
\ CREATE DECODE_SONY
\ CREATE DECODE_RC5
\ CREATE DECODE_RC6

\ CREATE DECODE_BOSEWAVE
\ CREATE DECODE_LEGO_PF
\ CREATE DECODE_MAGIQUEST
\ CREATE DECODE_WHYNTER

\ CREATE DECODE_DISTANCE     \  universal decoder for pulse width or pulse distance protocols
\ CREATE DECODE_HASH         \  special decoder for all protocols
