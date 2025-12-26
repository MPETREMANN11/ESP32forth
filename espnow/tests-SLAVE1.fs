\ *********************************************************************
\ tests for SLAVE1 espnow development
\    Filename:      tests-SLAVE1.fs
\    Date:          21 dec. 2025
\    Updated:       23 dec. 2025
\    File Version:  0.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/tests-SLAVE1.fs

myMac .mac cr
." - - - - " cr
." MASTER: " MASTER .mac cr
." SLAVE1: " SLAVE1 .mac cr
." SLAVE2: " SLAVE2 .mac cr


\ variable ESPNOW_RECV_MAC
\ variable ESPNOW_RECV_DATA
\ variable ESPNOW_RECV_DATA_LEN

: mon-cb-espnow ( macAddr dataAddr len -- )
    to ESPNOW_RECV_MAC
    to ESPNOW_RECV_DATA
    to ESPNOW_RECV_DATA_LEN
    cr ." --- ESP-NOW RECU ---"
    cr ." Longueur   : "  ESPNOW_RECV_DATA_LEN .
    cr ." Datas      : "  ESPNOW_RECV_DATA .
    cr ." Source MAC : "  ESPNOW_RECV_MAC .mac 
    cr ." -------------------"
    cr ." Msg : " ESPNOW_RECV_DATA  ESPNOW_RECV_DATA_LEN type
  ;

\ Enregistrement du callback
' mon-cb-espnow espnowRegisterRecv

<EOF>








