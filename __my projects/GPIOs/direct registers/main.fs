\ *********************************************************************
\ Direct registers access
\    Filename:      main.fs
\    Date:          11 nov 2023
\    Updated:       11 nov 2023
\    File Version:  0.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    GNU General Public License
\ *********************************************************************



RECORDFILE /spiffs/main.fs
DEFINED? --directRegs [if] forget --directRegs  [then]
create --directRegs

s" /spiffs/defRegsMasks.fs"     included   
s" /spiffs/config.fs"           included
s" /spiffs/trafficLight.fs"     included






<EOF>


