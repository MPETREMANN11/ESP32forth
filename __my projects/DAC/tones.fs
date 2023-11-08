\ *********************************************************************
\ tones management and test
\    Filename:      tones.txt
\    Date:          08 nov 2023
\    Updated:       08 neov 2023
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/tones.fs
ledc    \ select ledc vocabulary
: initTones ( -- )
    BUZZER CHANNEL0 ledcAttachPin
  ;

\ select octave in interval -1..8
: set.octave ( n[-1..8] )
    to OCTAVE
  ;

: OCT6 ( -- )    6 set.octave ;
: OCT5 ( -- )    5 set.octave ;
: OCT4 ( -- )    4 set.octave ;
: OCT3 ( -- )    3 set.octave ;
: OCT2 ( -- )    2 set.octave ;
: OCT1 ( -- )    1 set.octave ;

\ set octave 3
: OCT! ( -- )
    3 to OCTAVE
  ;

\ decrement octave
: OCT- ( -- )
    -1 +to OCTAVE
  ;

\ increment octave
: OCT+ ( -- )
    1 +to OCTAVE
  ;

\ select note in interval 1..12
: get.note ( n[1..12] -- )
    1- OCTAVE 1+ 12 * +  cell *     \ calc. offset in NOTES array
    NOTES + @                       \ fetch frequency of selected note
  ;


vocabulary music
music definitions
music also

\ set duration of a whole note
: o  ( -- )
    WHOLE_NOTE_DURATION to duration
  ;

\ set duration of a white note
: o|  ( -- )
    WHOLE_NOTE_DURATION 2/ to duration
  ;

\ set duration of a black note
: .|  ( -- )
    WHOLE_NOTE_DURATION 2/ 2/ to duration
  ;

\ set duration of a half black note
: .|'  ( -- )
    WHOLE_NOTE_DURATION 2/ 2/ 2/ to duration
  ;

\ set duration of a quarter black note
: .|"  ( -- )
    WHOLE_NOTE_DURATION 2/ 2/ 2/ 2/ to duration
  ;

ledc
\ sustain note in interval [0..100]
: sustain.note ( -- )
    duration SUSTAIN 100 */ ms
    CHANNEL0 0 ledcWriteTone drop
    duration 100 SUSTAIN - 100 */ ms
  ;


: create-note
    \ compile position in octave
    create      ( position -- )
        ,
    \ get note frequency in current octave
    does>
        @ 1- get.note
        CHANNEL0 swap ledcWriteTone drop
        sustain.note
  ;

\ notes in english notation
 1 create-note C
 2 create-note C#
 3 create-note D
 4 create-note D#
 5 create-note E
 6 create-note F
 7 create-note F#
 8 create-note G
 9 create-note G#
10 create-note A
11 create-note A#
12 create-note B

\ notes in french notation
 1 create-note DO
 2 create-note DO#
 3 create-note RE
 4 create-note RE#
 5 create-note MI
 6 create-note FA
 7 create-note FA#
 8 create-note SOL
 9 create-note SOL#
10 create-note LA
11 create-note LA#
12 create-note SI

: SIL ( -- )
    CHANNEL0 0 ledcWriteTone drop
    duration ms
  ;

forth definitions


<EOF>

