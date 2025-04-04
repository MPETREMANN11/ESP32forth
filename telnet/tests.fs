\ *********************************************************************
\ ESP32 telnet tests
\    Filename:      tests.fs
\    Date:          04 april 2025
\    Updated:       04 april 2025
\    File Version:  0.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    GNU General Public License
\ *********************************************************************


RECORDFILE /spiffs/tests.fs

myWiFiConnect

myMDNS z>s type  

telnetd             \ select telnetd vocabulary
telnetPORT server   \ open TELNET server
forth               \ select forth vocabulary

<EOF>
