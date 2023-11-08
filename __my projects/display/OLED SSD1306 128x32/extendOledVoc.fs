\ *************************************
\ addtionnal definitions for oled vocabulary
\    Filename:      addDeffs.fs
\    Date:          07 nov 2023
\    Updated:       07 nov 2023
\    File Version:  1.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ **************************************


RECORDFILE /spiffs/extendOledVoc.fs
oled definitions
: OledTriangle { x0 y0 x1 y1 x2 y2 color -- }
    x0 y0 x1 y1 color OledDrawL
    x1 y1 x2 y2 color OledDrawL
    x2 y2 x0 y0 color OledDrawL
  ;
forth definitions
<EOF>




oled
OledCLS OledDisplay
0 0 80 30 8 WHITE OledRectRF OledDisplay
