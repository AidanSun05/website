/*
   TestCoinCell.ino
   Sketch to print the voltage of the coin cell to the serial monitor.
   Baud rate: 9600 baud

   Circuit: Connect one of the "BAT" pins to A0.

   Created on 15 May 2020 by Aidan Sun
*/

void setup() {
  Serial.begin(9600); // Open a serial port
  Serial.print("Coin Cell Test - ");

  // Get A0 readings
  int analogIn = analogRead(A0);

  // Get voltage from analog reading
  // The maximum voltage that can be read is 5V and the number of possible values returned
  // by analogRead() is 1024.
  // Divide 5 by 1024 to get a scale factor of 0.0048. Multiply this scale factor by the
  // analog reading to get voltage.
  float voltage = analogIn * 0.0048;

  // Print the voltage to one decimal
  Serial.print(voltage, 1);
  Serial.print("V "); // Print the unit (volts)

  if (voltage >= 2.0) {
    // If the battery is 2.0V or above, it is OK.
    Serial.println("Battery OK");
  }
  else {
    // If the battery is less than 2.0V, it needs to be replaced.
    Serial.println("Replace battery.");
  }
}

void loop() {} // No loop
