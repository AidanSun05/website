/*
   DrawGraphics.ino
   Sketch to demonstrate drawing basic graphics on the OLED.
   Created July 2, 2020
*/

#include <SSD1306_I2C.h>
#include <Wire.h>

SSD1306 display;

void setup() {
  Wire.begin();
  display.begin(0x3C);

  // Draw 100 pixels in random locations
  for (int i = 0; i < 100; i++) {
    // Coordinates are created with random()
    int x = random(0, 128);
    int y = random(0, 64);

    // Draw the pixel
    display.drawPixel(x, y, HIGH); // Draw pixel at (x, y)
  }

  display.updateDisplay(); // Must be called after graphics methods
  delay(1500); // Delay to let the user see the graphics
  display.clear(); // Clear the display

  // Draw lines
  display.drawLine(0, 0, 70, 50, HIGH);          // With coordinates
  display.drawAngledLine(70, 50, 30, 285, HIGH); // With a coordinate, length, and angle

  display.updateDisplay();
  delay(1500);
  display.clear();

  // Draw rectangles
  display.drawRect(10, 15, 30, 40, HIGH, OUTLINE);          // With top-left coordinates, width, and height
  display.drawRectByCoords(20, 20, 100, 50, HIGH, OUTLINE); // With top-left and bottom-right coordinates
  display.drawRect(30, 30, 10, 15, HIGH, FILL);
  display.drawRectByCoords(60, 30, 80, 50, HIGH, FILL);

  display.updateDisplay();
  delay(1500);
  display.clear();

  // Draw circles with center coordinates and radius
  display.drawCircle(64, 32, 30, HIGH, OUTLINE);
  display.drawCircle(20, 20, 20, HIGH, FILL);

  display.updateDisplay();
  delay(1500);
  display.clear();
  display.updateDisplay();
}

void loop() {}
