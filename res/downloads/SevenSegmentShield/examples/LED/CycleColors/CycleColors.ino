/*
   CycleColors.ino
   This sketch cycles through 48 colors on the LED.
   Created on 10 April 2020 by Aidan Sun
*/

#include <SevenSegmentShield.h>

RgbLed led;

const long COLORS[48] = {
  0xFF2000, 0xFF4000, 0xFF6000, 0xFF8000, 0xFFA000, 0xFFC000, 0xFFE000, 0xFFFF00,
  0xE0FF00, 0xC0FF00, 0xA0FF00, 0x80FF00, 0x60FF00, 0x40FF00, 0x20FF00, 0x00FF00,
  0x00FF20, 0x00FF40, 0x00FF60, 0x00FF80, 0x00FFA0, 0x00FFC0, 0x00FFE0, 0x00FFFF,
  0x00E0FF, 0x00C0FF, 0x00A0FF, 0x0080FF, 0x0060FF, 0x0040FF, 0x0020FF, 0x0000FF,
  0x2000FF, 0x4000FF, 0x6000FF, 0x8000FF, 0xA000FF, 0xC000FF, 0xE000FF, 0xFF00FF,
  0xFF00E0, 0xFF00C0, 0xFF00A0, 0xFF0080, 0xFF0060, 0xFF0040, 0xFF0020, 0xFF0000
};

const int NUM_OF_COLORS = 48;

void setup() {}

void loop() {
  for (int i = 0; i < NUM_OF_COLORS; i++) {
    long currentColor = COLORS[i];
    led.displayHexColor(currentColor); // Display the color
    delay(100);
  }

  // Turn off the LED
  led.turnOff();
  delay(1000);
}
