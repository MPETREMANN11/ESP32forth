\ *************************************
\ Convert decimal time in roman numeral digits
\    Filename:      clepsydra.fs
\    Date:          30 jan 2022
\    Updated:       19 nov 2023
\    File Version:  1.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ **************************************


\ include strings.txt

RECORDFILE /spiffs/clepsydra.fs

: vector
    create ( n -- ) 0
        do ,  loop
    does>  ( n -- )
        swap cells + @ execute
  ;

12 string antiquus   \ vintage in latin

: ,I
    dup     c@ antiquus c+$! ;
: ,V
    dup 1 + c@ antiquus c+$! ;
: ,X
    dup 2 + c@ antiquus c+$! ;

:noname  ,I ,X          drop ;
:noname  ,V ,I ,I ,I    drop ;
:noname  ,V ,I ,I       drop ;
:noname  ,V ,I          drop ;
:noname  ,V             drop ;
:noname  ,I ,V          drop ;
:noname  ,I ,I ,I       drop ;
:noname  ,I ,I          drop ;
:noname  ,I             drop ;

' drop ( 0 : no output )
    10 vector ,digit

: ?dup ( n -- n | n n )
    dup if dup then ;

: roman-rec ( year -- )
    10 /mod dup
    if
        >r over 2 + r> recurse
    else
        drop
    then
    ,digit
  ;

: within ( test low high -- flag )
    -rot over swap >=
    -rot swap <=
    and
  ;

\ convert n in roman numeral format
: roman ( n -- c-addr u )
    dup 0 4000 within 0= if
        ." EX LIMITO!"
        drop exit
    then
    antiquus 0$!
     s" IVXLCDM" drop swap roman-rec
    antiquus
  ;


\ used for tame in roman format concatenantion
16 string tempus

: tempusTo$ { HH MM -- }
    HH 0 =  MM 0=  AND if
        60  to MM
        23  to HH
    THEN
    HH 0 >  MM 0=  AND if
        60  to MM
        -1 +to HH
    then
    HH 0 <= if
        24 to HH
    then
    HH roman tempus $!
    [char] : tempus c+$!            \ add char :
    MM roman tempus append$
    tempus
  ;

: .tempus ( hh mm -- )
    tempusTo$ type ;

<EOF>


