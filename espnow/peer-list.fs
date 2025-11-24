\ *********************************************************************
\ peer list for tests
\    Filename:      peer-list.fs
\    Date:          23 nov. 2025
\    Updated:       23 nov. 2025
\    File Version:  0.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/peer-list.fs

$08 $3A $F2 $AA $9B $B0 define-mac-address: MASTER
$08 $3A $F2 $6E $1E $F8 define-mac-address: SLAVE1


<EOF>



