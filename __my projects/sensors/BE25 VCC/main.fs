\ *********************************************************************
\ MAIN for BE25 and OLED management
\    Filename:      main.fs
\    Date:          05 apr. 2024
\    Updated:       07 apr. 2024
\    File Version:  0.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     ---
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/main.fs
DEFINED? --be25 [if] forget --be25  [then]
create --be25

s" /spiffs/oledSSD1306.fs"      included
s" /spiffs/be25.fs"             included
s" /spiffs/config.fs"           included

s" /spiffs/I2Cscan.fs"          included
s" /spiffs/tests.fs"            included

<EOF>


