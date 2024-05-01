\ *********************************************************************
\ BME688
\    Filename:      tests.fs
\    Date:          31 mars 2024
\    Updated:       07 apr. 2024
\    File Version:  0.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     ---
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/tests.fs

\ *** UNIT TESTS *********************
s" /spiffs/assert.fs"         included

\ test B25 GPIOs correctly initialized
assert( B25_NUM_1 4 = )
assert( B25_NUM_2 3 = ) 

\ test SSD1306 I2C address correctly initialized
assert( I2C_SSD1306_ADDRESS $3c = )

\ test SSD1306 SCL & SDA GPIO correctly initialized
assert( SSD1306_SDA 0 = )
assert( SSD1306_SCL 1 = )

\ test BUZZER PIN correctly initialized
DEFINED? BUZZER_PIN [IF] 
    assert( BUZZER_PIN 10 = )
[THEN]


\ *** END OF UNIT TESTS **************

buzzer.init
B25.init
wire.init
Oled128x32Init



<EOF>


main
wire.init
wire.detect
Oled128x32Init

\ Simple Analogread to GPIO-4, bat: 12,4V (Voltmeter)
4 input pinMode
4 analogread  \ leave 3514

Mesure sur batterie 12,4V: 4 analogRead = 3514
( 3514 * 2.9 / 4096 )  / ( 7500 / ( 30000 + 7500 ) ) = 12.4396972656


 7500e fconstant RES_1
30000e fconstant RES_2
RES_1 RES_2 RES_1 F+ F/ fconstant RES_DIVISOR


fvariable Vout
fvariable Vref     
    3.3e Vref SF!
fvariable RES
    4095e RES SF!
: Vout! ( ADCout -- )
    s>f Vref SF@ F*  RES SF@ F/ 
    RES_DIVISOR F/
    FDUP F.
    Vout SF!
  ;



\ get voltage from GPIO, must be B25_NUM_1 or B25_NUM_2
: getVoltage ( #gpio -- )
  ;





