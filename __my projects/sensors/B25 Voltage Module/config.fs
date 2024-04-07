\ *********************************************************************
\ B25 VCC Sensor
\    Filename:      config.fs
\    Date:          31 mars 2024
\    Updated:       02 apr. 2024
\    File Version:  0.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/config.fs

\ set adress of OLED SSD1306 display 128x32 pixels
DEFINED? I2C_SSD1306_ADDRESS [IF] 
    $3c constant I2C_SSD1306_ADDRESS
[THEN]

\ set oled SSD1306 dimensions
oled
128 to WIDTH
 32 to HEIGHT
forth

\ set pins for OLED SSD1306 SDA and SCL
DEFINED? SSD1306_SDA [IF] 
    0 to SSD1306_SDA
    1 to SSD1306_SCL
[THEN]

\ set pins for OLED SSD1306 SDA and SCL
DEFINED? BUZZER_PIN [IF] 
    10 to BUZZER_PIN
[THEN]


<EOF>


