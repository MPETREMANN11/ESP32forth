\ *********************************************************************
\ B25 VCC Sensor configuration file
\    Filename:      config.fs
\    Date:          31 mars 2024
\    Updated:       03 may  2024
\    File Version:  0.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/config.fs

\ *** set parameters for B25 modules *** file b25.fs ******************

\ set GPIOs used by B25 modules
DEFINED? B25_NUM_1 [IF] 
    4 to B25_NUM_1          \ GPIO 4 to first B25 module
[THEN]
DEFINED? B25_NUM_2 [IF] 
    3 to B25_NUM_2          \ GPIO 3 to second B25 module
[THEN]

\ *** set OLED 128x32 pixels address *** file oledSSD1306.fs **********
DEFINED? I2C_SSD1306_ADDRESS [IF] 
    $3c constant I2C_SSD1306_ADDRESS
[THEN]


\ set oled SSD1306 parameters *** file oledSSD1306.fs *****************

oled            \ set OLED SSD1306 screen width and height
128 to WIDTH
 32 to HEIGHT
forth

\ set pins for OLED SSD1306 SDA and SCL
DEFINED? SSD1306_SDA [IF] 
    0 to SSD1306_SDA
    1 to SSD1306_SCL
[THEN]

\ *** set parameters for buzzer *** file buzzer.fs ********************

DEFINED? BUZZER_PIN [IF] 
    10 to BUZZER_PIN
[THEN]


<EOF>


