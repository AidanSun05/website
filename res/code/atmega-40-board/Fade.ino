/*
   Fade.ino
   Sketch to fade the onboard D13 "L" LED.

   The "L" LED is connected to D13, which is a PWM pin on all microcontrollers.
   This means that we can change its brightness with PWM.

   Created on 15 May 2020 by Aidan Sun
*/

void setup() {
  pinMode(13, OUTPUT);
}

void loop() {
  for (int i = 0; i < 256; i++) {
    // Step brightness up from 0 to 255
    analogWrite(13, i);
    delay(4);
  }

  for (int i = 255; i > -1; i--) {
    // Step brightness down from 255 to 0
    analogWrite(13, i);
    delay(4);
  }

  // Delay at end of loop to prevent LED from flashing too quickly
  delay(100);
}
