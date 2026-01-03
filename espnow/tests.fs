\ *********************************************************************
\ tests for espnow development
\    Filename:      tests.fs
\    Date:          22 nov. 2025
\    Updated:       23 nov. 2025
\    File Version:  0.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/tests.fs

myMac .mac cr
." - - - - " cr
." MASTER: " MASTER .mac cr
." SLAVE1: " SLAVE1 .mac cr

espnow
SLAVE1 add-peer

Oled128x32Init cr
oled.cls
z" my string"       oled.println
z" ET CECI AUSSI"   oled.println
z" PUIS UNE LIGNE PLUS"   oled.println
z" ET LA DERNIERE ligne"   oled.println


<EOF>






