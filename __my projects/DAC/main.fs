\ *************************************
\ main file for NOTES test
\    Filename:      main.fs
\    Date:          08 nov 2023
\    Updated:       08 nov 2023
\    File Version:  1.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ **************************************

RECORDFILE /spiffs/main.fs
DEFINED? --tonesTests [if] forget --tonesTests  [then]
create --tonesTests

s" /spiffs/config.fs"           included
s" /spiffs/frequencyNotes.fs"   included
s" /spiffs/tones.fs"            included
s" /spiffs/tonesTests.fs"       included


flightBumbleBee


<EOF>

