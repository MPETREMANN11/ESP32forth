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


