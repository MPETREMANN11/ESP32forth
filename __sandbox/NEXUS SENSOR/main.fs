\ *********************************************************************
\ BME688
\    Filename:      main.fs
\    Date:          31 mars 2024
\    Updated:       01 apr 2024
\    File Version:  0.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     ---
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/main.fs
DEFINED? --bme688 [if] forget --bme688  [then]
create --bme688

s" /spiffs/extendForthVoc.fs"   included

s" /spiffs/bme688.fs"           included
s" /spiffs/config.fs"           included

s" /spiffs/tests.fs"            included

<EOF>


