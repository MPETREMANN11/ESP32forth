\ *********************************************************************
\ BME688
\    Filename:      config.fs
\    Date:          31 mars 2024
\    Updated:       31 mars 2024
\    File Version:  0.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/config.fs

\ set SDA and SCL pins for BME688
21 to BME688_SDA
22 to BME688_SCL

\ set I2S address for BME688
$77 to BME_I2S_ADDRESS

<EOF>


