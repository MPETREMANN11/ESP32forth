\ *********************************************************************
\ SPI & 74HC585 control
\    Filename:      tests.fs
\    Date:          14 jan 2024
\    Updated:       14 jan 2024
\    File Version:  0.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    GNU General Public License
\ *********************************************************************



RECORDFILE /spiffs/tests.fs

SPI.init

255 0 data!
DATAS.send


<EOF>

