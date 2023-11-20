\ *********************************************************************
\ ESP32 errors
\    Filename:      esp_err.fs
\    Date:          19 nov 2023
\    Updated:       20 nov 2023
\    File Version:  2.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/esp_err.fs
vocabulary esp_errors
esp_errors definitions

structures 
struct ERRORS_CODES
    ptr field >errorZstring 
    ptr field >errorCode
esp_errors
 
0 value CODE_ERROR

: errorCode: ( comp:err zstr -- <name> | exec: err -- addr len )
    create , ,
    does> >R
        r@ >errorCode @ =
        if
            r@ >errorZstring @ to CODE_ERROR
        then
        rdrop
  ;

\ example: 
\ 5 z" test error" errorCode: te


    0 z" no error"                      errorCode: ESP_OK
   -1 z" failure"                       errorCode: ESP_FAIL
 $101 z" Out of memory"                 errorCode: ESP_ERR_NO_MEM
 $102 z" Invalid argumen"               errorCode: ESP_ERR_INVALID_ARG
 $103 z" Invalid state"                 errorCode: ESP_ERR_INVALID_STATE
 $104 z" Invalid size"                  errorCode: ESP_ERR_INVALID_SIZE
 $105 z" Requested resource not found"  errorCode: ESP_ERR_NOT_FOUND
 $106 z" Operation or feature not supported" 
                                        errorCode: ESP_ERR_NOT_SUPPORTED
 $107 z" Operation timed out"           errorCode: ESP_ERR_TIMEOUT
 $108 z" Received response was invalid" errorCode: ESP_ERR_INVALID_RESPONSE
 $109 z" CRC or checksum was invalid"   errorCode: ESP_ERR_INVALID_CRC
 $10a z" Version was invalid"           errorCode: ESP_ERR_INVALID_VERSION
 $10b z" MAC address was invalid"       errorCode: ESP_ERR_INVALID_MAC
 $10c z" Operation has not fully completed" 
                                        errorCode: ESP_ERR_NOT_FINISHED
 $10d z" Operation is not allowed"      errorCode: ESP_ERR_NOT_ALLOWED
$3000 z" Starting number of WiFi error code" 
                                        errorCode: ESP_ERR_WIFI_BASE
$4000 z" Starting number of MESH error codes" 
                                        errorCode: ESP_ERR_MESH_BASE
$6000 z" Starting number of flash error codes" 
                                        errorCode: ESP_ERR_FLASH_BASE
$c000 z" Starting number of HW cryptography module error codes" 
                                        errorCode: ESP_ERR_HW_CRYPTO_BASE
$d000 z" Starting number of Memory Protection API error codes" 
                                        errorCode: ESP_ERR_MEMPROT_BASE


: decode_esp_err ( esp_error -- )
    dup ESP_OK
    dup ESP_FAIL
    dup ESP_ERR_NO_MEM
    dup ESP_ERR_INVALID_ARG
    dup ESP_ERR_INVALID_STATE
    dup ESP_ERR_INVALID_SIZE
    dup ESP_ERR_NOT_FOUND
    dup ESP_ERR_NOT_SUPPORTED
    dup ESP_ERR_TIMEOUT
    dup ESP_ERR_INVALID_RESPONSE
    dup ESP_ERR_INVALID_CRC
    dup ESP_ERR_INVALID_VERSION
    dup ESP_ERR_INVALID_MAC
    dup ESP_ERR_NOT_FINISHED
    dup ESP_ERR_NOT_ALLOWED
    dup ESP_ERR_WIFI_BASE
    dup ESP_ERR_MESH_BASE
    dup ESP_ERR_FLASH_BASE
    dup ESP_ERR_HW_CRYPTO_BASE
    dup ESP_ERR_MEMPROT_BASE
    drop
  ;

forth definitions
esp_errors

0 value NODEBUG

: .esp_error ( error -- )
    0 to CODE_ERROR
    NODEBUG 0= if
        space
        decode_esp_err
        CODE_ERROR 0<> if
            CODE_ERROR z>s cr type
        else
            cr ." ** no error **" 
        then
    else
        drop
    then
  ;    
forth

<EOF>

