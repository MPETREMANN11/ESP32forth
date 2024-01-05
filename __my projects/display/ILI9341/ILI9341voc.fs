\ *************************************
\ ILI9341 vocabulary
\    Filename:      ILI9341voc.fs
\    Date:          04 jan 2024
\    Updated:       04 jan 2024
\    File Version:  1.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Display:       ILI9341 Waveshare
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ **************************************

\ For functions names, see here:
\ https://github.com/Bodmer/TFT_ILI9341/blob/master/TFT_ILI9341.cpp

RECORDFILE /spiffs/ILI9341voc.fs

vocabulary ILI9341
ILI9341 definitions

\ send 8 bits data to ILI9341
: >spi8 ( n -- )
    postpone LCD.8dataWrite
  ; immediate

\ send 16 bits data to ILI9341
: >spi16 ( n -- )
    postpone LCD.16dataWrite
  ; immediate

\ send 32 bits data to ILI9341
: >spi32 ( n -- )
    postpone LCD.32dataWrite
  ; immediate

\ send CMD to ILI9341
: >spiCmd ( n -- )
    postpone LCD.cmdWrite
  ; immediate


\ Send command and optionals datas:
\ 0 cmd --          send only cmd command, 0 parameters
\ n0..nn n cmd --   send cmd folowed by n0 to nn datas, n parameters
: sendCommand ( n1..nn count command -- )
    over 0= if
        nip >spiCmd
    else
        >spiCmd
        0 do
            >spi8
        loop
    then
  ;




240 constant TFTWIDTH           \ ILI9341 max TFT width
320 constant TFTHEIGHT          \ ILI9341 max TFT height

0 value _width
0 value _height


\ #define ILI9341_NOP 0x00     ///< No-op register
\ #define ILI9341_SWRESET 0x01 ///< Software reset register
\ #define ILI9341_RDDID 0x04   ///< Read display identification information
\ #define ILI9341_RDDST 0x09   ///< Read Display Status
\ 
\ #define ILI9341_SLPIN 0x10  ///< Enter Sleep Mode
\ #define ILI9341_SLPOUT 0x11 ///< Sleep Out
\ #define ILI9341_PTLON 0x12  ///< Partial Mode ON
\ #define ILI9341_NORON 0x13  ///< Normal Display Mode ON
\ 
\ #define ILI9341_RDMODE 0x0A     ///< Read Display Power Mode
\ #define ILI9341_RDMADCTL 0x0B   ///< Read Display MADCTL
\ #define ILI9341_RDPIXFMT 0x0C   ///< Read Display Pixel Format
\ #define ILI9341_RDIMGFMT 0x0D   ///< Read Display Image Format
\ #define ILI9341_RDSELFDIAG 0x0F ///< Read Display Self-Diagnostic Result
\ 


\ #define ILI9341_GAMMASET 0x26 ///< Gamma Set
\ #define ILI9341_DISPOFF 0x28  ///< Display OFF
\ #define ILI9341_DISPON 0x29   ///< Display ON
\ 
\ #define ILI9341_CASET 0x2A ///< Column Address Set
\ #define ILI9341_PASET 0x2B ///< Page Address Set
\ #define ILI9341_RAMWR 0x2C ///< Memory Write
\ #define ILI9341_RAMRD 0x2E ///< Memory Read
\ 
\ #define ILI9341_PTLAR 0x30    ///< Partial Area
\ #define ILI9341_VSCRDEF 0x33  ///< Vertical Scrolling Definition


