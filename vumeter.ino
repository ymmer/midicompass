//  https://blog.blinkenlight.net/experiments/basic-effects/vu-meter/
//  www.blinkenlight.net
//
//  Copyright 2011 Udo Klein
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program. If not, see http://www.gnu.org/licenses/
 
 
#include <MsTimer2.h>
 
volatile uint8_t current_volume = 0;
volatile uint8_t current_top_volume = 0;
volatile uint32_t speed = 0;
volatile uint32_t height = 0;
 
 
void drop() {
    if (current_volume < current_top_volume) {
        // volume decreased recently
        // ensure top_volume LED is lit
        digitalWrite(current_top_volume+1, HIGH);
 
        // now let the top_volume indicator follow down
        ++speed;
        height += speed;
         
        if (height > 20000) {
            height-= 20000;
            digitalWrite(current_top_volume+1, LOW);
            --current_top_volume;
            digitalWrite(current_top_volume+1, HIGH);
        }
    }
}
 
void set_volume(uint8_t volume) {
    cli();
    current_volume = volume;
 
    if (current_volume >= current_top_volume) {
        current_top_volume = current_volume;
        speed = 0;
        height = 0;
    }
 
    for (uint8_t pin=2; pin<20; ++pin) {
        digitalWrite(pin, pin < current_volume+2);
    }
    sei();
}
 
void setup() {
    for (uint8_t pin=2; pin<20; ++pin) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
    Serial.begin(9600);
    Serial.println("ready, send characters a-s to control output");
    set_volume(0);
 
    MsTimer2::set(1, drop);
    MsTimer2::start();
}
 
void loop() {
    uint8_t volume = Serial.read() - 'a';
    if (volume < 't'-'a') {
        set_volume(volume);
    }
}
