\ *********************************************************************
\ ESP NOW tests
\    Filename:      esp-now-test.fs
\    Date:          21 may 2024
\    Updated:       23 may 2024
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

create mac 6 allot          \ store local MAC address

\ display MAC address in hex format
: .mac ( -- addr len )
    base @ hex
    6 0 do
        mac i + c@ <# # # #> type
        i 5 < if
            [char] : emit
        then
    loop
    base !
  ;

\ change this parameters with your own wifi params
\ z" Mariloo"                     constant mySSID
\ z" 1925144D91DE5373C3C2D7959F"  constant myPASSWORD

z" ESP-MASTER-AP"           constant mySSID
z" "                        constant myPASSWORD

\ Initialize WiFi
wifi

\ Configure WiFi in station mode
: wifi-init ( -- ) 
    WIFI_MODE_STA Wifi.mode 
    mySSID myPASSWORD WiFi.softAP 
  ;

wifi-init

mac WiFi WiFi.macAddress
.mac



espnow

\ Initit ESP-NOW
: esp-now-init ( -- )
    esp_now_init    \ 0 for success
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

