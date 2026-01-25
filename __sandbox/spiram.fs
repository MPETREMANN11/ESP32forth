\ *************************************
\ SPI RAM managing
\    Filename:      spiram.fs
\    Date:          25 jan 2026
\    Updated:       25 jan 2026
\    File Version:  1.0
\    MCU:           ESP32-S3 - only with PSRAM
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ **************************************



PSRAM? [IF]

also internals

\ Version qui retourne l'adresse sur la pile (plus universel)
: psram-alloc ( size -- addr )
    MALLOC_CAP_8BIT MALLOC_CAP_SPIRAM or
    heap_caps_malloc ;

\ Version qui libère n'importe quelle adresse PSRAM
: psram-free-addr ( addr -- )
    ?dup if heap_caps_free then ;

only forth

[THEN]

<EOF>


: test-psram
  1024 1024 * psram-alloc ( demande 1 Mo )
  dup 0= if ." Erreur d'allocation !" drop exit then
  to PSRAM_BUFFER
  ." Allocation réussie à l'adresse: " PSRAM_BUFFER .
;