\ *********************************************************************
\ B25 Voltage Sensor
\    Filename:      b25.fs
\    Date:          05 apr. 2024
\    Updated:       05 apr. 2024
\    File Version:  0.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     ---
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/b25.fs


02 value B25_NUM_1
03 value B25_NUM_2

\ init GPIOs used by B25 modules
: B25.init ( -- )
    B25_NUM_1 INPUT pinmode
    B25_NUM_2 INPUT pinmode
  ;

<EOF>

