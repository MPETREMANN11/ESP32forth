\ *********************************************************************
\ OLED SSD1306 128 x 32 px
\    Filename:      oledSSD1306.fs
\    Date:          07 apr. 2024
\    Updated:       03 may  2024
\    File Version:  0.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     ---
\    GNU General Public License
\ *********************************************************************



RECORDFILE /spiffs/oledSSD1306.fs

\ address set in config.fs
0 value I2C_SSD1306_ADDRESS

\ values of SDA and SCL pins set in config.fs file
0 value SSD1306_SCL
0 value SSD1306_SDA

wire also \ add wire vocabulary in search order

: wire.init ( -- )
    SSD1306_SDA SSD1306_SCL wire.begin
    0 = if
        abort" SD1306 wire initialisation error"
    then
  ;

only
oled also  \ add oled vocabulary in search order

: .ready ( -- )
    OledCLS
    4 OledTextsize      \ Draw 2x Scale Text
    WHITE OledTextc     \ Draw white text
    0 0 OledSetCursor   \ Start at top-left corner
    z" ready" OledPrintln OledDisplay
  ;

: Oled128x32Init ( -- )
    OledAddr @ 0= 
    if
        WIDTH HEIGHT OledReset OledNew
        SSD1306_SWITCHCAPVCC I2C_SSD1306_ADDRESS OledBegin drop
    then
    .ready
  ;


\ *** Dispaly Voltages on OLED screen ******************************************

\ display xxx digits in formatted string
\ n must be integer from Voltage x 10, example: 12.6 -> 126
: ##Volts ( n -- addr len )
    <#  [char] V hold bl hold  
        #  [char] . hold  # #  #> 
  ;

\ draw a black rectangle 128 px width 8 px height at x y
\ : oled.lines.clear { x y -- }
\     x y x 128 +  y 8 +  BLACK OledRectF 
\     OledDisplay
\   ;

\ erase display at line n, with n in [0..3]
\ : lineCls ( n -- )
\     0 swap 8 * oled.lines.clear
\   ;
    

: .volt ( voltn -- )
    SF@ 10e F* f>s ##Volts s>z OledPrintln
  ;

\ display voltages
: .volt_1_2 ( Vout1 Vout2 -- )
    OledCls
    2 OledTextsize      \ Draw 2x Scale Text
    WHITE OledTextc     \ Draw white text
    0 0 OledSetCursor   \ Start at top-left corner
    FSWAP .volt  .volt  OledDisplay
  ;


only

<EOF>

