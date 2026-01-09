\ *************************************
\ display text to SSD1306 128x32 display
\    Filename:      oled128x32-display.fs
\    Date:          26 nov 2025
\    Updated:       26 nov 2025
\    File Version:  1.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ **************************************


RECORDFILE /spiffs/oled128x32-display.fs
\ set oled SSD1306 dimensions

also oled

128 to WIDTH
 32 to HEIGHT

\ set adress of OLED SSD1306 display 128x32 pixels
$3c constant I2C_SSD1306_ADDRESS

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

\ store number of lines printed on oled screen
0 value nbLines   

\ clear oled screen
: oled.cls ( -- )
    OledCls   OledDisplay
    OledHOME
    0 to nbLines
  ;

\ print text and go to next line
: oled.println ( zstr -- )
    OledPrintln OledDisplay 
    1 +to nbLines
  ;

only FORTH
    
<EOF>


