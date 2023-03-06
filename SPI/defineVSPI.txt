\ *********************************************************************
\ define VSPI port
\    Filename:      define VSPI.txt
\    Date:          06 mar 2023
\    Updated:       06 mar 2023
\    File Version:  0.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

\ define VSPI pins
\             ESP32             MAX7219
19 constant VSPI_MISO  \ <-->    unused
23 constant VSPI_MOSI  \ <-->    DIN
18 constant VSPI_SCLK  \ <-->    CLK
05 constant VSPI_CS    \ <-->    CS

\ define SPI port frequency
4000000 constant SPI_FREQ

\ select SPI vocabulary
only FORTH  SPI also

\ initialize SPI port
: init.VSPI ( -- )
    VSPI_CS OUTPUT pinMode
    VSPI_SCLK VSPI_MISO VSPI_MOSI VSPI_CS SPI.begin
    SPI_FREQ SPI.setFrequency
  ;

