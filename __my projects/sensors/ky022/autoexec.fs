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
<EOF>

