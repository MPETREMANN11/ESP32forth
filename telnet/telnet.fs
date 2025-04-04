\ *********************************************************************
\ TELNET .....
\    Filename:      telnet.fs
\    Date:          04 april 2025
\    Updated:       04 april 2025
\    File Version:  0.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/telnet.fs

z" Mariloo"                     constant mySSID
z" 1925144D91DE5373C3C2D7959F"  constant myPASSWORD

z" esp32card"                   constant myMDNS

\ connection to local WiFi LAN
: myWiFiConnect ( -- )
    mySSID myPASSWORD  login
    myMDNS MDNS.begin drop
    myMDNS MDNS.begin 
  ;



<EOF>


