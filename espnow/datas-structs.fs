\ *********************************************************************
\ datas and structures for espnow development
\    Filename:      datas-structs.fs
\    Date:          23 nov. 2025
\    Updated:       23 nov. 2025
\    File Version:  0.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/datas-structs.fs

decimal
struct esp_now_peer_info_t
     6 field ->peer_addr
    16 field ->lmk
    u8 field ->channel
    u8 field ->ifidx
    u8 field ->encrypt
   u32 field ->priv


<EOF>

decimal
structures
struct esp_now_peer_info_t
     6 field ->peer_addr
    16 field ->lmk
    u8 field ->channel
    u8 field ->ifidx
    u8 field ->encrypt
   u32 field ->priv
forth
esp_now_peer_info_t

