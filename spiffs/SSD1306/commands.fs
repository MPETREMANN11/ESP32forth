\ *************************************
\ Manage commands for OLED SSD1306 128x32 display
\    Filename:      SSD10306commands.fs
\    Date:          21 may 2023
\    Updated:       21 may 2023
\    File Version:  1.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ **************************************


s" /spiffs/main.fs" included


\ begin I2C transmssion to SSD1306 OLED display
: SSD1306beginTransmission ( -- )
    addrSSD1306 Wire.beginTransmission
  ;

\ memoryze transmitted bytes, interval [00..31]
variable maxBytesSended

\ end I2C transmssion to SSD1306 OLED display
: SSD1306endTransmission ( -- )
    -1 Wire.endTransmission drop
    0 maxBytesSended !
  ;

\ if sended bytes > 32 end and restart transmission
: incrementMaxBytes ( -- )
    1 maxBytesSended +!
    maxBytesSended @ 31 > if
        SSD1306endTransmission
        SSD1306beginTransmission
    then
  ;

\ Send code c to I2C bus
: Wire.send ( c -- )
    >r 0 <# r> hold #>
    Wire.write drop
    incrementMaxBytes
  ;

\ send a data array to SSD1306 connected via I2C bus
\ CAUTION: array size max 32 bytes
: sendDatasToSSD1306 ( addr len -- )
    SSD1306beginTransmission
    Wire.write drop
    SSD1306endTransmission
  ;

\ define a command or data stream for SSD1306
: streamCreate: ( comp: <name> | exec: -- addr len )
    create
        here    \ leave current dictionnary pointer on stack
        0 c,    \ initial lenght data is 0
    does>
        dup 1+ swap c@
        \ send a data array to SSD1306 connected via I2C bus
        sendDatasToSSD1306
  ;

\ store at <WORD> addr length of datas compiled beetween
\ <WORD> and here
: ;endStream ( addr-var len ---)
    dup 1+ here
    swap -      \ calculate cdata length
    \ store c in first byte of word defined by streamCreate:
    swap c!
  ;


\ Initialize SSD105 128x32 display
streamCreate: dispSetup
    CTRL_COMMANDS c,
    SSD1306_DISPLAYOFF c,
    SSD1306_SETDISPLAYCLOCKDIV c,  $80 c,
    SSD1306_SETMULTIPLEX c,   DISPLAY_HEIGHT 1- c,
    SSD1306_SETDISPLAYOFFSET c,   $00 c,
    SSD1306_SETSTARTLINE $00 or  c,
    SSD1306_CHARGEPUMP c,  $14 c,   ( CHARGE_PUMP_ON )
    SSD1306_SEGREMAP $01 or c,   ( Reverse mapping )
    SSD1306_COMSCANDEC c,
    SSD1306_SETCOMPINS c,   $02 c,
    SSD1306_SETCONTRAST c,   $7f c,
    SSD1306_SETPRECHARGE c,   $22 c,
    SSD1306_SETVCOMDETECT c,   $40 c,
\     SSD1306_MEMORYMODE c,  HORIZONTAL_ADDRESSING_MODE c,
    SSD1306_DISPLAYALLON_RESUME c,
    SSD1306_NORMALDISPLAY c,
    SSD1306_DISPLAYON c,
    ;endStream

\ Reset SSD105 128x32 display
streamCreate: dispReset
    CTRL_COMMANDS c,
    SSD1306_DISPLAYOFF c,
    SSD1306_MEMORYMODE c,  HORIZONTAL_ADDRESSING_MODE c,
    SSD1306_COLUMNADDR c,   \ $21
    $00 c,  \ start
    $7f c,  \ end
    SSD1306_PAGEADDR c,     \ $22
    $00 c,  \ start
    $07 c,  \ end
    SSD1306_DISPLAYON c,
    ;endStream

\ main INITialization for SSD1306 128x32 OLED display
: dispInit ( -- )
    \ start the I2C interface using pin 21 and 22 on ESP32 DEVKIT V1
    \ with 21 used as sda and 22 as scl.
    21 22 wire.begin drop
    dispSetup
    dispReset
  ;

streamCreate: dispOn      \ toggle display on
    CTRL_COMMANDS c,
    $af c, \ DISPLAYON
    ;endStream

streamCreate: dispOff     \ toggle display off
    CTRL_COMMANDS c,
    $ae c, \ DISPLAYOFF
    ;endStream


\ store normal | inverse display state
SSD1306_NORMALDISPLAY value _SSD1306_DISPLAY

\ set display mode normal
: dispNormal  ( -- )
    SSD1306beginTransmission
    CTRL_COMMANDS Wire.send
    SSD1306_NORMALDISPLAY Wire.send
    SSD1306endTransmission
    SSD1306_NORMALDISPLAY to _SSD1306_DISPLAY
  ;

\ invert display mode
: dispInvert  
    SSD1306beginTransmission
    CTRL_COMMANDS Wire.send
    SSD1306_INVERTDISPLAY Wire.send
    SSD1306endTransmission
    SSD1306_INVERTDISPLAY to _SSD1306_DISPLAY
  ;

: OledInvert ( -- )
    \ test if actual display mode is normal
    _SSD1306_DISPLAY SSD1306_NORMALDISPLAY = if
        dispInvert
    else
        dispNormal
    then
  ;

: setLine { page -- }
    SSD1306beginTransmission
    CTRL_COMMANDS Wire.send
    SSD1306_COLUMNADDR Wire.send
    $00 Wire.send  \ start
    $7f Wire.send  \ end
    SSD1306_PAGEADDR Wire.send
    page Wire.send  \ start
    page Wire.send  \ end
    SSD1306endTransmission
  ;

\ pointer for current page [0..3]
0 value currentPage

\ move pointer to next page
: crLine ( ---)
    currentPage 1+ 3 and to currentPage
    currentPage setLine
  ;


\ Reset SSD105 128x32 display
streamCreate: dispReset
    CTRL_COMMANDS c,
    SSD1306_MEMORYMODE c,  HORIZONTAL_ADDRESSING_MODE c,
    SSD1306_COLUMNADDR c,   \ $21
    $00 c,  \ start
    $7f c,  \ end
    SSD1306_PAGEADDR c,     \ $22
    $00 c,  \ start
    $03 c,  \ end
    ;endStream

\ send 32x 0 value
streamCreate: sendBlock32bytesWith0 ( -- )
    CTRL_DATAS c,
    $00 c, $00 c, $00 c, $00 c, $00 c, $00 c, $00 c, $00 c, 
    $00 c, $00 c, $00 c, $00 c, $00 c, $00 c, $00 c, $00 c, 
    $00 c, $00 c, $00 c, $00 c, $00 c, $00 c, $00 c, $00 c, 
    $00 c, $00 c, $00 c, $00 c, $00 c, $00 c, $00 c, $00 c, 
    ;endStream

\ clear a complete line
: lineCLS ( -- )
    4 for
        aft
            sendBlock32bytesWith0
        then
    next
  ;

\ clear complete screen
: OledCLS  ( -- )
    dispReset
    DISPLAY_HEIGHT 8 / for
        aft
            lineCLS
        then
    next
  ;

\ adjust display contrast
: setContrast ( [0..254] -- )
    >r
    SSD1306beginTransmission
    CTRL_COMMANDS Wire.send
    SSD1306_SETCONTRAST Wire.send
    r>  Wire.send     \ value between 00..fe
    SSD1306endTransmission
  ;

