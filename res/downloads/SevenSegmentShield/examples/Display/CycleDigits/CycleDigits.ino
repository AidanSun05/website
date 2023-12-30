/*
   CycleDigits.ino
   This sketch displays the digits 0-9 on all segments. After each iteration,
   the decimal point is toggled.
   Created on 8 April 2020 by Aidan Sun
*/

#include <SevenSegmentShield.h>
#include <Wire.h>

Display ds;
bool isDpOn = false; // Decimal point off at start

void setup() {
  Wire.begin();
  ds.begin();
}

void loop() {
  // Outer loop to cycle through all numbers
  for (int i = 0; i < 10; i++) {
    // Inner loop to cycle through all 4 digits
    for (int j = 1; j < 5; j++) ds.writeDigit(j, i, isDpOn); // Digit, value, decimal point

    delay(1000); // 1 second delay between values
  }

  ds.clear(); // Clear display
  delay(1000);

  isDpOn = !isDpOn; // Toggle decimal point
}
