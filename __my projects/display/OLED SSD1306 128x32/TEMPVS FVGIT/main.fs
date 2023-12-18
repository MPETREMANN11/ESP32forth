\ *************************************
\ main file for tempusFugit
\    Filename:      main.fs
\    Date:          19 nov 2023
\    Updated:       19 nov 2023
\    File Version:  1.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ **************************************

RECORDFILE /spiffs/main.fs
DEFINED? --tempusFugit [if] forget --tempusFugit  [then]
create --tempusFugit

s" /spiffs/strings.fs"          included
s" /spiffs/RTClock.fs"          included

s" /spiffs/clepsydra.fs"        included

s" /spiffs/config.fs"           included
s" /spiffs/oledTools.fs"        included

oled
: start ( HH MM -- )
    0 RTC.setTime       \ define current time
    Oled128x32Init
    1 OledTextsize
    WHITE OledTextc
    begin
        OledCLS OledDisplay
        16 20 OledSetCursor
        RTC.getTime drop tempusTo$ s>z OledPrintln OledDisplay
        1000 ms
    key? until
  ;
forth
 
<EOF>

   
    