\ *********************************************************************
\ BME688
\    Filename:      bme688.fs
\    Date:          31 mars 2024
\    Updated:       31 mars 2024
\    File Version:  0.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     ---
\    GNU General Public License
\ *********************************************************************


\ set SDA and SCL pins
0 value BME688_SDA
0 value BME688_SCL

\ ser I2S address for BME688
0 value BME_I2S_ADDRESS

: bme688.init ( -- )
    [ wire ]
    BME688_SDA BME688_SCL wire.begin
    [ forth ]
    0 > if
        abort" BME688 wire initialisation error"
    then
  ;


