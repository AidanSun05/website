#include <LiquidCrystal.h> // Include library

// Define LCD pins
#define RS 8
#define EN 9
#define D4 4
#define D5 5
#define D6 6
#define D7 7

// Define analog input pin
#define ANALOG_PIN A0

// Create LiquidCrystal object
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

void setup() {}

void loop() {
  // Read the analog pin
  String buttonPressed = "none  ";
  int reading = analogRead(ANALOG_PIN);

  // Interpret the reading based on the value
  // Add spaces at the end of each word to clear the text from the previous longer word.
  if (reading <= 100) {
    buttonPressed = "right ";
  } else if (reading > 100 && reading <= 200) {
    buttonPressed = "up    ";
  } else if (reading > 200 && reading <= 400) {
    buttonPressed = "down  ";
  } else if (reading > 400 && reading <= 600) {
    buttonPressed = "left  ";
  } else if (reading > 600 && reading <= 800) {
    buttonPressed = "select";
  }

  // Show the button pressed
  lcd.setCursor(0, 0);
  lcd.print(buttonPressed);

  delay(2); // Let the ADC settle
}
