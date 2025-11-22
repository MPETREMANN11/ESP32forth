\ *********************************************************************
\ espnow development
\    Filename:      espnow.fs
\    Date:          22 nov. 2025
\    Updated:       22 nov. 2025
\    File Version:  0.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ *********************************************************************


RECORDFILE /spiffs/main.fs

 0 constant ESP_OK
-1 constant ESP_FAIL

also WiFi
also espnow

: espnowInit ( -- )
    \ start wifi
    WIFI_MODE_STA WiFi.mode
    \ initialize espnow and check result
    esp_now_init ESP_OK <> throw
;

<EOF>



: espnow_register_example
  \ register the mac 12:34:56:78:9a:bc as a peer
  $12 c, $34 c, $56 c, $78 c, $9a c, $bc c,
  here 6 -
  ESPNOW_add_peer ESP_OK <> throw
  -6 allot
;
: espnow_send_some
  \ NULL a.k.a. send to peerlist
  \ send 10 bytes of data space pointer
  0 here 10 - 10
  espnow_send ESP_OK <> throw
;
