\ *********************************************************************
\ NEXUS SENSE
\    Filename:      nexus.fs
\    Date:          02 apr. 2024
\    Updated:       02 apr. 2024
\    File Version:  0.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     ---
\    GNU General Public License
\ *********************************************************************

\ values of SDA and SCL pins set in config.fs file
0 value NEXUS_SDA
0 value NEXUS_SCL

: wire.init ( -- )
    [ wire ]
    NEXUS_SDA NEXUS_SCL wire.begin
    [ forth ]
    0 > if
        abort" NEXUS wire initialisation error"
    then
  ;
