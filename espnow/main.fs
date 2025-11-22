\ *********************************************************************
\ main for espnow development
\    Filename:      main.fs
\    Date:          22 nov. 2025
\    Updated:       22 nov. 2025
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

s" /spiffs/espnow.fs"   included

s" /spiffs/tests.fs"    included

<EOF>



