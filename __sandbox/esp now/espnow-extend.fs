\ *********************************************************************
\ ESP NOW extension - Complete espnow vocabulary
\    Filename:      espnow-extend.fs
\    Date:          06 jun 2024
\    Updated:       06 jun 2024
\    File Version:  1.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     ---
\    GNU General Public License
\ *********************************************************************


RECORDFILE /spiffs/espnow-extend.fs
only forth also
internals also
wifi also
espnow also definitions

\ constants defined for errors tests
0 constant ESP_OK



\ display MAC address in hex format
: .mac { mac-addr -- }
    base @ hex
    6 0 do
        mac-addr i + c@ <# # # #> type
        i 5 < if
            [char] : emit
        then
    loop
    base !
  ;

\ define name for MAC address
: define-mac-address: ( comp: <name> <mac-str> -- | exec: -- addr )
    create
        base @ >r  hex          \ save current base
        5 for
            [char] : parse      \ search : delimiter
            S>NUMBER?           \ try convert in integer
            if      c,          \ compile integer in mac-address
            else    abort" MAC address scan error"
            then
        next
        r> base !               \ restore current base
    does>
  ;

\ example of mac-address definition:
\ define-mac-address: MASTER_MAC EC:62:60:9C:76:30
\ MASTER_MAC .mac     \ display: EC:62:60:9C:76:30

\ Configure WiFi in station mode
: wifi-init ( -- ) 
    WIFI_MODE_STA Wifi.mode
  ;

\ Init ESP-NOW
: espnow-init ( -- )
    wifi-init
    esp_now_init ESP_OK <>   \ 0 for success
    if 
        ." ESP-NOW init failed" cr 
        exit
    then
    ." ESP-NOW init success" cr
  ;

forth definitions
only forth

<EOF>
