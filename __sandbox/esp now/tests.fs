\ *********************************************************************
\ ESP32 espnow tests
\    Filename:      tests.fs
\    Date:          06 june 2024
\    Updated:       06 june 2024
\    File Version:  0.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

\ for tests:
\  1) copy espnow.userwords.h in same directory as ESP32forth.ino
\  2) rename espnow.userwords.h to userwords.h
\  3) recompile and upload ESP32forth.ino
\
\ verify espnow vocabulary:
\  espnow vlist

RECORDFILE /spiffs/tests.fs

create myMAC 6 allot
WiFi
myMAC WiFi.macAddress
hex
myMAC 6 dump
decimal



<EOF>
