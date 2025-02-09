#include <LiquidCrystal.h> // Include library

// Define pin constants
#define RS 8
#define EN 9
#define D4 4
#define D5 5
#define D6 6
#define D7 7

// Create LiquidCrystal object
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

void setup() {
  lcd.begin(16, 2); // 16 columns, 2 rows

  lcd.setCursor(0, 0); // Set cursor to row 1, column 1
  lcd.print("Hello, world!"); // Print "Hello, World!"
}

void loop() {}
