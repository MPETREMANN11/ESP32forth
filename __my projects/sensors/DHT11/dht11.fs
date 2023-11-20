\ *********************************************************************
\ DHT11, DHT22 1-wire Control
\    Filename:      dht11.fs
\    Date:          11 nov 2023
\    Updated:       11 nov 2023
\    File Version:  0.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     ohiyooo2 / Frank Lin
\    Original publ. https://ohiyooo2.pixnet.net/blog/post/406078286
\    Adaptation for The Great Book for ESP32forth: Marc PETREMANN
\    GNU General Public License
\ *********************************************************************




RECORDFILE /spiffs/dht11.fs
\ define VSPI pins
\ 19 constant VSPI_MISO
23 constant VSPI_MOSI
18 constant VSPI_SCLK
05 constant VSPI_CS

DHT_PIN constant VSPI_MISO

\ define SPI port frequency
4000000 2 / constant SPI_FREQ

\ select SPI vocabulary
spi

\ initialize HSPI port
: init.VSPI ( -- )
    VSPI_CS OUTPUT pinMode
    VSPI_SCLK VSPI_MISO VSPI_MOSI VSPI_CS SPI.begin
    SPI_FREQ SPI.setFrequency
  ;

forth

\ read the state of digital I/O, 0=low, 1=high
: Pin@ ( pin -- status )
    digitalRead
;

: wait.DHT ( -- )     \ wait until pulse-high
    begin 
        DHT_PIN Pin@ 
    until
;

<EOF>
