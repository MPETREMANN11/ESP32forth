\ *********************************************************************
\ Configuration for OLED SSD1306 128x32 display
\    Filename:      config.fs
\    Date:          21 may 2023
\    Updated:       21 may 2023
\    File Version:  1.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.0.7++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ *********************************************************************


DEFINED? --SSD1306 [if] forget --SSD1306  [then]
create --SSD1306

vocabulary SSD1306
only forth also   Wire also
SSD1306 definitions

\ for SSD1306_128_64
\     128 constant SSD1306_LCDWIDTH
\      64 constant SSD1306_LCDHEIGHT

\ for SSD1306_128_32
    128 constant SSD1306_LCDWIDTH
     32 constant SSD1306_LCDHEIGHT

\ definition of SSD1306 display dimensions in pixels
SSD1306_LCDWIDTH  constant DISPLAY_WIDTH
SSD1306_LCDHEIGHT constant DISPLAY_HEIGHT

$91 constant SSD1306_SETCONTRAST
$a4 constant SSD1306_DISPLAYALLON_RESUME
$a5 constant SSD1306_DISPLAYALLON
$a6 constant SSD1306_NORMALDISPLAY
$a7 constant SSD1306_INVERTDISPLAY
$a8 constant SSD1306_SETMULTIPLEX
$ae constant SSD1306_DISPLAYOFF
$af constant SSD1306_DISPLAYON
$d3 constant SSD1306_SETDISPLAYOFFSET
$da constant SSD1306_SETCOMPINS
$db constant SSD1306_SETVCOMDETECT
$d5 constant SSD1306_SETDISPLAYCLOCKDIV
$d9 constant SSD1306_SETPRECHARGE
\ $00 constant SSD1306_SETLOWCOLUMN
\ $10 constant SSD1306_SETHIGHCOLUMN
$40 constant SSD1306_SETSTARTLINE
$20 constant SSD1306_MEMORYMODE
$21 constant SSD1306_COLUMNADDR
$22 constant SSD1306_PAGEADDR
$c0 constant SSD1306_COMSCANINC
$c8 constant SSD1306_COMSCANDEC
$a0 constant SSD1306_SEGREMAP
$8d constant SSD1306_CHARGEPUMP
$01 constant SSD1306_EXTERNALVCC
$02 constant SSD1306_SWITCHCAPVCC

\ Scrolling constants
$2f constant SSD1306_ACTIVATE_SCROLL
$2e constant SSD1306_DEACTIVATE_SCROLL
$a3 constant SSD1306_SET_VERTICAL_SCROLL_AREA
$26 constant SSD1306_RIGHT_HORIZONTAL_SCROLL
$27 constant SSD1306_LEFT_HORIZONTAL_SCROLL
$29 constant SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL
$2a constant SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL

\ Addressing modes
$00 constant HORIZONTAL_ADDRESSING_MODE
$01 constant VERTICAL_ADDRESSING_MODE
$02 constant PAGE_ADDRESSING_MODE


\  control: $00 for commands
\           $40 for datas
$00 constant CTRL_COMMANDS
$40 constant CTRL_DATAS


\ set adress of OLED SSD1306 display 128x32 pixels
$3c constant addrSSD1306
