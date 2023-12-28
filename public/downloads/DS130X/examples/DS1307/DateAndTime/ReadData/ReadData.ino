/*
   ReadData.ino
   Sketch to read unformatted time data from the RTC.
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
  // Read time from RTC
  uint8_t hours   = rtc.read(HOURS);
  uint8_t minutes = rtc.read(MINUTES);
  uint8_t seconds = rtc.read(SECONDS);

  Serial.println("Hour: "   + String(hours));
  Serial.println("Minute: " + String(minutes));
  Serial.println("Second: " + String(seconds));

  Serial.println();

  // Read date from RTC
  uint8_t dayOfWeek  = rtc.read(DAY_OF_WEEK);
  uint8_t dayOfMonth = rtc.read(DAY_OF_MONTH);
  uint8_t month      = rtc.read(MONTHS);
  uint8_t year       = rtc.read(YEARS);

  Serial.println("Day of week: " + String(dayOfWeek));
  Serial.println("Day: "         + String(dayOfMonth));
  Serial.println("Month: "       + String(month));
  Serial.println("Year: "        + String(year));

  Serial.println("--------------------"); // Print a separator
  delay(1000); // 1 second pause
}
