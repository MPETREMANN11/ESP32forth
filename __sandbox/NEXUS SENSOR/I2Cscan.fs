\ *********************************************************************
\ Scan I2C bus to determine peripherials adresses
\    Filename:      I2Cscan.fs
\    Date:          08 feb 2022
\    Updated:       01 apr 2024
\    File Version:  1.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ *********************************************************************


RECORDFILE /spiffs/I2Cscan.fs

: .## ( n -- )
    <# # # #> type
  ;

\ not all bitpatterns are valid 7bit i2c addresses
: Wire.7bitaddr? ( a -- f )  
    dup $07 >=
    swap $77 <=  and
  ;

: Wire.detect   ( -- )
    [ wire ]
    base @ >r hex
    cr
    ."      00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f"
    $80 $00 do
        i $0f and 0= 
        if     
            cr i .##  ."  : "
        then
        i Wire.7bitaddr? if
            i  Wire.beginTransmission
            -1 Wire.endTransmission 0 =
            if
                i .## space
            else
                ." -- "
            then
        else
            space space
        then
    loop
    cr r> base !
    [ forth ]
  ;

<EOF>


\ Wire.detect \ example of display:
\      00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f 
\ 00 :                      -- -- -- -- -- -- -- -- -- 
\ 10 : -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
\ 20 : -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
\ 30 : -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
\ 40 : -- -- -- -- -- -- -- -- 48 -- -- -- -- -- -- -- 
\ 50 : -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
\ 60 : -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
\ 70 : -- -- -- -- -- -- -- --                         











