#include <Keypad.h>
#include <SevenSegmentShield.h>
#include <Wire.h>

#define BUZZER 12

// Shield setup
Display ds;
// End of shield setup

// Keypad setup (necessary)
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};

byte rowPins[ROWS] = { 8, 9, 10, 11 };
byte colPins[COLS] = { 2, 4, 7 };
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
// End of keypad setup

String timeStr = "0000"; // Time as a string
int t = 0;               // The timer value as int

void setup() {
  Wire.begin(); // Init I2C
  ds.begin(); // Init display
  pinMode(BUZZER, OUTPUT);
  ds.displayInt(0); // Set all digits to 0
}

void loop() {
  char ch = readKeypad();  // Read the keypad
  if (isDigit(ch)) {
    timeStr = timeStr.substring(1); // Remove first digit
    timeStr += ch;                  // Add new digit to end
    t = timeStr.toInt();            // Turn the string to int
  }
  else if (ch == '*') {
    // Clear the time
    timeStr = "0000";
    t = 0;
  }
  else if (ch == '#') {
    // Start the timer
    for ( ; t > -1; t--) {
      ds.displayInt(t); // Display the time
      delay(1000);
    }
    t = 0;
    alarm();
    timeStr = "0000"; // Reset the string
  }
  ds.displayInt(t); // Display the time
}

void alarm() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER, HIGH);
    delay(500);
    digitalWrite(BUZZER, LOW);
    delay(500);
  }
}

char readKeypad() {
  return keypad.getKey();
}

void waitForKeys() {
  char pressed = readKeypad();
  char key = keypad.getKey();
  while (pressed != key);
}