\ #define ILI9341_VSCRSADD 0x37 ///< Vertical Scrolling Start Address
\ #define ILI9341_PIXFMT 0x3A   ///< COLMOD: Pixel Format Set
\ 
\ #define ILI9341_FRMCTR1                                                        \
\   0xB1 ///< Frame Rate Control (In Normal Mode/Full Colors)
\ #define ILI9341_FRMCTR2 0xB2 ///< Frame Rate Control (In Idle Mode/8 colors)
\ #define ILI9341_FRMCTR3                                                        \
\   0xB3 ///< Frame Rate control (In Partial Mode/Full Colors)
\ #define ILI9341_INVCTR 0xB4  ///< Display Inversion Control
\ #define ILI9341_DFUNCTR 0xB6 ///< Display Function Control
\ 
\ #define ILI9341_PWCTR1 0xC0 ///< Power Control 1
\ #define ILI9341_PWCTR2 0xC1 ///< Power Control 2
\ #define ILI9341_PWCTR3 0xC2 ///< Power Control 3
\ #define ILI9341_PWCTR4 0xC3 ///< Power Control 4
\ #define ILI9341_PWCTR5 0xC4 ///< Power Control 5
\ #define ILI9341_VMCTR1 0xC5 ///< VCOM Control 1
\ #define ILI9341_VMCTR2 0xC7 ///< VCOM Control 2
\ 
\ #define ILI9341_RDID1 0xDA ///< Read ID 1
\ #define ILI9341_RDID2 0xDB ///< Read ID 2
\ #define ILI9341_RDID3 0xDC ///< Read ID 3
\ #define ILI9341_RDID4 0xDD ///< Read ID 4
\ 
\ #define ILI9341_GMCTRP1 0xE0 ///< Positive Gamma Correction
\ #define ILI9341_GMCTRN1 0xE1 ///< Negative Gamma Correction
\ //#define ILI9341_PWCTR6     0xFC



$20 constant INVOFF     \ Display Inversion OFF
$21 constant INVON      \ Display Inversion ON

\ Inverse display if bool is true
: invertDisplay ( bool -- )
    if      0 INVON  sendCommand
    else    0 INVOFF sendCommand
    then
  ;



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
    LCD_WIDTH  to _width
    LCD_HEIGHT to _height
  ;

\ void Adafruit_ILI9341::setRotation(uint8_t m) {
\   rotation = m % 4; // can't be higher than 3
\   switch (rotation) {
\   case 0:
\     m = (MADCTL_MX | MADCTL_BGR);
\     _width = ILI9341_TFTWIDTH;
\     _height = ILI9341_TFTHEIGHT;
\     break;
\   case 1:
\     m = (MADCTL_MV | MADCTL_BGR);
\     _width = ILI9341_TFTHEIGHT;
\     _height = ILI9341_TFTWIDTH;
\     break;
\   case 2:
\     m = (MADCTL_MY | MADCTL_BGR);
\     _width = ILI9341_TFTWIDTH;
\     _height = ILI9341_TFTHEIGHT;
\     break;
\   case 3:
\     m = (MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
\     _width = ILI9341_TFTHEIGHT;
\     _height = ILI9341_TFTWIDTH;
\     break;
\   }
\ 
\   sendCommand(ILI9341_MADCTL, &m, 1);
\ }

$36 constant MADCTL         \ Memory Access Control, need 2 parameters

: setRotation ( rotation -- )
    0 { _m }
    4 mod
    case
        0 of
            MADCTL_MX MADCTL_BGR or to _m
            orientation.portrait
        endof
        1 of
            MADCTL_MV MADCTL_BGR or or to _m
            orientation.landscape
        endof
        2 of
            MADCTL_MY MADCTL_BGR or or to _m
            orientation.portrait
          endof
        3 of
            MADCTL_MX MADCTL_MY MADCTL_MV MADCTL_BGR or or or or to _m
            orientation.landscape
          endof
    endcase
    _m 1 MADCTL sendCommand
  ;









// Color definitions
$0000 constant BLACK
$000F constant NAVY
$03E0 constant DARKGREEN
$03EF constant DARKCYAN
$7800 constant MAROON
$780F constant PURPLE
$7BE0 constant OLIVE
$C618 constant LIGHTGREY
$7BEF constant DARKGREY
$001F constant BLUE
$07E0 constant GREEN
$07FF constant CYAN
$F800 constant RED
$F81F constant MAGENTA
$FFE0 constant YELLOW
$FFFF constant WHITE
$FD20 constant ORANGE
$AFE5 constant GREENYELLOW
$FC18 constant PINK


FORTH definitions

<EOF>
