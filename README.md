### guitarduino
An arduino guitar tuner for UMass Boston's ENGIN-104 class  
Designed to be run an Arduino Mega2560
  
# Quirks:  
1. The detection logic for how in tune the note is is too fine at higher frequencies, and too coarse at lower ones.
2. It's only configured for standard tuning. (this could easily be changed however)
3. There's some weird noise in bins 0-1, so we exclude them.
