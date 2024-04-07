\ *********************************************************************
\ B25 Buzzer
\    Filename:      buzzer.fs
\    Date:          07 apr. 2024
\    Updated:       07 apr. 2024
\    File Version:  0.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/buzzer.fs

\ values of BUZZER pins set in config.fs file
0 value BUZZER_PIN

\ set BUZZER_PIN in OUTPUT direction
: buzzer.init ( -- )
    BUZZER_PIN OUTPUT pinmode
  ;

<EOF>


