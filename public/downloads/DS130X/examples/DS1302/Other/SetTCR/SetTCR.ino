/*
   SetTCR.ino
   Sketch to demonstrate using the DS1302's trickle charge feature.
   Created August 30, 2020

   DS1302 connections:
   CLK to 2
   DAT to 3
   CE  to 4
*/

#include <DS130X.h>

DS1302 rtc(3, 2, 4); // DAT, CLK, CE

void setup() {
  rtc.setWriteProtect(false); // Disable write protect
  rtc.writeRaw(TCR_SET, TCR_1D_2R); // Set TCR to 1 diode and 2kR
}

void loop() {} // No loop
