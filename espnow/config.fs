\ *********************************************************************
\ configuration for espnow development
\    Filename:      config.fs
\    Date:          17 dec. 2025
\    Updated:       21 dec. 2025
\    File Version:  0.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/config.fs

$EC $62 $60 $9C $76 $30 define-mac-address: MASTER
$EC $62 $60 $9C $A9 $50 define-mac-address: SLAVE1

wifi-init 100 ms
getMyMac

espnowInit

myMac 6 MASTER 6 str= [IF]      \ peers for MASTER
    SLAVE1 espnowAddPeer
\     SLAVE2 espnowAddPeer
    s" MASTER" set-title
[THEN]

myMac 6 SLAVE1 6 str= [IF]      \ peers for SLAV1
    MASTER espnowAddPeer
\     SLAVE2 espnowAddPeer
    s" SLAVE1" set-title
[THEN]

\ myMac 6 SLAVE2 6 str= [IF]      \ peers for SLAV2
\     MASTER espnowAddPeer
\     SLAVE1 espnowAddPeer
\     s" SLAVE2" set-title
\ [THEN]

<EOF>


