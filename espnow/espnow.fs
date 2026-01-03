\ *********************************************************************
\ espnow development
\    Filename:      espnow.fs
\    Date:          22 nov. 2025
\    Updated:       28 dec. 2025
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
\ ex: SLAVE1 espnowAddPeer
: espnowAddPeer ( peer-addr -- )
    peer-exist?
    esp_now_add_peer ESP_OK <>   \ 0 for success
    if  cr ." ESP-NOW add_peer failed" 
        -1 throw
    then ;

\ send datas to peer
: espnowSend ( peerAddr myData len -- )
    esp_now_send  ?dup ESP_OK <>     \ 0 for success
    if  cr ." ESP-NOW esp_now_send failed" 
        ." error: " .
        -1 throw
    then ;

\ Register receive callback
: espnowRegisterRecv ( callback -- )
    esp_now_register_recv_cb ESP_OK <>     \ 0 for success
    if  cr ." ESP-NOW esp_now_register_recv_cb failed" 
        -1 throw
    then ;

\ Register send callback
\ non functionnal at this time
\ : espnowRegisterSend ( callback -- )
\     esp_now_register_send_cb ESP_OK <>     \ 0 for success
\     if  ." ESP-NOW esp_now_register_send_cb failed" cr 
\         -1 throw
\     then ;

only FORTH

<EOF>




