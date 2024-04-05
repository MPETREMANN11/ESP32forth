\ *********************************************************************
\ BME688
\    Filename:      main.fs
\    Date:          05 apr. 2024
\    Updated:       05 apr. 2024
\    File Version:  0.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     ---
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/main.fs
DEFINED? --be25 [if] forget --be25  [then]
create --be25

s" /spiffs/be25.fs"           included
s" /spiffs/config.fs"           included

\ s" /spiffs/tests.fs"            included

<EOF>


