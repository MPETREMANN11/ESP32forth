\ *********************************************************************
\ tools for REGISTERS definitions and manipulations
\    Filename:      registersDefinitions.txt
\    Date:          02 apr 2022
\    Updated:       12 nov 2023
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/defRegsMasks.fs
\ define a register, similar as constant
: defREG: ( comp: reg-addr -- <name> | exec: -- reg-addr )
    create
        ,
    does>
        @
  ;

\ define a mask for registers
: defMASK: ( comp: mask0 position -- <name> | exec: -- position mask1 )
    create
        dup ,
        lshift ,
    does>
        dup @
        swap cell + @
  ;

\ display n in bbbbbbbb bbb..... format
: .binDisp ( n -- )
    base @ >r  binary   \ select binary base
    <#                  \ start num formating
    4 for
        aft
            8 for
                aft  #  then
            next
            bl hold     \ add 'space' in number formating
        then
    next
    #> 
    cr space ." 33222222 22221111 11111100 00000000"
    cr space ." 10987654 32109876 54321098 76543210"
    cr type             \ display n in binary format
    r> base !           \ restore current numeric base
  ;

\ display reg content
: .reg ( reg -- )
    @ .binDisp
  ;

\ set mask in addr
: regSet ( val shift mask addr -- )
    [ registers ] m! [ forth ]
  ;

\ test mask in addr
: regTst ( shift mask addr -- val )
    [ registers ] m@ [ forth ]
  ;
<EOF>


\ == EXAMPLES: ======

\ $3FF48484 defREG: RTCIO_PAD_DAC1_REG       \ DAC1 configuration register
\ $3FF48488 defREG: RTCIO_PAD_DAC2_REG       \ DAC2 configuration register
\ $3FF48898 defREG: SENS_SAR_DAC_CTRL1_REG   \ DAC control
\ $3FF4889c defREG: SENS_SAR_DAC_CTRL2_REG   \ DAC output control


\ 1 25 defMASK: mSENS_DAC_CLK_INV

\ enable SW
\ 1 mSENS_SW_TONE_EN SENS_SAR_DAC_CTRL1_REG regSet
\ SENS_SAR_DAC_CTRL1_REG .reg

\ disable SW
\ 0 mSENS_SW_TONE_EN SENS_SAR_DAC_CTRL1_REG regSet
\ SENS_SAR_DAC_CTRL1_REG .reg


