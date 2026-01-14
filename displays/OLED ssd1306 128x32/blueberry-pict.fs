\ *************************************
\ addtionnal definitions for oled vocabulary
\    Filename:      blueberry-pict.fs
\    Date:          13 jan 2026
\    Updated:       13 jan 2026
\    File Version:  1.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ **************************************


RECORDFILE /spiffs/blueberry-pict.fs
create BLUEBERRY_PICT
$FF c, $FF c, $FF c, $FF c, $F7 c, $FF c, $FF c, $FD c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $FF c, $FF c, $FF c, $F7 c, $FF c, $FF c, $FD c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $FF c, $FF c, $FF c, $F7 c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $FF c, $FF c, $FF c, $07 c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $FF c, $FF c, $03 c, $F7 c, $FF c, $FF c, $FE c, $07 c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $FF c, $F9 c, $FF c, $F7 c, $FF c, $FF c, $FE c, $FC c, $0F c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $FF c, $F7 c, $FF c, $F3 c, $FF c, $FF c, $FC c, $FF c, $FB c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $FF c, $EF c, $FF c, $FE c, $FF c, $FF c, $F0 c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $FF c, $EF c, $FF c, $FF c, $CF c, $FE c, $1C c, $FF c, $FE c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $FF c, $EF c, $FF c, $FF c, $FF c, $1F c, $FD c, $FF c, $FE c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $FF c, $F3 c, $FF c, $FF c, $FF c, $FF c, $F3 c, $FF c, $FD c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $FF c, $F9 c, $FF c, $FF c, $F7 c, $FF c, $1F c, $FF c, $F9 c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $FF c, $FE c, $7F c, $FF c, $FF c, $FF c, $FF c, $FF c, $F3 c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $FF c, $FF c, $1F c, $FF c, $FF c, $FF c, $FF c, $FF c, $F7 c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $FF c, $FF c, $C3 c, $FF c, $FF c, $FF c, $FF c, $FF c, $CF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $FF c, $FF c, $F8 c, $7F c, $FF c, $FF c, $FF c, $FE c, $3F c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $FF c, $FF c, $FF c, $07 c, $FF c, $FF c, $FF c, $F1 c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $FF c, $FF c, $FF c, $C0 c, $0F c, $FF c, $FE c, $1F c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $FF c, $FF c, $FF c, $CA c, $00 c, $00 c, $00 c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $FF c, $FF c, $FE c, $CA c, $00 c, $00 c, $00 c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $FF c, $FF c, $FF c, $02 c, $00 c, $00 c, $00 c, $BF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $FF c, $FF c, $FF c, $81 c, $01 c, $E0 c, $00 c, $7F c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $FF c, $FF c, $FF c, $C2 c, $7B c, $F7 c, $81 c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $FF c, $FF c, $FF c, $F4 c, $6C c, $4F c, $FB c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $FF c, $FF c, $FF c, $DC c, $FE c, $1E c, $DD c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $FF c, $FF c, $FF c, $3C c, $FF c, $3F c, $E2 c, $3F c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $FF c, $FF c, $E1 c, $65 c, $79 c, $71 c, $E0 c, $20 c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $FF c, $F8 c, $3F c, $05 c, $BF c, $BF c, $D0 c, $10 c, $03 c, $FF c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $FE c, $0F c, $E0 c, $05 c, $BF c, $F7 c, $B0 c, $00 c, $78 c, $1F c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $E3 c, $FF c, $80 c, $05 c, $DE c, $0B c, $30 c, $06 c, $1F c, $C3 c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FF c, $BF c, $FF c, $04 c, $05 c, $CF c, $FE c, $70 c, $00 c, $8F c, $F8 c, $FF c, $FF c, $FF c, $FF c, $FF c,
$FE c, $7F c, $FF c, $C0 c, $05 c, $E3 c, $FD c, $78 c, $00 c, $1F c, $FF c, $7F c, $FF c, $FF c, $FF c, $FF c,

<EOF>


