\ *********************************************************************
\ main for espnow development
\    Filename:      main.fs
\    Date:          22 nov. 2025
\    Updated:       23 nov. 2025
\    File Version:  0.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/main.fs
DEFINED? --espnow [if] forget --espnow  [then]
create --espnow

needs /spiffs/oled128x32-display.fs
needs /spiffs/datas-structs.fs
needs /spiffs/mac-address.fs
needs /spiffs/espnow.fs
needs /spiffs/peer-list.fs
needs /spiffs/tests.fs

<EOF>


