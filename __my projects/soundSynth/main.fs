\ *********************************************************************
\ ESP32 Sound Synth
\    Filename:      main.fs
\    Date:          26 mars 2024
\    Updated:       26 mars 2024
\    File Version:  0.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/main.fs
DEFINED? --soundSynth [if] forget --soundSynth  [then]
create --soundSynth

s" /spiffs/config.fs"           included

s" /spiffs/tests.fs"            included

<EOF>


