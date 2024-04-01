\ *********************************************************************
\ BME688
\    Filename:      tests.fs
\    Date:          31 mars 2024
\    Updated:       31 mars 2024
\    File Version:  0.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     ---
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/tests.fs

bme688.init     \ initialize BME688

wire

: test ( -- )
    BME_I2S_ADDRESS -1 wire.requestFrom 0 > 
    if 
        wire.available 
        0 do 
            wire.Read .
        loop 
    then 
  ;

forth

<EOF>



s" /spiffs/I2Cscan.fs"   included
bme688.init




