\ *********************************************************************
\ Complete structures vocabulary
\    Filename:      structuresAdd.fs
\    Date:          27 jan 2024
\    Updated:       27 jan 2024
\    File Version:  0.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    GNU General Public License
\ *********************************************************************


\ Usage:
\ 
\     1) define a structure:
\     struct sockaddr
\         i8  field >len      \ ??
\         i8  field >family   \ AF_INET ??
\         14  field >data
\ 
\     2) define words to fettch or store:
\     : >familiy@
\         struct@ >family ;
\     : >family!
\         struct! >family ;


structures also definitions

: field@ ( -- )
    ' dup ,
    >body @ >r
    r@ 1 = if ['] c@   ,   then
    r@ 2 = if ['] uw@  ,   then
    r@ 4 = if ['] @    ,   then
    r@ 8 = if ['] 2@   ,   then
    rdrop
  ; immediate

: field! ( -- )
    ' dup ,
    >body @ >r
    r@ 1 = if ['] c!   ,   then
    r@ 2 = if ['] w!   ,   then
    r@ 4 = if ['] !    ,   then
    r@ 8 = if ['] 2!   ,   then
    rdrop
  ; immediate

previous
forth definitions


