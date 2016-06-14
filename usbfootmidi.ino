#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
#define LED 13 // LED pin on Arduino board
#define switch1 8 // 1st Switch
#define switch2 9 // 2nd Switch
#define MIDI_COMMAND_CONTROL_CHANGE 0xB0

//Variables
int switch1LastState = 0;
int switch1CurrentState = 0;
int switch2LastState = 0;
int switch2CurrentState = 0;

//Potentiometer controlls (Analog)
int volumePot = A3;
int volumePrev = 0;
int volumeCurrent = 0;
int levelPot = A4;
int levelPrev = 0;
int levelCurrent = 0;
int wahPot = A2;
int wahLevelPrev = 0;
int wahLevelCurrent = 0;

// the format of the message to send Via serial 
typedef union {
struct {
uint8_t command;
uint8_t channel;
uint8_t data2;
uint8_t data3;
} msg;
uint8_t raw[4];
} t_midiMsg;

void blinkLed(byte num) { // Basic blink function
for (byte i=0;i<num;i++) {
digitalWrite(LED,HIGH);
delay(100);
digitalWrite(LED,LOW);
delay(100);
}
}

void setup() {
pinMode(LED, OUTPUT);
pinMode(switch1, INPUT);
pinMode(switch2, INPUT);
pinMode(volumePot, INPUT);
pinMode(levelPot, INPUT);
pinMode(wahPot, INPUT);
lcd.clear();
lcd.print("ranDumb-MIDI"); // Starting message
delay(5000);
Serial.begin(115200);
blinkLed(3); // Setup sucess indicator
}

void loop() {

t_midiMsg midiMsg; // MIDI message for Switch 1
switch1CurrentState = digitalRead(switch1); // reading Switch 1
switch2CurrentState = digitalRead(switch2); // reading Switch 2

// SWITCH 1 CONTROL START 
if (switch1CurrentState == 1){ // If Switch is true 
if(switch1LastState == 0){ // If Switch1 was not pressed continously 

midiMsg.msg.command = MIDI_COMMAND_CONTROL_CHANGE; // this sends Channel info along with Control Change message 
midiMsg.msg.channel = 1; // Data 1 
midiMsg.msg.data2 = 1; // Data 2
midiMsg.msg.data3 = 0; 
lcd.clear();
lcd.print("sw1 Msg1"); 
/* Send MIDI Message */
Serial.write(midiMsg.raw, sizeof(midiMsg)); 

blinkLed(2); }
}
if (switch1CurrentState == 0){ // If Switch1 was false
if(switch1LastState == 1){ 

midiMsg.msg.command = MIDI_COMMAND_CONTROL_CHANGE; // this sends Channel info along with Control Change message 
midiMsg.msg.channel = 1; // Data Not Channel
midiMsg.msg.data2 = 1; // Velocity 
midiMsg.msg.data3 = 127; /* Velocity */
lcd.clear();
lcd.print("sw1 Msg2"); 
/* Send note on */
Serial.write(midiMsg.raw, sizeof(midiMsg)); 

blinkLed(2); }
}

switch1LastState = switch1CurrentState;
// SWITCH 1 CONTROL END
// SWITCH 2 CONTROL START
if (switch2CurrentState == 1){
if(switch2LastState == 0){
midiMsg.msg.command = MIDI_COMMAND_CONTROL_CHANGE;
midiMsg.msg.channel = 1; // Data
midiMsg.msg.data2 = 2; // Velocity
midiMsg.msg.data3 = 0; /* Velocity */

lcd.clear();
lcd.print("Sw2 Msg1 ");
/* Send note on */
Serial.write(midiMsg.raw, sizeof(midiMsg)); 
blinkLed(2); }
} 
// SWITCH 2 Second State Control 
if (switch2CurrentState == 0){
if(switch2LastState == 1){
midiMsg.msg.command = MIDI_COMMAND_CONTROL_CHANGE;
midiMsg.msg.channel = 1; 
midiMsg.msg.data2 = 2;
midiMsg.msg.data3 = 127; /* Velocity */

lcd.clear();
lcd.print("Sw2 Msg2 ");
/* Send note on */
Serial.write(midiMsg.raw, sizeof(midiMsg)); 
blinkLed(2); }
} 
switch2LastState = switch2CurrentState;

// SWITCH2 CONTROL END

volumeCurrent = analogRead(volumePot)/8;
// levelCurrent = analogRead(levelPot)/8;
wahLevelCurrent = analogRead(wahPot)/8;

   if(volumePrev != volumeCurrent)
{
midiMsg.msg.command = MIDI_COMMAND_CONTROL_CHANGE;
midiMsg.msg.channel = 1;
midiMsg.msg.data2 = 7;
midiMsg.msg.data3 = volumeCurrent; // Velocity 

          Serial.write(midiMsg.raw, sizeof(midiMsg)); 
blinkLed(2); 
// Printing Switch number and data MIDI data that is sent to the LCD screen
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Volume");
lcd.setCursor(0,1);
lcd.print(volumeCurrent);
volumePrev = volumeCurrent;

blinkLed(4); 
}// End Of Volume Control Loop
/* // I don't have the Level control so I just Quoted out this part of the code . 
//Level control loop

if(levelPrev != levelCurrent)
{
midiMsg.msg.command = MIDI_COMMAND_CONTROL_CHANGE;
midiMsg.msg.channel = 1;
midiMsg.msg.data2 = 0x0C;
midiMsg.msg.data3 = levelCurrent; 

Serial.write(midiMsg.raw, sizeof(midiMsg)); 
blinkLed(2); 
// Printing Switch number and data MIDI data that is sent to the LCD screen
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Level");
lcd.setCursor(0,1);
lcd.print(levelCurrent);
levelPrev = levelCurrent;

blinkLed(4); 
}// End Of Level Control Loop

*/ 
// Wah Control Loop
if(wahLevelPrev != wahLevelCurrent)
{
midiMsg.msg.command = MIDI_COMMAND_CONTROL_CHANGE;
midiMsg.msg.channel = 1;
midiMsg.msg.data2 = 0x0B;
midiMsg.msg.data3 = wahLevelCurrent; /* Velocity */

          /* Send note on */
Serial.write(midiMsg.raw, sizeof(midiMsg)); 
blinkLed(2); 
// Printing Switch number and data MIDI data that is sent to the LCD screen
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Wah");
lcd.setCursor(0,1);
lcd.print(wahLevelCurrent);
wahLevelPrev = wahLevelCurrent;

blinkLed(4); 
}// End Of wah Control Loop
}
