\ *********************************************************************
\ SPI & 74HC585 control
\    Filename:      main.fs
\    Date:          14 jan 2024
\    Updated:       14 jan 2024
\    File Version:  0.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/main.fs
DEFINED? --74HC595 [if] forget --74HC595  [then]
create --74HC595

s" /spiffs/config.fs"           included
s" /spiffs/SPIinit.fs"          included
s" /spiffs/74HC595.fs"          included
s" /spiffs/tests.fs"            included

<EOF>

