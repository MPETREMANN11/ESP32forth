\ *********************************************************************
\ ENUM in FORTH
\    Filename:      enum.fs
\    Date:          07 apr. 2024
\    Updated:       07 apr. 2024
\    File Version:  1.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     ---
\    Author:        FIG Chapter
\    GNU General Public License
\ *********************************************************************

: enum ( -- )  
    create 
        0 ,    
    does> ( -- n )  
        dup @ constant  
        1 swap +! 
  ;

\    An example of use is:
\ 
\    ENUM COLOR
\         COLOR RED
\         COLOR BLUE
\         COLOR GREEN

\ ENUM defines a new enumerate called COLOR.  COLOR can then be used to define
\ unique CONSTANTs for each color.  In the example, RED will return a value of
\ zero, BLUE will return 1, and GREEN will return 2.

