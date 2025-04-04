\ *********************************************************************
\ ESP32 telnet main program
\    Filename:      main.fs
\    Date:          04 april 2025
\    Updated:       04 april 2025
\    File Version:  0.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/main.fs
DEFINED? --telnet [if] forget --telnet  [then]
create --telnet

s" /spiffs/config.fs"   included

s" /spiffs/telnet.fs"   included

s" /spiffs/tests.fs"    included

<EOF>


