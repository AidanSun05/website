/*
   RAMReadWrite.ino
   Sketch to demonstrate reading and writing the DS1307's RAM.
   Created August 29, 2020
*/

#include <DS130X.h>
#include <Wire.h>

DS1307 rtc;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  // Write to all bytes of RAM
  Serial.println("Writing to RAM");
  int value = 0; // Value to hold in RAM
  for (int i = rtc.ramStart(); i <= rtc.ramEnd(); i++) {
    // ramStart() returns the starting address of RAM.
    // ramEnd() returns the ending address of RAM.
    rtc.write(i, value); // Write to RAM
    value++; // New value for next address
  }

  // Read from all bytes of RAM
  Serial.println("Reading from RAM");
  for (int i = rtc.ramStart(); i <= rtc.ramEnd(); i++) {
    uint8_t ramValue = rtc.read(i); // Read from RAM
    // Print the value in RAM
    Serial.println("Address 0x" + String(i, HEX) + " has value " + String(ramValue));
  }
}

void loop() {} // No loop
