# midicompass

resources needed to make an arduino based status monitor for dj projects (deck volume level meter, fx assignment, crossfader state, etc.)

my first project in arduino, so collecting snippets here, feel free to help out with hints

## libraries to build on

* https://github.com/FastLED/FastLED
* https://github.com/FortySevenEffects/arduino_midi_library

## firmware

to make the controller appear as a MIDI device

### HIDUINO

* https://github.com/ddiakopoulos/hiduino

## software necessary

for MIDI over USB (serial)

### Serial MIDI

* http://www.spikenzielabs.com/SpikenzieLabs/Serial_MIDI.html

## additional

rotary encoder /w push button and a capacitive touch area is to be added after the MIDI CC value to LED matrix is working

* playground.arduino.cc/Code/CapacitiveSensor
