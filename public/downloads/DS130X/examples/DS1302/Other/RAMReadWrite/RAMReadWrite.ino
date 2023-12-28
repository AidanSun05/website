/*
   RAMReadWrite.ino
   Sketch to demonstrate reading and writing the DS1307's RAM.
   Created August 30, 2020

   DS1302 connections:
   CLK to 2
   DAT to 3
   CE  to 4
*/

#include <DS130X.h>

DS1302 rtc(3, 2, 4); // DAT, CLK, CE

void setup() {
  rtc.setWriteProtect(false);
  Serial.begin(9600);

  // Write to all bytes of RAM
  Serial.println("Writing to RAM");
  int value = 0; // Value to hold in RAM
  for (int i = rtc.ramWStart(); i <= rtc.ramWEnd(); i += 2) {
    // Add 2 to counter to skip every other address (the read address)
    // ramWStart() returns the starting address for writing RAM.
    // ramWEnd() returns the ending address for writing RAM.
    rtc.write(i, value); // Write to RAM
    value++; // New value for next address
  }

  // Read from all bytes of RAM
  Serial.println("Reading from RAM");
  for (int i = rtc.ramRStart(); i <= rtc.ramREnd(); i += 2) {
    // Add 2 to counter to skip every other address (the write address)
    // ramRStart() returns the starting address for reading RAM.
    // ramREnd() returns the ending address for reading RAM.
    uint8_t ramValue = rtc.read(i); // Read from RAM
    // Print the value in RAM
    Serial.println("Address 0x" + String(i, HEX) + " has value " + String(ramValue));
  }
}

void loop() {} // No loop
