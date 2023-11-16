\ *********************************************************************
\ ESP32 errors
\    Filename:      esp_err.fs
\    Date:          16 nov 2023
\    Updated:       16 nov 2023
\    File Version:  0.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/esp_err.fs
vocabulary esp_errors
esp_errors definitions

    0 constant ESP_OK
   -1 constant ESP_FAIL
 $101 constant ESP_ERR_NO_MEM
 $102 constant ESP_ERR_INVALID_ARG
 $103 constant ESP_ERR_INVALID_STATE
 $104 constant ESP_ERR_INVALID_SIZE
 $105 constant ESP_ERR_NOT_FOUND
 $106 constant ESP_ERR_NOT_SUPPORTED
 $107 constant ESP_ERR_TIMEOUT
 $108 constant ESP_ERR_INVALID_RESPONSE
 $109 constant ESP_ERR_INVALID_CRC
 $10a constant ESP_ERR_INVALID_VERSION
 $10b constant ESP_ERR_INVALID_MAC
 $10c constant ESP_ERR_NOT_FINISHED
 $10d constant ESP_ERR_NOT_ALLOWED

$3000 constant ESP_ERR_WIFI_BASE
$4000 constant ESP_ERR_MESH_BASE
$6000 constant ESP_ERR_FLASH_BASE
$c000 constant ESP_ERR_HW_CRYPTO_BASE
$d000 constant ESP_ERR_MEMPROT_BASE

: decode_esp_err ( esp_error -- addr len )
    case
        ESP_OK                  of  s" no error"                exit endof
        ESP_FAIL                of  s" failure"                 exit endof
        ESP_ERR_NO_MEM          of  s" Out of memory"           exit endof
        ESP_ERR_INVALID_ARG     of  s" Invalid argument"        exit endof
        ESP_ERR_INVALID_STATE   of  s" Invalid state"           exit endof
        ESP_ERR_INVALID_SIZE    of  s" Invalid size"            exit endof
        ESP_ERR_NOT_FOUND       of  s" Requested resource not found" exit endof
        ESP_ERR_NOT_SUPPORTED   of  s" Operation or feature not supported" exit endof
        ESP_ERR_TIMEOUT         of  s" Operation timed out"     exit endof
        ESP_ERR_INVALID_RESPONSE of s" Received response was invalid" exit endof
        ESP_ERR_INVALID_CRC     of  s" CRC or checksum was invalid" exit endof
        ESP_ERR_INVALID_VERSION of  s" Version was invalid"     exit endof
        ESP_ERR_INVALID_MAC     of  s" MAC address was invalid" exit endof
        ESP_ERR_NOT_FINISHED    of  s" Operation has not fully completed" exit endof
        ESP_ERR_NOT_ALLOWED     of  s" Operation is not allowed" exit endof

        ESP_ERR_WIFI_BASE       of  s" Starting number of WiFi error codes"     exit endof
        ESP_ERR_MESH_BASE       of  s" Starting number of MESH error codes"     exit endof
        ESP_ERR_FLASH_BASE      of  s" Starting number of flash error codes"     exit endof
        ESP_ERR_HW_CRYPTO_BASE  of  s" Starting number of HW cryptography module error codes"     exit endof
        ESP_ERR_MEMPROT_BASE    of  s" Starting number of Memory Protection API error codes"     exit endof
        drop -1 -1 of s" ** Unlisted error **" endof
    endcase
  ;

forth definitions

0 value NODEBUG

: .esp_error ( error -- )
    NODEBUG 0= if
        space
        [ esp_errors ] decode_esp_err [ forth ] type cr
    else
        drop
    then
  ;    

<EOF>

