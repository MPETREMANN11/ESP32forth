\ *********************************************************************
\ main for espnow development
\    Filename:      main.fs
\    Date:          22 nov. 2025
\    Updated:       21 dec. 2025
\    File Version:  0.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ *********************************************************************






\ Variables pour le fichier
variable fd
128 23 * 8 / constant IMG_SIZE

\ Espace tampon pour les pixels (16 octets * 23 lignes)
create img-buffer 368 allot 

: load-bmp ( -- )
  s" /spiffs/image.bmp" r/o open-file throw fd !
  
  \ Sauter l'entête (Header + DIB + Palette = 62 octets en général)
  \ À ajuster selon votre fichier spécifique
  62 s>d fd @ repurpose-file-position \ Se placer au début des pixels
  
  \ Lire les données
  img-buffer IMG_SIZE fd @ read-file throw drop
  fd @ close-file throw
;


