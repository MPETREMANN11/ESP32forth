\ *********************************************************************
\ ESP32 espnow main program
\    Filename:      main.fs
\    Date:          06 june 2024
\    Updated:       06 june 2024
\    File Version:  0.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/main.fs
DEFINED? --espnow [if] forget --espnow  [then]
create --espnow

s" /spiffs/espnow-extend.fs"    included

s" /spiffs/tests.fs"            included




<EOF>


