\ *********************************************************************
\ MAIN for B25 VCC module and OLED management
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
DEFINED? --b25 [if] forget --b25  [then]
create --b25

s" /spiffs/oledSSD1306.fs"      included
s" /spiffs/b25.fs"              included
s" /spiffs/buzzer.fs"           included

s" /spiffs/config.fs"           included

s" /spiffs/I2Cscan.fs"          included
s" /spiffs/tests.fs"            included

<EOF>


