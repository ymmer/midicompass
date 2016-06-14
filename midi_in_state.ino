// midi_in_state.ino
// version 2015-06-01
// Arduino MIDI tutorial
// by Staffan Melin
// http://libremusicproduction.com/
// libraries

#include <SoftwareSerial.h>

// We will use the SoftwareSerial library instead of the Serial library, as this will let us control which pins our MIDI interface is connected to.
SoftwareSerial mySerial(2, 3); // RX, TX

// constants

#define PIN_LED 13

// MIDI commands
#define MIDI_NOTE_ON 144
#define MIDI_NOTE_OFF 128

// filter MIDI events on channel and note number
const int filterChannel = 0; // MIDI channel 1
const int filterNote = 60; // middle C

// states
#define STATE_NONE 0
#define STATE_NOTE_ON 1
#define STATE_NOTE 2
int state;

// keep action (LED) going for actionDuration milliseconds
bool actionOn;
unsigned long actionTime;
unsigned long actionDuration = 100;

// received MIDI data
byte midiByte;
byte midiChannel;
byte midiCommand;
byte midiNote;
byte midiVelocity;

void setup() {

    mySerial.begin(31250);
    delay(100);

    pinMode(PIN_LED, OUTPUT);
    
    state = STATE_NONE;
    actionOn = false;    
}

void loop () {

    // Is there any MIDI waiting to be read?

    if (mySerial.available() > 0) {

        // read MIDI byte

        midiByte = mySerial.read();

        switch (state) {

        case STATE_NONE:
        
            // remove channel info
            midiChannel = midiByte & B00001111;
            midiCommand = midiByte & B11110000;

            if (midiChannel == filterChannel)
            {
                if (midiCommand == MIDI_NOTE_ON)
                {
                    state = STATE_NOTE_ON;
                }
            }
            break;

        case STATE_NOTE_ON:
            midiNote = midiByte;
            state = STATE_NOTE;
            break;
            
        case STATE_NOTE:
            midiVelocity = midiByte;
            state = STATE_NONE;
            
            if (midiNote == filterNote && midiVelocity > 0)
            {
                digitalWrite(PIN_LED, HIGH);
                actionTime = millis();
                actionOn = true;
            }
            
            break;
            
        } // switch

    } // mySerial.available()

    // turn action off after predetermined time

    if (actionOn)
    {
        if ((millis() - actionTime) > actionDuration)
        {
            actionOn = false;
            digitalWrite(PIN_LED, LOW);
        }
    }    

} // loop
