\ *********************************************************************
\ BME688
\    Filename:      autoexec.fs
\    Date:          31 mars 2024
\    Updated:       31 mars 2024
\    File Version:  0.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     ---
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/autoexec.fs
create crlf 13 C, 10 C,
: RECORDFILE  ( "filename" "filecontents" "<EOF>" -- )
    bl parse
    W/O CREATE-FILE throw >R
    BEGIN
        tib #tib accept
        tib over
        S" <EOF>" startswith?
        DUP IF
            swap drop
        ELSE
            swap
            tib swap
            R@ WRITE-FILE throw
            crlf 1+ 1 R@ WRITE-FILE throw
        THEN
    UNTIL
    R> CLOSE-FILE throw
;
: MAIN
    s" /spiffs/main.fs"       included
  ;
internals 120 to line-width
forth
<EOF>
