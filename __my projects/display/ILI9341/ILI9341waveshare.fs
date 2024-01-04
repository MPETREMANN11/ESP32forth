\ *************************************
\ definitions for ILI9341
\    Filename:      ILI9341waveshare.fs
\    Date:          03 jan 2024
\    Updated:       04 jan 2024
\    File Version:  1.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Display:       ILI9341 Waveshare
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ **************************************

\ Wiki:      https://www.waveshare.com/wiki/2.4inch_LCD_Module
\ Datasheet: https://www.waveshare.com/w/upload/e/e3/ILI9341_DS.pdf

RECORDFILE /spiffs/ILI9341waveshare.fs

\ LCD dimensions when Turn LCD ON
240 constant LCD_WIDTH
320 constant LCD_HEIGHT

\ values modified when init or rotation
0 value lcdWidth
0 value lcdHeight


\ ILI9341 LCD display pins using SP32 SPI interface
19 constant LCD_MOSI    \ DIN - SPI data input
20 constant LCD_MISO    \ unused, needed by SPI.begin
18 constant LCD_SCK     \ CLK - SPI clock input
 5 constant LCD_CS      \ CS  - Chip selection, low active
17 constant LCD_DC      \ DC  - Data/Command sel. (high = data, low = command)
16 constant LCD_RESET   \ RST - Reset, low active
 4 constant LCD_BL      \ BL  - Backlight

40000000 constant SPI_FREQ

0 value lcdWidth
0 value lcdHeight

also SPI \ select SPI vocabulary

\ ILI9341 display controller hard reset
: LCD.reset
    50 ms
    LCD_RESET LOW   digitalWrite
    50 MS
    LCD_RESET HIGH  digitalWrite
    50 ms
  ;

\ Backlight control function
: LCD.backlight ( HIGH|LOW -- )
    LCD_BL swap digitalWrite
  ;


\ Write 8 bit command to LCD controller
: LCD.cmdWrite ( cmd -- )
    LCD_CS LOW digitalWrite
    LCD_DC LOW digitalWrite
    SPI.write
    LCD_DC HIGH digitalWrite
    LCD_CS HIGH digitalWrite
  ;

\ Write 8 bit data to LCD controller
: LCD.8dataWrite ( data -- )
    LCD_CS LOW digitalWrite
    SPI.write
    LCD_CS HIGH digitalWrite
  ;
 
\ Write 16 bit data to LCD controller
: LCD.16dataWrite ( data -- )
    LCD_CS LOW digitalWrite
    SPI.write16
    LCD_CS HIGH digitalWrite
  ;

\ Write 32 bit data to LCD controller
: LCD.32dataWrite ( data -- )
  LCD_CS LOW digitalWrite
  SPI.write32
  LCD_CS HIGH digitalWrite
;

\ Get width of LCD screen in currect rotation
: LCD.getWidth
  lcdWidth
;

\ Get height of LCD screen in currect rotation
: LCD.getHeight
  lcdHeight
;


\ LCD general commands
$36 constant MADCTL         \ Memory Access Control

\ LCD display controller commands
$00 constant MADCTL_RGB     \ Red-Green-Blue pixel order
$04 constant MADCTL_MH      \ LCD refresh right to left
$08 constant MADCTL_BGR     \ Blue-Green-Red pixel order
$10 constant MADCTL_ML      \ LCD refresh Bottom to top
$20 constant MADCTL_MV      \ Reverse Mode
$40 constant MADCTL_MX      \ Right to left
$80 constant MADCTL_MY      \ Bottom to top

\ set portrait orientation
: orientation.portrait ( -- )
    LCD_WIDTH  to lcdWidth
    LCD_HEIGHT to lcdHeight
  ;

\ set landscape orientation
: orientation.landscape ( -- )
    LCD_WIDTH  to lcdWidth
    LCD_HEIGHT to lcdHeight
  ;

\ Set LCD display rotation:
\   0 top away from connector
\   1 top right of connector
\   2 top at connector
\   3 top left of connector
: LCD.rotation ( rotation -- )
    MADCTL LCD.cmdWrite
    4 mod
    case
        0 of 
            MADCTL_MX MADCTL_BGR or LCD.8dataWrite
            orientation.portrait
        endof
        1 of 
            MADCTL_MV MADCTL_BGR or LCD.8dataWrite
            orientation.landscape
        endof
        2 of 
            MADCTL_MY MADCTL_BGR or LCD.8dataWrite
            orientation.portrait
          endof
        3 of 
            MADCTL_MX MADCTL_MY MADCTL_MV MADCTL_BGR or or or LCD.8dataWrite
            orientation.landscape
          endof
    endcase
  ;






: SPI.init ( -- )
    \ set pins mode
    LCD_BL    OUTPUT pinMode
    LCD_RESET OUTPUT pinMode
    LCD_DC    OUTPUT pinMode
    LCD_SCK LCD_MOSI LCD_MISO LCD_CS SPI.begin
    \ set SPi frequency transmission
    SPI_FREQ SPI.setFrequency
  ;

\ initialize SPI pins and DC RST BL pins
: LCD.init ( -- )
    \ init screen Width and Height
    orientation.landscape
    \ Reset display
    LCD.reset
  ;








only forth definitions

<EOF>


