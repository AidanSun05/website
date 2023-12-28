// Include the KeyboardController library
#include <KeyboardController.h>

// Initialize USB Controller
USBHost usb;

// Attach Keyboard controller to USB
KeyboardController keyboard(usb);

void setup() {
  Serial.begin(9600); // open a serial port
}

void loop() {
  usb.Task();
}

void keyPressed() {
  // Get the ASCII value of the key pressed.
  int key = keyboard.getKey();
  if (key != 19) {
    // Pressed key is not 'Enter'
    Serial.write(key);
  }
  else {
    // Enter key pressed, start a new line
    Serial.println();
  }
}
