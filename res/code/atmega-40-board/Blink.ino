/*
   Blink.ino
   Sketch to blink the onboard D13 "L" LED.
   Created on 15 May 2020 by Aidan Sun
*/

void setup() {
  pinMode(13, OUTPUT);
}

void loop() {
  digitalWrite(13, HIGH); // Turn LED on
  delay(1000); // Wait one second
  digitalWrite(13, LOW); // Turn LED off
  delay(1000); // Wait one second
}
