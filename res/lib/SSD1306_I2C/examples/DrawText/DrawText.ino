/*
   DrawText.ino
   Sketch to demonstrate drawing text on the OLED.
   Created July 2, 2020
*/

#include <SSD1306_I2C.h>
#include <Wire.h>

SSD1306 display;

void setup() {
  Wire.begin();
  display.begin(0x3C);

  // Print strings
  display.println("Hello World!", HIGH);
  display.print("abcdefg", HIGH);

  display.updateDisplay();
  delay(2000);
  display.clear();

  // Print inverted text
  display.drawRect(10, 10, 80, 30, HIGH, FILL);
  display.setTextSize(2);
  display.setCursor(15, 20);
  display.print("Invert", LOW);

  display.updateDisplay();
  delay(2000);
  display.clear();

  // Print with various text sizes and cursor locations
  display.setTextSize(2);  // Characters are now twice as big
  display.setCursor(0, 0); // Top-left corner of screen
  display.print("abcd\tef", HIGH);
  display.setCursor(20, 30);
  display.print("1234", HIGH);

  display.updateDisplay();
  delay(2000);
  display.clear();

  // Chars and numbers can also be printed:
  display.setCursor(0, 0); // Cursor to top-left corner
  display.setTextSize(1);  // Small chars (5px by 7px)

  // Print a char
  display.print("Char: ", HIGH);
  display.println('a', HIGH); // println() moves cursor to next line

  // Print an integer
  display.print("Int: ", HIGH);
  display.println(32767, HIGH);

  // Print a long
  display.print("Long: ", HIGH);
  display.println(2147483647, HIGH);

  // Print a float
  display.print("Float: ", HIGH);
  display.println(123.45, HIGH);

  // Print a float, displayed to # of decimals
  display.println("Float to 3 decimals:", HIGH);
  display.println(123.4567, HIGH, 3);

  display.updateDisplay();
  delay(2000);
  display.clear();

  // Print all standard ASCII chars
  display.setCursor(0, 0); // Reset cursor

  for (int i = 0; i < 94; i++) {
    display.print((char)(i + 33), HIGH);

    // 20 chars (with text size 1) fit on one line, so make a new line if needed
    if (((i % 20) == 0) && (i != 0)) display.println();
  }

  display.updateDisplay();
}

void loop() {}
