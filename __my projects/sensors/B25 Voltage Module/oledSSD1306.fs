\ *********************************************************************
\ OLED SSD1306 128 x 32 px
\    Filename:      oledSSD1306.fs
\    Date:          07 apr. 2024
\    Updated:       07 apr. 2024
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

: wire.init ( -- )
    [ wire ]
    SSD1306_SDA SSD1306_SCL wire.begin
    [ forth ]
    0 = if
        abort" SD1306 wire initialisation error"
    then
  ;


: .ready ( -- )
    [ oled ]
    OledCLS
    4 OledTextsize      \ Draw 2x Scale Text
    WHITE OledTextc     \ Draw white text
    0 0 OledSetCursor   \ Start at top-left corner
    z" ready" OledPrintln OledDisplay
    [ forth ]
  ;

: Oled128x32Init
    [ oled ]
    OledAddr @ 0= 
    if
        WIDTH HEIGHT OledReset OledNew
        SSD1306_SWITCHCAPVCC I2C_SSD1306_ADDRESS OledBegin drop
    then
    .ready
    [ forth ]
  ;

\ display xxx digits in formatted string
: .xxVolts ( n -- )
    <# #  [char] . hold  # #  #> type
  ;

<EOF>

