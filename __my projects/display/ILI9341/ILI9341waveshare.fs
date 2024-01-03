\ *************************************
\ definitions for ILI9341
\    Filename:      ILI9341waveshare.fs
\    Date:          03 jan 2024
\    Updated:       03 jan 2024
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

\ LCD dimensions when Turn ON
240 constant LCD_WIDTH
320 constant LCD_HEIGHT

\ ILI9341 LCD display pins using SP32 SPI interface
19 constant LCD_MOSI    \ DIN
18 constant LCD_SCK     \ CLK
 5 constant LCD_CS      \ CS

\  4 constant LCD_RESET   \ RST
\ 16 constant LCD_DC      \ DC
\ 17 constant LCD_BL      \ BL




<EOF>


