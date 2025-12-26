\ *********************************************************************
\ tests for SLAVE2 espnow development
\    Filename:      tests-SLAVE2.fs
\    Date:          21 dec. 2025
\    Updated:       21 dec. 2025
\    File Version:  0.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/tests-SLAVE2.fs

myMac .mac cr
." - - - - " cr
." MASTER: " MASTER .mac cr
." SLAVE1: " SLAVE1 .mac cr
." SLAVE2: " SLAVE2 .mac cr
espnow

<EOF>







