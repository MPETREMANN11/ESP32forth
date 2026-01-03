\ *********************************************************************
\ main for espnow development
\    Filename:      main.fs
\    Date:          22 nov. 2025
\    Updated:       21 dec. 2025
\    File Version:  0.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/main.fs

internals 120 to line-width forth

DEFINED? --espnow [if] forget --espnow  [then]
create --espnow

needs /spiffs/oled128x32-display.fs
needs /spiffs/datas-structs.fs
needs /spiffs/mac-address.fs
needs /spiffs/espnow.fs
needs /spiffs/config.fs
myMac 6 MASTER 6 str= [IF]
    needs /spiffs/tests-MASTER.fs
[THEN]

myMac 6 SLAVE1 6 str= [IF]
    needs /spiffs/strings.fs
    needs /spiffs/ledsCommand.fs
        initLeds  ledTest
    needs /spiffs/tests-SLAVE1.fs
[THEN]

myMac 6 SLAVE2 6 str= [IF]
    needs /spiffs/tests-SLAVE2.fs
[THEN]


<EOF>


