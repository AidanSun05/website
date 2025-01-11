// Include libraries
#include <SPI.h>     // For SPI communication
#include <SD.h>      // For SD operations
#include <DS130X.h>  // For DS1307

// Global variables
DS1307 rtc;
File f;

void setup() {
  Serial.begin(9600); // Open a serial port

  // Init SD card
  Serial.print("Initializing SD card... ");

  if (!SD.begin(4)) {
    Serial.println("Failed.");
    while (1);
  }
  Serial.println("Done.");
}

void loop() {
  // Open success, start logging
  String line = rtc.timeStr() + ","; // Get the time and delimit with comma

  int reading = analogRead(A0);

  // Perform necessary calculations here
  //reading = (reading * 500) / 1023; // For LM35: value -> deg C

  line.concat(reading); // Append reading to line

  f = SD.open("log.txt", FILE_WRITE); // Open log.txt for writing
  f.println(line);      // Write to file
  Serial.println(line); // Print line to serial monitor
  f.close();            // Close file
  delay(1000);          // Delay 1 second
}
