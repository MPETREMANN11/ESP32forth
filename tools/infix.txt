\ *********************************************************************
\ convert infix to postfix notation
\    Filename:      infix.txt
\    Date:          15 oct 2022
\    Updated:       20 oct 2022
\    File Version:  1.1
\    MCU:           ESP32Forth
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

DEFINED? --infix [if] forget --infix  [then]
create --infix

\ Define operator stack
80 constant OPERATORS         \ increase value 80 if needed
cell 2* OPERATORS * constant OP-SIZE
create op               \ operator stack
    OP-SIZE allot

\ pointer for operator pointer
0 value op-pointer

\ get current operator pointer
: opp@    ( --- addr )
    op op-pointer +
  ;

\ increase operator pointer
: opp+ ( -- )
    cell 2* +to op-pointer 
  ;

\ decrease operator pointer
: opp- ( -- )
    cell 2* negate +to op-pointer 
  ;

\ execute or compile word
: ?interp ( cfa -- )
    state @             \ 0 = interpret mode, -1 = compile mode
    if      ,           \ compile word
    else    execute     \ execute word
    then
  ;

\ store cfa and level on operator staock
: >op     ( cfa level -- )
    opp+ 
    opp@ 2!
  ;

\ get cfa from operator stacl
: op>     ( -- )
    opp@ 2@
    opp- 
    swap drop ?interp
  ;

\ get level pointed by current operator stack
: lev?    ( -- level )
    op-pointer op + @
  ;

\ end an algebric formula
: ]$
    begin   lev?
    while   op>
    repeat
    forth ; immediate

\ execute part of algebra operator
: dealing  ( addr --- )
    2@
    begin   over lev? > invert
    while   >r >r op> r> r>
    repeat
    >op
  ;

\ define algebra operator.
: infix   ( n --- <word1> <word2>   in compilation )
    '               \ get cfa word1
    create          \ create word2
        swap , , 
        immediate
    does>
        dealing 
  ;

vocabulary algebra    algebra definitions
7 infix * *    7 infix / /    
6 infix + +    6 infix - -
5 infix > >    5 infix < <    5 infix = =
4 infix invert not
3 infix and and
2 infix or or

: noop ;

