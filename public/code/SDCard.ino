#include <SPI.h>
#include <SD.h>

File f;

void setup() {
  Serial.begin(9600); // Open a serial port

  Serial.print("Initializing SD card... ");

  if (!SD.begin(4)) {
    Serial.println("Failed.");
    return;
  }
  Serial.println("Done.");

  f = SD.open("test.txt", FILE_WRITE); // Create test.txt

  if (f) { // open success
    Serial.println("Writing to file");
    f.println("This is a file test");
    f.close(); // Close file
    Serial.println("Done.");
  } else {
    // Open failed, show an error
    Serial.println("Error opening file");
  }

  // Open file for reading
  f = SD.open("test.txt");
  if (f) { // file opened successfully
    Serial.println("Reading file");
    while (f.available()) {
      // Read from file until reached the end
      char ch = f.read();
      Serial.print(ch);
    }
    f.close(); // Close the file
  } else { // file open failed
    Serial.println("File open failed.");
  }
}

void loop() {} // Empty loop
