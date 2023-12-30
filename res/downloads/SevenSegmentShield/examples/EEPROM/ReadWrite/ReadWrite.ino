/*
   ReadWrite.ino
   This sketch reads and writes to the 24LC128 on the shield.
   Created on 10 April 2020 by Aidan Sun
*/

#include <SevenSegmentShield.h>
#include <Wire.h>

MemoryChip mc;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  // Write to the first 20 bytes
  for (int i = 0; i < 20; i++) {
    mc.writeByte(i, i); // address, data
    delay(5); // Short delay to prevent losing communication data
  }

  // Read from the first 20 bytes
  for (int i = 0; i < 20; i++) {
    uint8_t value = mc.readByte(i); // Read from the address

    // Print the data in the byte
    Serial.print("EEPROM address ");
    Serial.print(i);
    Serial.print(" holds ");
    Serial.println(value);
  }
}

void loop() {}
