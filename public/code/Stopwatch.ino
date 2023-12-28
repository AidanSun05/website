/*
  Stopwatch
  Displays numbers on a seven segment display shield like a stopwatch.
  Parts required:
  Arduino Uno
  Seven segment shield
*/

#include <SevenSegmentShield.h> // Include shield library
#include <Wire.h> // Include Wire library for I2C

Display ds;    // Create Display object to use
int count = 0; // The variable to display

void setup() {
  Wire.begin(); // Init I2C bus
  ds.begin();   // Init display
}

void loop() {
  ds.displayInt(count++); // Increment count and display on segments
  delay(1000);            // Wait one second before continuing
}
