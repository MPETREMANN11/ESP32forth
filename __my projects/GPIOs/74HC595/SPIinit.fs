\ *********************************************************************
\ SPI & 74HC585 control
\    Filename:      SPIinit.fs
\    Date:          14 jan 2024
\    Updated:       14 jan 2024
\    File Version:  0.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

\ infos: https://lastminuteengineers.com/74hc595-shift-register-arduino-tutorial/
\        https://siliciumcorp.com/comment-fonctionne-le-8-bit-shift-register-74hc595 ***!!!


RECORDFILE /spiffs/SPIinit.fs

 6 constant CS1     \ Chip Select, SS1 SPI signal
 7 constant MOSI    \ Master Out Slave In
 8 constant SCLK    \ Signal CLocK
-1 constant MISO    \ unused

100000 constant SPI_FREQ

0 constant LSBFIRST 
1 constant MSBFIRST 

\ definition of SPI_MODE0..SPI_MODE3 constants  
0 constant SPI_MODE0
1 constant SPI_MODE1  
2 constant SPI_MODE2  
3 constant SPI_MODE3 

: SPI.init ( -- )
    CS1     OUTPUT pinMode          \ set pins mode
\     MOSI    OUTPUT pinMode
\     SCLK    OUTPUT pinMode
    [ SPI ]
    SCLK MISO MOSI CS1 SPI.begin    \ set SPI GPIOs
    SPI_FREQ SPI.setFrequency       \ set SPi frequency transmission
    MSBFIRST SPI.setBitOrder        \ set bit order
    SPI_MODE0 SPI.setDataMode       \ set SPI mode
    [ FORTH ]
  ;

<EOF>




