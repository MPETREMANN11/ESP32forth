\ *********************************************************************
\ ASSERT( tool for eForth
\    Filename:      assert.fs
\    Date:          06 dec 2023
\    Updated:       06 dec 2023
\    File Version:  1.1
\    MCU:           ESP32-WROOM-32
\    Forth:         eForth all versions
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

\ inspired from assert( gForth and assert( PHP
\ This version is simpler than that of gForth. Example of use:
\   assert( 1 2 + 3 = )
\   assert( 1 2 + 5 = )
\ The code between "assert(" and ")" is executed only if ASSERT_LEVEL is not zero.
\ If the test result returns 0, the word ) displays the code which generated this test.
\ The execution of:
\   assert( 1 2 + 5 = )
\ display:
\   ASSERT : assert( 1 2 + 5 = ) ERROR


RECORDFILE /spiffs/assert.fs
\ value 0 or -1
-1 value ASSERT_LEVEL

\ temporary pointer to input bufffer
variable assert-start

\ followed by the code to test
: assert(   ( -- )
    \ save current input buffer position
    tib >in @ + assert-start !
    ASSERT_LEVEL 0= if
        POSTPONE (
    then
  ; immediate

\ closes the code to be tested and displays an error message 
\ if the test result is zero
: ) ( fl -- )
    0= if
        cr ." ASSERT : " 
        \ calculate new input buffer position and display code with error
        assert-start @
        tib >in @ + over - 1- type
         -1 throw
    then
  ; immediate
<EOF>

