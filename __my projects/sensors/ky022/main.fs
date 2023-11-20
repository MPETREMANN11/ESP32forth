\ *********************************************************************
\ DHT11, DHT22 1-wire Control
\    Filename:      main.fs
\    Date:          11 nov 2023
\    Updated:       11 nov 2023
\    File Version:  0.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     ohiyooo2 / Frank Lin
\    Original publ. https://ohiyooo2.pixnet.net/blog/post/406078286
\    Adaptation for The Great Book for ESP32forth: Marc PETREMANN
\    GNU General Public License
\ *********************************************************************



RECORDFILE /spiffs/main.fs
DEFINED? --KY022test [if] forget --KY022test  [then]
create --KY022test



\ Define a pin name 
: defPIN: ( GPIOx ---  |  --- GPIOx ) 
    value 
  ; 

s" /spiffs/dumpTool.fs"         included
s" /spiffs/config.fs"           included
s" /spiffs/esp_err.fs"          included
s" /spiffs/ky022.fs"            included


<EOF>



