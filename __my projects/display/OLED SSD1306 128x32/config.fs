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
\ set oled SSD1306 dimensions
oled
128 to WIDTH
 32 to HEIGHT
forth

\ set adress of OLED SSD1306 display 128x32 pixels
$3c constant I2C_SSD1306_ADDRESS


<EOF>
