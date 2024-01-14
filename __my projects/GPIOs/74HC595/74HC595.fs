\ *********************************************************************
\ SPI & 74HC585 control
\    Filename:      74HC595.fs
\    Date:          14 jan 2024
\    Updated:       14 jan 2024
\    File Version:  0.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/74HC595.fs

DEFINED? NB_SHIFT_REGS 0= [IF]
3 constant NB_SHIFT_REGS
[THEN]

create DATAS
    NB_SHIFT_REGS  allot

\ store byte at pos in DATAS, pos [0..NB_SHIFT_REGS-1]
: data!  ( byte pos -- )
    DATAS + c!
  ;

: DATAS.send ( -- )
    [ SPI ]
    NB_SHIFT_REGS 0 do
        DATAS NB_SHIFT_REGS 1- i - +
        c@ SPI.write
    loop
    1 SPI.setHwCs
    1 ms
    0 SPI.setHwCs
    [ forth ]
  ;

<EOF>
