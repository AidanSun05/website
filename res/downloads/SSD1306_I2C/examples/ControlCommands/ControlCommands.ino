/*
   ControlCommands.ino
   Sketch to demonstrate various commands to send to the OLED.
   Note: None of these commands affect the display's RAM, so
   updateDisplay() isn't needed.
   Created July 2, 2020
*/

#include <SSD1306_I2C.h>
#include <Wire.h>

SSD1306 display;

void setup() {
  Wire.begin();
  display.begin(0x3C);

  // Display a filled rectangle
  display.drawRect(20, 10, 100, 40, HIGH, FILL);
  display.updateDisplay(); // Show the rectangle

  // Invert the display
  delay(2000);
  display.setInvert(true);  // Inverted display
  delay(2000);
  display.setInvert(false); // Normal display

  // Turn display off/on
  delay(2000);
  display.setState(false);  // Display off (sleep mode)
  delay(2000);
  display.setState(true); // Display on (RAM contents are displayed)

  // Turn all pixels on, then resume display
  delay(2000);
  display.displayAllOn();  // All pixels on, regardless of RAM contents
  delay(2000);
  display.resumeDisplay(); // Display what is in RAM
}

void loop() {}
