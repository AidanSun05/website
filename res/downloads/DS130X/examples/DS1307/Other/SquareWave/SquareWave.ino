/*
   SquareWave.ino
   Sketch to demonstrate the square wave output of the DS1307.
   Created August 29, 2020

   The DS1307 has a Square Wave output pin (#7). This pin can provide six frequencies:
   Continuous Off : SQW_LOW
   1 Hz           : SQW_1HZ
   4.096 kHz      : SQW_4KHZ
   8.192 kHz      : SQW_8KHZ
   32.768 kHz     : SQW_32KHZ
   Continuous On  : SQW_HIGH

   To use this output, first call setHalt(false) to enable the oscillator.
   Then, use writeRaw [not write()] to write to the SQW_OUT register.
   For a desired frequency, use the constant listed next to the frequency
   above.
*/

#include <DS130X.h>
#include <Wire.h>

DS1307 rtc;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  rtc.setHalt(false); // Enable internal oscillator (needed for working output)
}

void loop() {
  // Output high
  Serial.println("SQW High");
  rtc.writeRaw(SQW_OUT, SQW_HIGH);
  delay(5000);

  // Output 1 Hz
  Serial.println("SQW 1 Hz");
  rtc.writeRaw(SQW_OUT, SQW_1HZ);
  delay(5000);

  // Output 4.096 kHz
  Serial.println("SQW 4.096 kHz");
  rtc.writeRaw(SQW_OUT, SQW_4KHZ);
  delay(5000);

  // Output 8.192 kHz
  Serial.println("SQW 8.192 kHz");
  rtc.writeRaw(SQW_OUT, SQW_8KHZ);
  delay(5000);

  // Output 32.768 kHz
  Serial.println("SQW 32.768 kHz");
  rtc.writeRaw(SQW_OUT, SQW_32KHZ);
  delay(5000);

  // Output off
  Serial.println("SQW Off");
  rtc.writeRaw(SQW_OUT, SQW_LOW);
  delay(5000);
}
