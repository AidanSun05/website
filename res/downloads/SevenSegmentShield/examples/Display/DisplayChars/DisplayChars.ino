/*
   DisplayChars.ino
   This sketch displays characters such as letters and dashes on the segments.
   Created on 9 April 2020 by Aidan Sun
*/

#include <SevenSegmentShield.h>
#include <Wire.h>

Display ds;

void setup() {
  Wire.begin();
  ds.begin(); // Initialize display
}

void loop() {
  // Display a dash on segment 1
  ds.writeDigitData(4, 64); // digit number, data to write

  // Display the letters A-C on segments 2-4
  ds.writeDigitData(3, 119); // A
  ds.writeDigitData(2, 124); // B
  ds.writeDigitData(1, 57);  // C
}
