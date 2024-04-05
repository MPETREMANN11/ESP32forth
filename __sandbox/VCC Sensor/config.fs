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

\ set DA pin
DEFINED? DA_PIN [IF] 
\     21 to DA_PIN
[THEN]


<EOF>


