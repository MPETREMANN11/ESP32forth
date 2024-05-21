\ *********************************************************************
\ ESP NOW tests
\    Filename:      esp-now-test.fs
\    Date:          21 may 2024
\    Updated:       21 may 2024
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

\ change this parameters with your own wifi params
z" Mariloo"                     constant mySSID
z" 1925144D91DE5373C3C2D7959F"  constant myPASSWORD

wifi

\ Initialize WiFi
WIFI_MODE_STA Wifi.mode
mySSID myPASSWORD WiFi.begin 

create mac 6 allot          \ store local MAC address
mac WiFi WiFi.macAddress

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


espnow

esp_now_init        \ 0 for success

\ esp_now_deinit

variable version    \ store ESP NOW version

version esp_now_get_version drop
version @ .   \ display version

