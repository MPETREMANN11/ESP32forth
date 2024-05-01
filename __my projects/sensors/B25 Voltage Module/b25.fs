\ *********************************************************************
\ B25 Voltage Sensor
\    Filename:      b25.fs
\    Date:          05 apr. 2024
\    Updated:       09 apr. 2024
\    File Version:  0.0
\    MCU:           ESP32 all models
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     ---
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/b25.fs

\ set GPIOs used by B25 modules, values set in config.fs
0 value B25_NUM_1
0 value B25_NUM_2

\ init GPIOs used by B25 modules
: B25.init ( -- )
    B25_NUM_1 INPUT pinmode
    B25_NUM_2 INPUT pinmode
  ;

\ set divisor ratio of resistors R1 and R2
 7500e fconstant RES_1      \ value in Ohms for R1
30000e fconstant RES_2      \ value in Ohms for R2
RES_1 RES_2 RES_1 F+ F/ fconstant RES_DIVISOR

fvariable Vref              \ Référence de tension pour ADC
    3.3e Vref SF!
fvariable RES               \ RESolution pour la conversion ADC
    4095e RES SF!

fvariable Vout_1            \ final Voltage after calculation for B25_NUM_1
fvariable Vout_2            \ final Voltage after calculation for B25_NUM_2

\ Calculate Vout from ADC value
: Vout.calc ( ADCval -- R:Vout )
    s>f Vref SF@ F*  RES SF@ F/ 
    RES_DIVISOR F/
    \ FDUP F.
  ;

\ get voltage from GPIO, must be B25_NUM_1 or B25_NUM_2
\ result stored in Vout_1 or Vout_2
: getVoltage { gpio -- }
    gpio analogread Vout.calc
    gpio case
        B25_NUM_1 of  Vout_1 SF!   endof
        B25_NUM_2 of  Vout_2 SF!   endof
    endcase
  ;


<EOF>
