/*
   DisplayNum.ino
   This sketch displays numbers on the shield with displayInt().
   Created on 9 April 2020 by Aidan Sun
*/

#include <SevenSegmentShield.h>
#include <Wire.h>

Display ds;

void setup() {
  Wire.begin();
  ds.begin();
}

void loop() {
  int number = random(-999, 10000); // Generate a random number from -999 to 9999
  ds.displayInt(number); // Display the number
  delay(1000); // Wait to let the user see the number
}
