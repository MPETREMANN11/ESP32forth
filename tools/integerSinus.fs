\ *********************************************************************
\ Sinus and Cosinus
\    Filename:      integerSinus.fs
\    Date:          17 jul 2021
\    File Version:  1.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

\ Sinus and Cosinus
\ Use table calculating integer sin.
\ Get values scaled by 10K.

 create sintab  \ 0...90 Grad, Index in Grad
 0000 ,  0175 ,  0349 ,  0523 ,  0698 ,  0872 ,  1045 ,
 1219 ,  1392 ,  1564 ,  1736 ,  1908 ,  2079 ,  2250 , 
 2419 ,  2588 ,  2756 ,  2924 ,  3090 ,  3256 ,  3420 , 
 3584 ,  3746 ,  3907 ,  4067 ,  4226 ,  4384 ,  4540 , 
 4695 ,  4848 ,  5000 ,  5150 ,  5299 ,  5446 ,  5592 ,  
 5736 ,  5878 ,  6018 ,  6157 ,  6293 ,  6428 ,  6561 , 
 6691 ,  6820 ,  6947 ,  7071 ,  7193 ,  7314 ,  7431 , 
 7547 ,  7660 ,  7771 ,  7880 ,  7986 ,  8090 ,  8192 , 
 8290 ,  8387 ,  8480 ,  8572 ,  8660 ,  8746 ,  8829 , 
 8910 ,  8988 ,  9063 ,  9135 ,  9205 ,  9272 ,  9336 ,
 9397 ,  9455 ,  9511 ,  9563 ,  9613 ,  9659 ,  9703 , 
 9744 ,  9781 ,  9816 ,  9848 ,  9877 ,  9903 ,  9925 , 
 9945 ,  9962 ,  9976 ,  9986 ,  9994 ,  9998 , 10000 ,

\ fetch value from sintab, grad in interval 0..90
: sinus@   ( grad ---)
    cell *  sintab + @  ;

0 value isneg?      \ flag for negative value

: sin  ( grad -- sinus )
    dup 0< to isneg?  360 mod  dup 180 >
    if      180 -  -1 to isneg? 
    else    0 to isneg?     then
    dup  90 > 
    if      180 swap -      then
    sinus@  isneg?
    if      negate          then  ;

: cos ( grad -- cosinus )
    90 + sin  ;
