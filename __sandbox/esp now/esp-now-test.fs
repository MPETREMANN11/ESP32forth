\ *********************************************************************
\ ESP NOW tests
\    Filename:      esp-now-test.fs
\    Date:          21 may 2024
\    Updated:       01 jun 2024
\    File Version:  1.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     ---
\    GNU General Public License
\ *********************************************************************

\ for tests:
\  1) copy espnow.userwords.h in same directory as ESP32forth.ino
\  2) rename espnow.userwords.h to userwords.h
\  3) recompile and upload ESP32forth.ino
\
\ verify espnow vocabulary:
\  espnow vlist

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

internals also
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
only forth

\ example of mac-address definition:
\ define-mac-address: yyy-mac EC:62:60:9C:76:30
\ yyy-mac .mac     \ display: EC:62:60:9C:76:30



\ change this parameters with your own wifi params
\ z" Mariloo"                     constant mySSID
\ z" 1925144D91DE5373C3C2D7959F"  constant myPASSWORD

\ z" ESP-MASTER-AP"           constant mySSID
\ z" "                        constant myPASSWORD

\ Initialize WiFi
wifi

\ Configure WiFi in station mode
: wifi-init ( -- ) 
    WIFI_MODE_STA Wifi.mode 
\     mySSID myPASSWORD WiFi.softAP 
\     0= if
\         ." Soft AP creation failed" cr
\     then
  ;

wifi-init

create myMAC 6 allot
WiFi
myMAC WiFi.macAddress
hex
myMAC 6 dump
decimal



espnow

0 constant ESP_OK

\ Initit ESP-NOW
: esp-now-init ( -- )
    esp_now_init ESP_OK <>   \ 0 for success
    if 
        ." ESP-NOW init failed" cr 
        exit
    then
    ." ESP-NOW init success" cr
  ;

esp-now-init

\ esp_now_deinit

variable version    \ store ESP NOW version

version esp_now_get_version drop
version @ .   \ display version

