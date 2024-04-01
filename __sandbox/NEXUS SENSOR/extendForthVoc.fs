\ *********************************************************************
\ extensions for FORTH vocabulary - for compatibility
\    Filename:      extendForthVoc.fs
\    Date:          01 apr 2024
\    Updated:       01 apr 2024
\    File Version:  1.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ *********************************************************************



RECORDFILE /spiffs/extendForthVoc.fs

DEFINED? spaces 0= [IF]
: spaces ( n -- )   
    for
        aft
            space
        then
    next
  ;
[THEN]

<EOF>






