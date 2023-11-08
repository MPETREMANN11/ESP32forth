\ *********************************************************************
\ tones management and test
\    Filename:      tonesTests.txt
\    Date:          08 nov 2023
\    Updated:       08 neov 2023
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ *********************************************************************





RECORDFILE /spiffs/tonesTests.fs
: music-scale ( -- )
    C C# D D# E F F# G G# A A# B
  ;

initTones
forth also music also
.|"
80 to SUSTAIN
OCT1 music-scale
OCT2 music-scale
OCT3 music-scale
OCT4 music-scale 
OCT5 music-scale
OCT6 music-scale

initTones
forth also music also

: 1stLine ( -- )
    .|"  ( duration of a quarter black note )
    OCT5 MI RE# RE DO#     RE DO# DO  OCT4 SI
    OCT5 DO  OCT4 SI LA# LA     SOL# SOL FA# FA
    MI RE# RE DO#    RE DO# DO OCT3 SI 
  ;

: 2ndLine ( -- )
    .|"  ( duration of a quarter black note )
    OCT4 DO OCT3 SI FA# FA    SOL# SOL FA# FA
    MI RE# RE DO#    RE DO DO# OCT2 SI OCT3
    MI RE# RE DO#    RE DO DO OCT2 SI OCT3
    MI RE# RE DO#    DO FA FA RE#
  ;

: 3rdLine ( -- )
    .|"  ( duration of a quarter black note )
    MI RE# RE DO#   DO DO# RE RE#
    MI RE# RE DO#    DO FA FA RE#
    MI RE# RE DO#    DO DO# RE RE#
    MI RE# RE DO#    RE DO DO# OCT2 SI OCT3
  ;

: flightBumbleBee ( -- )
    initTones
    1stLine
    2ndLine
    3rdLine
  ;


<EOF>



