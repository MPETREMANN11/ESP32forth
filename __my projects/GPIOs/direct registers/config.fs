\ *********************************************************************
\ direct registers access
\    Filename:      config.fs
\    Date:          11 nov 2023
\    Updated:       11 nov 2023
\    File Version:  0.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    GNU General Public License
\ *********************************************************************



RECORDFILE /spiffs/config.fs
\ definie LEDs GPIOs
25 constant ledRED
26 constant ledYELLOW
27 constant ledGREEN


\ GPIO 0-31 output register R/W
$3FF44004 defREG: GPIO_OUT_REG

\ GPIO 0-31 output register_W1TS WO
$3FF44008 defREG: GPIO_OUT_W1TS_REG 

\ GPIO 0-31 output register_W1TC WO
$3FF4400C defREG: GPIO_OUT_W1TC_REG

\ GPIO 32-39 output register  R/W
$3FF44010 defREG: GPIO_OUT1_REG

\ GPIO 32-39 output bit set register WO
$3FF44014 defREG: GPIO_OUT1_W1TS_REG

\ GPIO 32-39 output bit clear register WO
$3FF44018 defREG: GPIO_OUT1_W1TC_REG
 
\ GPIO 0-31 output enable register R/W
$3FF44020 defREG: GPIO_ENABLE_REG

\ GPIO 0-31 output enable register_W1TS WO
$3FF44024 defREG: GPIO_ENABLE_W1TS_REG

\ GPIO 0-31 output enable register_W1TC WO
$3FF44028 defREG: GPIO_ENABLE_W1TC_REG

\ GPIO 32-39 output enable register R/W
$3FF4402C defREG: GPIO_ENABLE1_REG

\ GPIO 32-39 output enable bit set register WO
$3FF44030 defREG: GPIO_ENABLE1_W1TS_REG

\ GPIO 32-39 output enable bit clear register WO
$3FF44034 defREG: GPIO_ENABLE1_W1TC_REG 


<EOF>



