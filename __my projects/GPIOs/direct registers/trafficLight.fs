\ *********************************************************************
\ manage traffic lights with direct registers access
\    Filename:      trafficLight.fs
\    Date:          13 nov 2023
\    Updated:       13 nov 2023
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/trafficLight.fs

\ define masks for red yellow and green LEDs
1 ledRED     defMASK: mLED_RED
1 ledYELLOW  defMASK: mLED_YELLOW
1 ledGREEN   defMASK: mLED_GREEN

\ initialisation GPIO G25 G26 and G27 in output mode
: GPIO.init ( -- )
    1 mLED_RED     GPIO_ENABLE_REG regSet
    1 mLED_YELLOW  GPIO_ENABLE_REG regSet
    1 mLED_GREEN   GPIO_ENABLE_REG regSet
  ;

\ define a ON and OFF sequence
: GPIO.on.off.sequence { position mask delay -- }
    1 position mask GPIO_OUT_W1TS_REG regSet
    delay ms
    1 position mask GPIO_OUT_W1TC_REG regSet
  ;

\ define a ON and OFF sequence
: TRAFFIC.sequence { val position mask delay -- }
    val position mask GPIO_OUT_W1TS_REG regSet
    delay ms
    val position mask GPIO_OUT_W1TC_REG regSet
  ;

: TRAFFIC.red ( -- )
    1 mLED_RED    2500 TRAFFIC.sequence  ;
: TRAFFIC.yellow ( -- )
    1 mLED_YELLOW 1000 TRAFFIC.sequence  ;
: TRAFFIC.green ( -- )
    1 mLED_GREEN  3000 TRAFFIC.sequence  ;
: TRAFFIC.red-yellow ( -- )
    3 mLED_RED
      mLED_YELLOW nip + 500 TRAFFIC.sequence  ;

: TRAFFIC.german.cycle ( -- )
    TRAFFIC.green
    TRAFFIC.yellow
    TRAFFIC.red
    TRAFFIC.red-yellow
  ;

: TRAFFIC.loop ( -- )
    begin
        TRAFFIC.german.cycle
    key? until
  ;
<EOF>

