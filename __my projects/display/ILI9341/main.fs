\ *************************************
\ main file for ILI9341
\    Filename:      main.fs
\    Date:          03 jan 2024
\    Updated:       03 jan 2024
\    File Version:  1.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ **************************************



RECORDFILE /spiffs/main.fs

DEFINED? --ILI9341 [if] forget --ILI9341  [then]
create --ILI9341

s" /spiffs/config.fs"           included
s" /spiffs/ILI9341waveshare.fs" included
s" /spiffs/graphics.fs"         included
s" /spiffs/tests.fs"            included

<EOF>
