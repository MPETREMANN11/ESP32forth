\ *********************************************************************
\ mac addresses 
\    Filename:      mac-address.fs
\    Date:          22 nov. 2025
\    Updated:       23 nov. 2025
\    File Version:  0.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/mac-address.fs

\ store current mac Address
create myMac 6 allot               

\ get  mac address for current ESP32 card and store result in myMac
: getMyMac ( -- )
    myMac WiFi.macAddress
  ;

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

: define-mac-address: ( $n1..$n6 comp: <name> -- | exec: -- addr )
    create
        \ store mac address in 6 first bytes
        >r >r >r >r >r
        c,  r> c,  r> c,  r> c,  r> c,  r> c,  r> c,  
        \ store 0 in others bytes
        esp_now_peer_info_t 6 - 1- for
            0 c,
        next
    does>
  ;

<EOF>



