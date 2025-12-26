\ *********************************************************************
\ Command LEDs from GPIOs 
\    Filename:      ledsCommand.fs
\    Date:          14 dec. 2025
\    Updated:       14 dec. 2025
\    File Version:  0.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/ledsCommand.fs

\ define 3 GPIOs for LEDs
25 constant LED_RED
26 constant LED_YELLOW
27 constant LED_GREEN

\ set GPIOs in output mode
: initLeds ( -- )
    LED_RED    OUTPUT pinMode
    LED_YELLOW OUTPUT pinMode
    LED_GREEN  OUTPUT pinMode
  ;

\ turn LEDx ON
: ledOn ( led -- )
    HIGH digitalWrite  ;

\ turn LEDx OFF
: ledOff ( led -- )
    LOW  digitalWrite  ;

: redOn  ( -- )     LED_RED ledOn  ;
: redOff ( -- )     LED_RED ledOff ;
: yellowOn  ( -- )  LED_YELLOW ledOn  ;
: yellowOff ( -- )  LED_YELLOW ledOff ;
: greenOn  ( -- )   LED_GREEN ledOn  ;
: greenOff ( -- )   LED_GREEN ledOff ;

<EOF>




