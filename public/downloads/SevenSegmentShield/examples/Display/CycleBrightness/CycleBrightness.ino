/*
   CycleBrightness.ino
   This sketch cycles through the brightness for the segments.
   The possible values are 3mA, 6mA, 9mA, 12mA, 18mA, and 21mA.
   Created on 8 April 2020 by Aidan Sun
*/

#include <SevenSegmentShield.h>
#include <Wire.h>

Display ds;

void setup() {
  Wire.begin();
  ds.begin();
  ds.displayInt(8888); // Display the number 8888
}

void loop() {
  uint8_t brightnessLevels[] = { BRIGHTNESS_3MA, BRIGHTNESS_6MA, BRIGHTNESS_9MA, BRIGHTNESS_12MA, BRIGHTNESS_18MA, BRIGHTNESS_21MA };

  for (int i = 0; i < 6; i++) {
    ds.writeBrightness(brightnessLevels[i]);
    delay(500); // 1/2 second delay
  }
}
