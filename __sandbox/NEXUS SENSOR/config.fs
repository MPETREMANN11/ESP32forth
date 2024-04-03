\ *********************************************************************
\ BME688
\    Filename:      config.fs
\    Date:          31 mars 2024
\    Updated:       02 apr. 2024
\    File Version:  0.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/config.fs

\ set SDA and SCL pins for BME688
DEFINED? NEXUS_SDA [IF] 
    21 to NEXUS_SDA
    22 to NEXUS_SCL
[THEN]

\ set I2S address for APDS9960
DEFINED? APDS9960_I2S_ADDRESS [IF]
    $39 to APDS9960_I2S_ADDRESS
[THEN]

\ set I2S address for BME688
DEFINED? BME688_I2S_ADDRESS [IF]
    $77 to BME688_I2S_ADDRESS
[THEN]

\ set I2S address for PCF8523
DEFINED? PCF8523_I2S_ADDRESS [IF]
    $68 to PCF8523_I2S_ADDRESS
[THEN]

<EOF>


