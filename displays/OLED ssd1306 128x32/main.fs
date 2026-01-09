\ *************************************
\ main file for SSD1306 128x32 display
\    Filename:      main.fs
\    Date:          05 nov 2023
\    Updated:       07 jan 2026
\    File Version:  1.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ **************************************

RECORDFILE /spiffs/main.fs

internals 120 to line-width forth

DEFINED? --oledTest [if] forget --oledTest  [then]
create --oledTest

needs /spiffs/oled128x32-display.fs
needs /spiffs/oledTests.fs

<EOF>

