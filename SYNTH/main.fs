\ *************************************
\ main for ESP32 Synth/
\    Filename:      main.fs
\    Date:          25 jan 2026
\    Updated:       26 jan 2026
\    File Version:  1.0
\    MCU:           ESP32-S3 - ESP32 WROOM
\    Forth:         ESP32forth all versions 7.0.7.21+
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ **************************************


RECORDFILE /spiffs/main.fs

internals 160 to line-width forth

DEFINED? --espsynth [if] forget --espsynth  [then]
create --espsynth

needs /spiffs/datasAndStructs.fs
needs /spiffs/defNotes.fs
needs /spiffs/assert.fs
needs /spiffs/tests.fs

<EOF>






