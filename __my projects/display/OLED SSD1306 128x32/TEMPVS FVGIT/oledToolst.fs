oledTools\ *************************************
\ tools for SSD1306 128x32 display
\    Filename:      oledTools.fs
\    Date:          05 nov 2023
\    Updated:       05 nov 2023
\    File Version:  1.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ **************************************


RECORDFILE /spiffs/oledTools.fs

oled

: Oled128x32Init
    OledAddr @ 0= 
    if
        WIDTH HEIGHT OledReset OledNew
        SSD1306_SWITCHCAPVCC I2C_SSD1306_ADDRESS OledBegin drop
    then
    OledCLS
    1 OledTextsize      \ Draw 2x Scale Text
    WHITE OledTextc     \ Draw white text
    0 0 OledSetCursor   \ Start at top-left corner
    z" 123456789" OledPrintln OledDisplay
  ;




forth

<EOF>






