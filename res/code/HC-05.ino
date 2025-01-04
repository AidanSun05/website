#include <SoftwareSerial.h> // Include library

SoftwareSerial bt(2, 3); // RX, TX

void setup() {
  bt.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);   // Set built-in LED pin as output. LED_BUILTIN is equal to 13 on most boards,
  // it changes depending on the board you select, since not all boards have the LED on 13.
  digitalWrite(LED_BUILTIN, LOW); // LED off on start

  bt.println("Enter '0' to turn off LED, enter '1' to turn on"); // Send instructions
}

void loop() {
  if (bt.available()) {
    char ch = bt.read(); // Read incoming command
    if (ch == '0') {
      // Turn off LED
      bt.println("LED Off");
      digitalWrite(LED_BUILTIN, LOW);
    }
    else if (ch == '1') {
      // Turn on LED
      bt.println("LED On");
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
}