: (
    1  ['] noop >op 
  ; immediate

: )
    [ forth ]
    begin   1 lev? <
    while   op>
    repeat
    1 lev? =
    if    opp-
    else  ." miss " 40 emit space  
          -1 throw
    then 
  ; immediate

\ start algebric formula
forth definitions
: $[
    op OP-SIZE 0 fill
    0 to op-pointer
    algebra 
  ; immediate


\ : tt
\    $[ ( 2 + 3 ) * ( 4 + 5 ) ]$
\   ;


\ *** examples: ******************************************************

2 value two
0 value result
: ex1
    $[ ( ( two + 3 ) * ( 4 + 1 ) ) ]$
    to result
    \ two 3 + 4 1 + *
  ;


: ex2 
    $[ ( ( 2 + 3 ) * ( 4 - 1 ) / 2 ) ]$
    \ 2 3 + 4 1 - * 2 /
  ;


: ex3
    $[ 48 / 2 * ( 9 + 3 ) ]$ 
    \ 48 2 / 9 3 + *
  ;


-1 constant true
 0 constant false

0 value KELVIN
0 value FAHRENHEIT
0 value tempCelsius
: toK ( ---)
    false to FAHRENHEIT
    true  to KELVIN
  ;
: toF ( ---)
    false to KELVIN
    true  to FAHRENHEIT
  ;
: .temp ( deg --- deg2)
    to tempCelsius
    $[
        ( ( tempCelsius + 273 ) and KELVIN )   +
        ( ( ( tempCelsius * 9 / 5 ) + 32 ) and FAHRENHEIT )
    ]$
    \ tempCelsius 273 + KELVIN AND tempCelsius 9 * 5 / 32 + FAHRENHEIT AND + .
    .
  ;
\ usage:
\ 10 ToF .temp 42  ok
\ 10 toK .temp 283  ok


\ *** Example with filled rectangles **************************

0 value DX      \ current X coordinate
0 value DY      \ current Y coordinate
 
: dx+  ( ---)   \ increment DX 1 position
    DX 1+ to DX ;
 
: dy+  ( ---)   \ increment DY 1 position
    DY 1+ to DY ;

0 value SX    \ x Start rect 1
0 value EX    \ x End   rect 1
0 value SY    \ y Start rect 1 
0 value EY    \ y Etart rect 1 

: inRect? ( --- fl)
    $[      ( NOT ( DX < SX  ) ) AND 
            ( NOT ( DX > EX  ) ) AND 
            ( NOT ( DY < SY  ) ) AND 
            ( NOT ( DY > EY  ) )
    ]$  ;

: inLeftRect? ( --- fl)
    4  to SX    \ x Start rect left
    40 to EX    \ x End   rect left
    4  to SY    \ y Start rect left 
    16 to EY    \ y Etart rect left 
    inRect?  ;
 
: inRightRect? ( --- fl)
    22 to SX    \ x Start rect right
    58 to EX    \ x End   rect right
    10 to SY    \ y Start rect right 
    26 to EY    \ y Etart rect right 
    inRect?  ;

: dispChar ( ---)
    $[    \ 46  is char .
          ( 46  * ( ( ( NOT inLeftRect? ) AND ( NOT inRightRect? ) ) abs ) )
          \ 43  is char +
        + ( 43  * ( ( ( inLeftRect? ) AND ( NOT inRightRect? ) ) abs ) )
          \ 88  is char X
        + ( 88  * ( inLeftRect? AND inRightRect? abs ) )
          \ 111 is char o
        + ( 111 * ( ( ( inRightRect? ) and ( NOT inLeftRect? ) ) abs ) )
    ]$
    emit
  ;

\ Execution of see dispChar display:
\ : dispChar
\     46 inLeftRect? invert inRightRect? invert AND abs * 
\     43 inLeftRect? inRightRect? invert AND abs * + 
\     88 inLeftRect? inRightRect? abs AND * + 
\     111 inRightRect? inLeftRect? invert AND abs * + 
\     emit
\ ;

: graphLoop ( ---)
    30 for
        cr
        120 for
            dispChar
            dx+
        next
        dy+
        0 to DX
    next ;

\ Execution of graphLoop display:
\ .........................................................................................................................
\ .........................................................................................................................
\ .........................................................................................................................
\ .........................................................................................................................
\ ....+++++++++++++++++++++++++++++++++++++................................................................................
\ ....+++++++++++++++++++++++++++++++++++++................................................................................
\ ....+++++++++++++++++++++++++++++++++++++................................................................................
\ ....+++++++++++++++++++++++++++++++++++++................................................................................
\ ....+++++++++++++++++++++++++++++++++++++................................................................................
\ ....+++++++++++++++++++++++++++++++++++++................................................................................
\ ....++++++++++++++++++XXXXXXXXXXXXXXXXXXXoooooooooooooooooo..............................................................
\ ....++++++++++++++++++XXXXXXXXXXXXXXXXXXXoooooooooooooooooo..............................................................
\ ....++++++++++++++++++XXXXXXXXXXXXXXXXXXXoooooooooooooooooo..............................................................
\ ....++++++++++++++++++XXXXXXXXXXXXXXXXXXXoooooooooooooooooo..............................................................
\ ....++++++++++++++++++XXXXXXXXXXXXXXXXXXXoooooooooooooooooo..............................................................
\ ....++++++++++++++++++XXXXXXXXXXXXXXXXXXXoooooooooooooooooo..............................................................
\ ....++++++++++++++++++XXXXXXXXXXXXXXXXXXXoooooooooooooooooo..............................................................
\ ......................ooooooooooooooooooooooooooooooooooooo..............................................................
\ ......................ooooooooooooooooooooooooooooooooooooo..............................................................
\ ......................ooooooooooooooooooooooooooooooooooooo..............................................................
\ ......................ooooooooooooooooooooooooooooooooooooo..............................................................
\ ......................ooooooooooooooooooooooooooooooooooooo..............................................................
\ ......................ooooooooooooooooooooooooooooooooooooo..............................................................
\ ......................ooooooooooooooooooooooooooooooooooooo..............................................................
\ ......................ooooooooooooooooooooooooooooooooooooo..............................................................
\ ......................ooooooooooooooooooooooooooooooooooooo..............................................................
\ ......................ooooooooooooooooooooooooooooooooooooo..............................................................
\ .........................................................................................................................
\ .........................................................................................................................
\ .........................................................................................................................
\ ......................................................................................................................... ok






