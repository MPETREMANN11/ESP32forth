\ *************************************
\ tests for ESP32 Synth.
\    Filename:      tests.fs
\    Date:          25 jan 2026
\    Updated:       26 jan 2026
\    File Version:  1.0
\    MCU:           ESP32-S3 - ESP32-WROOM
\    Forth:         ESP32forth all versions 7.0.7.21+
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ **************************************


RECORDFILE /spiffs/tests.fs
cr ." Loading tests "

\ OLED display test
oled oledinit 500 ms
oledcls oleddisplay
forth


\ Initialiser le synthétiseur (pin DAC, sample rate)
\ 25 44100 synth.begin

\ Jouer une note (canal, note MIDI, durée ms)
\ 0 60 1000 synth.note

\ Note On (canal, note MIDI)
\ 0 64 synth.noteOn

\ Note Off (canal)
0 synth.noteOff

\ Définir forme d'onde (canal, type: 0=SINE, 1=SQUARE, 2=SAW, 3=TRIANGLE)
\ 0 0 synth.setWaveform

\ Volume (canal, 0-127)
\ 0 100 synth.setVolume

\ Pan (canal, -64 à +63)
\ 0 0 synth.setPan

\ Enveloppe ADSR (canal, attack, decay, sustain, release)
\ 0 50 50 100 200 synth.setEnvelope

\ Filtre (canal, cutoff, resonance)
\ 0 1000 10 synth.setFilter

\ Reverb (enable 0/1, mix 0-127)
\ 1 50 synth.enableReverb

\ Chorus (enable 0/1, depth 0-127)
\ 1 30 synth.enableChorus

\ Mute/Unmute canal
\ 0 synth.mute
\ 0 synth.unmute

\ Arrêter un canal ou tous
\ 0 synth.stop
\ synth.stopAll

<EOF>





