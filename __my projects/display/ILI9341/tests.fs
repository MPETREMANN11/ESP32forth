\ *************************************
\ tests file for ILI9341
\    Filename:      tests.fs
\    Date:          03 jan 2024
\    Updated:       04 jan 2024
\    File Version:  1.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ **************************************



RECORDFILE /spiffs/tests.fs

s" /spiffs/assert.fs"     included

SPI.init
LCD.init

<EOF>




