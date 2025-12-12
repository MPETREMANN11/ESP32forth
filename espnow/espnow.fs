\ *********************************************************************
\ espnow development
\    Filename:      espnow.fs
\    Date:          22 nov. 2025
\    Updated:       12 dec. 2025
\    File Version:  0.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ *********************************************************************


RECORDFILE /spiffs/espnow.fs

also espnow

 0 constant ESP_OK
-1 constant ESP_FAIL

\ Initialize ESPNOW
: espnowInit ( -- )
    wifi-init
    esp_now_init ESP_OK <>   \ 0 for success
    if 
        ." ESP-NOW init failed" cr 
        -1 throw
    then
    ." ESP-NOW init success" cr
  ;

\ test if peer exist - stop execution if peer was previously added
: peer-exist? ( peer-addr -- peer-addr )
    dup esp_now_is_peer_exist  \ 0 = no peer added, 1 = peer added
    if  ." peer " .mac space ." previously added" cr
        -1 throw
    then ;

\ add peer to peer list
\ ex: SLAVE1 add-peer
: add-peer ( peer-addr -- )
    peer-exist?
    esp_now_add_peer ESP_OK <>   \ 0 for success
    if  ." ESP-NOW add_peer failed" cr 
        -1 throw
    then ;

\ send datas to peer
\ ex: SLAV1 s" TEST TRANSMISSION" espnowSend
: espnowSend ( peer-addr datas-addr len -- )
    esp_now_send ESP_OK <>   \ 0 for success
    if  ." ESP-NOW Send failed" cr 
        -1 throw
    then ;

only FORTH

<EOF>
