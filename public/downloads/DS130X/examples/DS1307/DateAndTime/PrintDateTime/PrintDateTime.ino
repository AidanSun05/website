/*
   PrintDateTime.ino
   Sketch to print the date and time strings from the RTC.
   Created August 28, 2020
*/

#include <DS130X.h>
#include <Wire.h>

DS1307 rtc;

void setup() {
  // Set time and date
  Wire.begin();
  rtc.setDate(28, 8, 20, FRIDAY); // Friday, 28 August 2020
  rtc.setTime(10, 5, 50);         // 10:05:50
  Serial.begin(9600); // Open serial port
}

void loop() {
  // Read day of week, date, and time strings
  String dow  = rtc.dowAbbr(); // Day of week (abbreviated)
  String date = rtc.dateStr(); // Read date
  String time = rtc.timeStr(); // Read time

  // Print the data in format:
  // dow, date time
  Serial.println(dow + ", " + date + " " + time);
  delay(1000); // Delay one second
}
