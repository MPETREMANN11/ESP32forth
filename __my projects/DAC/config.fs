\ *************************************
\ redirect text stream to SSD1306 128x32 display
\    Filename:      config.fs
\    Date:          05 nov 2023
\    Updated:       05 nov 2023
\    File Version:  1.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ **************************************


RECORDFILE /spiffs/config.fs
\ define GPIOs for teone test
 0 constant CHANNEL0    \ define PWM channel 0
25 constant BUZZER      \ buzzer connected to GPI25

\ select initial ocatve, initial value 3, range [-1..8]
3 value OCTAVE

\ duration for one note
1600 constant WHOLE_NOTE_DURATION

WHOLE_NOTE_DURATION value duration

\ sustain of note, in interval [0..100]
90 value SUSTAIN

<EOF>
