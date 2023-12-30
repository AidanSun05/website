/*
   ScrollDisplay.ino
   Sketch to demonstrate scrolling on the OLED.
   Created July 2, 2020
*/

#include <SSD1306_I2C.h>
#include <Wire.h>

SSD1306 display;

void setup() {
  Wire.begin();
  display.begin(0x3C);

  // Print the word "scroll" three times
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("scroll", HIGH);
  display.println("scroll", HIGH);
  display.println("scroll", HIGH);
  display.updateDisplay();

  // Horizontal scroll left, scroll pages 0-7, 2 frames interval
  display.setupScrollH(LEFT, 0, 7, FRAMES_2);
  display.startScroll(); // Begin scroll
  delay(2000);           // Scroll for 2 seconds
  display.stopScroll();  // Stop scrolling
  delay(500);            // Wait half a second between scrolling

  // Horizontal scroll right, scroll pages 0-7, 2 frames interval
  display.setupScrollH(RIGHT, 0, 7, FRAMES_2);
  display.startScroll();
  delay(2000);
  display.stopScroll();
  delay(500);

  // Vertical+horizontal scroll left and up, scroll pages 0-7, 2 frames interval
  display.setupScrollHV(LEFT, UP, 0, 7, FRAMES_2);
  display.startScroll();
  delay(2000);
  display.stopScroll();
  delay(500);

  // Vertical+horizontal scroll right and down, scroll pages 0-7, 2 frames interval
  display.setupScrollHV(RIGHT, DOWN, 0, 7, FRAMES_2);
  display.startScroll();
  delay(2000);
  display.stopScroll();
  delay(500);

  // Only scroll first page
  display.clear();
  display.setCursor(0, 0);
  display.println("scroll", HIGH);
  display.print("no scroll", HIGH);
  display.updateDisplay();
  delay(500);

  // Horizontal scroll left, scroll pages 0-1, 2 frames interval
  display.setupScrollH(LEFT, 0, 1, FRAMES_2);
  display.startScroll();
  delay(2000);
  display.stopScroll();
}

void loop() {}
