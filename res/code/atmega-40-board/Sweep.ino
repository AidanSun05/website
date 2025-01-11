/*
   Sweep.ino
   Sketch to control a servo.
   Created on 18 May 2020 by Aidan Sun

   Circuit: One standard 9g servo connected to D15 of the ATmega 40 Board
*/

#include <Servo.h> // Include the servo library for ATmega 40

Servo s;  // Create a servo object

void setup() {
  s.attach(15);  // Servo is connected to D15
}

void loop() {
  for (int i = 0; i < 181; i++) {
    // Increment angle from 0 to 180
    s.write(i);
    delay(10);
  }

  for (int i = 180; i > -1; i--) {
    // Decrement angle from 180 to 0
    s.write(i);
    delay(10);
  }
}
