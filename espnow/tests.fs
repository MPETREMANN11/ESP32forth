\ *********************************************************************
\ tests for espnow development
\    Filename:      tests.fs
\    Date:          22 nov. 2025
\    Updated:       23 nov. 2025
\    File Version:  0.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ *********************************************************************

RECORDFILE /spiffs/tests.fs

myMac .mac cr
." - - - - " cr
." MASTER: " MASTER .mac cr
." SLAVE1: " SLAVE1 .mac cr

espnow
SLAVE1 add-peer

\ 1. Définition du callback (l'ordre des arguments dépend de l'implémentation spécifique)
: MY-SEND-CALLBACK ( mac_addr_ptr status_code -- )
    ( Ici, on suppose que status_code est au sommet de la pile, puis mac_addr_ptr )
    DUP 0= IF
        ." ESP-NOW Envoi OK." CR
    ELSE
        ." ESP-NOW Erreur: " . CR
    THEN
    DROP  \ Retire le mac_addr_ptr de la pile
  ;
\ 2. Enregistrement du callback
: INIT-ESP-NOW-SEND-CB ( -- )
    ['] MY-SEND-CALLBACK  \ Met l'adresse du mot sur la pile
    esp_now_register_send_cb \ Appelle le mot (consomme l'adresse, place le code de retour)
    0= IF
        ." Callback d'envoi enregistré avec succès." CR
    ELSE
        ." Échec de l'enregistrement du callback." CR
    THEN
  ;
\ INIT-ESP-NOW-SEND-CB


Oled128x32Init cr
oled.cls
z" my string"       oled.println
z" ET CECI AUSSI"   oled.println
z" PUIS UNE LIGNE PLUS"   oled.println
z" ET LA DERNIERE ligne"   oled.println


<EOF>






