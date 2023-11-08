\ *************************************
\ main file for SSD1306 128x32 display
\    Filename:      main.fs
\    Date:          05 nov 2023
\    Updated:       05 nov 2023
\    File Version:  1.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ **************************************

RECORDFILE /spiffs/main.fs
DEFINED? --oledTest [if] forget --oledTest  [then]
create --oledTest

s" /spiffs/config.fs"           included
s" /spiffs/extendOledVoc.fs"    included
s" /spiffs/oledTools.fs"        included

<EOF>

