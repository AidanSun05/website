/*
   I2CScanner.ino
   This program looks for I2C devices connected to the Arduino and prints them in a grid.

   If a device is present, its address will show up in the corresponding row/column.
   Ex. For a device with address 0x48, "48" will show up in row 4 and column 8.

   Created on 16 May 2020 by Aidan Sun
*/

#include <Wire.h>

void setup() {
  // Initialize Wire and Serial
  Wire.begin();
  Serial.begin(9600);

  // Start of grid
  Serial.print("\n    ");

  // Print the grid's top row
  for (int i = 0; i < 16; i++) {
    Serial.print(i, HEX);
    Serial.print("  ");
  }

  Serial.println(); // Newline

  int numOfDevices = 0; // Number of I2C devices detected

  for (byte i = 0; i < 8; i++) {
    // Address tens place 0-7
    Serial.print(String(i) + "0 ");
    for (byte j = 0; j < 16; j++) {
      // Address ones place 0-F
      byte addr = (i * 16) + j; // Concatenate tens and ones to get address

      // Try to start communications with the device and get the status code.
      Wire.beginTransmission(addr);
      byte error = Wire.endTransmission();

      if (error == 0) {
        // If status code is 0, there is a device
        numOfDevices++; // Increment the number of devices
        if (addr < 16) Serial.print("0"); // Add a leading zero if address is < 16
        Serial.print(addr, HEX); // Print the hex address of the device, meaning it's there.
      }
      else {
        // Unknown or no device
        Serial.print("--"); // Print a dash in the device's location
      }
      Serial.print(" "); // Trailing whitespace so addresses are not overlapping
    }
    Serial.println(); // Newline
  }

  // Print number of devices
  if (numOfDevices == 1) {
    Serial.print("\nThere is 1 I2C device.\n");
  }
  else {
    Serial.print("\nThere are " + String(numOfDevices) + " I2C devices.\n");
  }
  Serial.println("Press Reset to scan again.");
}

void loop() {} // No loop
