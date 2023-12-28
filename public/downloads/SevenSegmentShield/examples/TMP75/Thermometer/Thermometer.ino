/*
   Thermometer.ino
   This sketch reads the temperature from the TMP75 and displays it on the segments.
   It will also change the color of the LED based on the temperature.
   Created on 10 April 2020 by Aidan Sun
*/

#include <SevenSegmentShield.h>
#include <Wire.h>

Display ds;
RgbLed led;
TMP75 tmp;

void setup() {
  Wire.begin();
  ds.begin();
  tmp.begin();
  Serial.begin(9600);
}

void loop() {
  // Get the temperature
  float temp = tmp.read();

  // Display the temperature
  ds.displayInt(round(temp));
  Serial.print("Temperature: ");
  Serial.println(temp);

  // Change the color of the LED based on the temperature
  if (temp <= 25) led.displayRGBColor(0, 0, 255); // Cold, LED will be blue
  else if (temp > 25 && temp <= 30) led.displayRGBColor(255, 127, 0); // Warm, LED will be yellow
  else led.displayRGBColor(255, 0, 0); // Hot, LED will be red

  delay(500); // Delay at end of loop
}
