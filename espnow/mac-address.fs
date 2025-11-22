\ *********************************************************************
\ mac addresses 
\    Filename:      mac-address.fs
\    Date:          22 nov. 2025
\    Updated:       22 nov. 2025
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

\ get  mac address for current ESP32 card
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


<EOF>





