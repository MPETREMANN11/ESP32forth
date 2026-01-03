\ *********************************************************************
\ strings management for ESP32forth
\    Filename:      strings.txt
\    Date:          20 jun 2021
\    Updated:       19 nov 2023
\    File Version:  1.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ *********************************************************************


RECORDFILE /spiffs/strings.fs
structures
struct __STRING
    ptr field >maxLength    \ point to max length of string
    ptr field >realLength   \ real length of string
    ptr field >strContent   \ string content
forth

\ compare two strings
: $= ( addr1 len1 addr2 len2 --- fl)
    str=
  ;

\ define a strvar
: string  ( n --- names_strvar )
    create
        dup
        ,                   \ n is maxlength
        0 ,                 \ 0 is real length
        allot
    does>
        dup >strContent
        swap >realLength @
  ;

\ get maxlength of a string
: maxlen$  ( strvar --- strvar maxlen )
    over __STRING - cell+ 
    >maxLength @
    ;

\ store str into strvar
: $!  ( str strvar --- )
    maxlen$                     \ get maxlength of strvar
    nip rot min                 \ keep min length
    2dup swap __STRING - cell+  \ store real length
    >realLength !
    cmove                       \ copy string
  ;

\ Example:
\ : s1
\     s" this is constant string" ;
\ 200 string test
\ s1 test $!

\ set length of a string to zero
: 0$! ( addr len -- )
    drop 0 swap 
    __STRING - cell+ >realLength !
  ;

\ Use: 
\ test 0$!
\ test type

\ extract n chars right from string
: right$  ( str1 n --- str2 )
    0 max over min >r + r@ - r>
  ;

\ Use:
\ s1 test $!
\ test 5 right$ type

\ extract n chars left frop string
: left$  ( str1 n --- str2 )
    0 max min
  ;

\ Use:
\ test 5 left$ type

\ extract n chars from pos in string
: mid$  ( str1 pos len --- str2 )
    >r over swap - right$ r> left$
  ;

\ Use:
\ test 1 10 mid$ type

\ append char c to string 
: c+$!  { car addr len -- }
    car addr len + c!
    1 addr __STRING - cell+ >realLength +!
  ; 

\ Use:
\ char X test c+$!
\ test type

\ append from addr1 len1 to addr2 len2 strings
: append$ { addr1 len1 addr2 len2 -- }
    addr1 addr2 len2 + len1 cmove
    len1 addr2 __STRING - cell+ >realLength +!
  ;

\ Use:
\ s" append text"  test append$
\ test type


\ work only with strings. Don't use with other arrays
: input$ { addr len -- }
    addr len maxlen$ nip accept
    addr __STRING - cell+ >realLength !
  ;
<EOF>

