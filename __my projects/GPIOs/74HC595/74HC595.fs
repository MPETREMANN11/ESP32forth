\ *********************************************************************
\ SPI & 74HC585 control
\    Filename:      74HC595.fs
\    Date:          14 jan 2024
\    Updated:       19 jan 2024
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

\ store byte for regn in DATAS, regn [0..NB_SHIFT_REGS-1]
: data!  ( byte regn -- )
    NB_SHIFT_REGS 1- swap -      \ calculate inverse offset
    DATAS + c!              
  ;

\ use for very short delay
: nop ;

SPI also

: DATAS.send ( -- )
    DATAS NB_SHIFT_REGS SPI.writeBytes
    nop
    1 SPI.setHwCs
    nop nop
    0 SPI.setHwCs
  ;

<EOF>
