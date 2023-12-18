\ *************************************
\ Manage a Real Time Clock
\    Filename:      RTClock.fs
\    Date:          19 nov 2023
\    Updated:       19 nov 2023
\    File Version:  1.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ **************************************



RECORDFILE /spiffs/RTClock.fs
0 value currentTime

\ store current time
: RTC.setTime { hh mm ss -- }
    hh 3600 *
    mm 60 *
    ss + +  1000 *
    MS-TICKS - to currentTime
  ;

\ fetch current time in seconds
: RTC.getTime ( -- hh mm ss )
    currentTime MS-TICKS + 1000 /
    3600 /mod swap 60 /mod swap
  ;

\ used for SS and MM part of time display
: :## ( n -- n' )
    #  6 base ! #  decimal  [char] : hold
  ;

\ display current time
: RTC.to$ ( -- addr len )
    currentTime MS-TICKS + 1000 /
    <# :## :## 24 mod #S #>
  ;


\ display current time
: .RTC ( -- )
    RTC.to$ type
  ;

<EOF>

