\ *************************************
\ graphics for ILI9341 series
\    Filename:      graphics.fs
\    Date:          03 jan 2024
\    Updated:       04 jan 2024
\    File Version:  1.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Display:       ILI9341 Waveshare and others ??
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ **************************************



RECORDFILE /spiffs/graphics.fs

\ Color definitions
$0000 constant ILI9341_BLACK        \   0,   0,   0
$000F constant ILI9341_NAVY         \   0,   0, 128
$03E0 constant ILI9341_DARKGREEN    \   0, 128,   0
$03EF constant ILI9341_DARKCYAN     \   0, 128, 128
$7800 constant ILI9341_MAROON       \ 128,   0,   0
$780F constant ILI9341_PURPLE       \ 128,   0, 128
$7BE0 constant ILI9341_OLIVE        \ 128, 128,   0
$C618  constant ILI9341_LIGHTGREY   \ 192, 192, 192
$7BEF constant ILI9341_DARKGREY     \ 128, 128, 128
$001F constant ILI9341_BLUE         \ 0,   0, 255
$07E0 constant ILI9341_GREEN        \   0, 255,   0
$07FF constant ILI9341_CYAN         \   0, 255, 255
$F800 constant ILI9341_RED          \ 255,   0,   0
$F81F constant ILI9341_MAGENTA      \ 255,   0, 255
$FFE0 constant ILI9341_YELLOW       \ 255, 255,   0
$FFFF constant ILI9341_WHITE        \ 255, 255, 255
$FD20 constant ILI9341_ORANGE       \ 255, 165,   0
$AFE5 constant ILI9341_GREENYELLOW  \ 173, 255,  47
$F81F constant ILI9341_PINK

<EOF>


