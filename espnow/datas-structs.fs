\ *********************************************************************
\ datas and structures for espnow development
\    Filename:      datas-structs.fs
\    Date:          23 nov. 2025
\    Updated:       26 nov. 2025
\    File Version:  0.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

doc: https://docs.espressif.com/projects/esp-idf/en/v4.3/esp32/api-reference/network/esp_now.html

RECORDFILE /spiffs/datas-structs.fs
forth
also structures
also espnow
struct esp_now_peer_info_t
  ESP_NOW_ETH_ALEN field ->peer_addr
                16 field ->lmk
                u8 field ->channel
                u8 field ->ifidx
                u8 field ->encrypt
               u32 field ->priv

\ set channel for peer
: set-channel ( chan addr -- )
    !field ->channel 
  ;

\ set encryption flag: 0=no encrypt.|1=encrypt.
: set-encrypt ( encript-fl addr -- )
    !field ->encrypt 
  ;

struct struct_message
   240 field ->msg_payload


struct event_recv_cb
    ESP_NOW_ETH_ALEN field ->event_peer_addr
                 u32 field ->event_data
                 u32 field ->event_data_len



only forth

<EOF>

