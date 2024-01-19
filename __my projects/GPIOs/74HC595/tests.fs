\ *********************************************************************
\ SPI & 74HC585 control
\    Filename:      tests.fs
\    Date:          14 jan 2024
\    Updated:       14 jan 2024
\    File Version:  0.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    GNU General Public License
\ *********************************************************************



RECORDFILE /spiffs/tests.fs

SPI.init

0 0 data!
0 1 data!
0 2 data!
DATAS.send



\ encode n in 7 segments display
: #data2!  (  n -- )
    0 { byte }
    dup 1 = if      \ display 1
        2 4 or   to byte
    then
    dup 2 = if      \ display 2
        1 2 or 64 or 9 or 16 or to byte
    then
    dup 4 = if      \ display 3
        1 2 or 4 or 8 or 64 or to byte
    then
    dup 8 = if      \ display 4
        2 4 or 32 or 64 or to byte
    then
    dup 16 = if     \ display 5
        1 4 or 8 or 32 or 64 or to byte
    then
    dup 32 = if     \ display 6
        1 4 or 8 or 16 or 32 or 64 or to byte
    then
    dup 64 = if     \ display 7
        1 2 or 4 or to byte
    then
    dup 128 = if    \ display 8
        1 2 or 4 or 8 or 16 or 32 or 64 or to byte
    then
    byte DATAS c!
    drop
  ;




: xs ( n -- )
    dup #data2!
    2 data!
    DATAS.send ;



: tt for
1 xs    500 ms
2 xs    500 ms
4 xs    500 ms
8 xs    500 ms
16 xs   500 ms
32 xs   500 ms
64 xs   500 ms
128 xs  500 ms
next ;

<EOF>

SPI

: DS ( -- )
    DATAS NB_SHIFT_REGS SPI.writeBytes
    nop
    1 SPI.setHwCs
    nop nop
    0 SPI.setHwCs
  ;




