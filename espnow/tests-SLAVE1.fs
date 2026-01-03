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

also espnow

\ define two strings
ESP_NOW_ETH_ALEN     string RecvMacAddr
ESP_NOW_MAX_DATA_LEN string RecvDataStr


\ interpret ESP-NOW transmitted string
: store-cb-espnow  { len dataAddr macAddr -- }
    macAddr ESP_NOW_ETH_ALEN RecvMacAddr  $!
    dataAddr len RecvDataStr $!
  ;

only FORTH

\ register this callback
' store-cb-espnow espnowRegisterRecv

: espnow-loop ( -- )
    begin
        RecvDataStr nip ?dup if
            RecvDataStr evaluate
            RecvDataStr 0$!
        then        
    again
  ;

espnow-loop

<EOF>

: mon-cb-espnow { len dataAddr macAddr -- }
    cr ." --- ESP-NOW RECU ---"
    cr ." Longueur   : "  len .
    cr ." Datas      : "  dataAddr .
    cr ." Source MAC : "  macAddr .mac 
    cr ." -------------------"
    cr ." Msg : " dataAddr len type
  ;



