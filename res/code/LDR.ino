// Pin constants
#define LDR_PIN A0
#define LED_PIN 11

#define THRESHOLD 500

void setup() {
  pinMode(LED_PIN, OUTPUT); // set LED pin as output
  Serial.begin(9600); // Open serial port
}

void loop() {
  int sensorValue = analogRead(LDR_PIN); // Read LDR value

  if (sensorValue > THRESHOLD) { // LDR reading above 500
    digitalWrite(LED_PIN, HIGH);
  } else { // LDR reading below 500
    digitalWrite(LED_PIN, LOW);
  }
  Serial.println("LDR Reading: " + String(sensorValue)); // Print readings to serial monitor

  delay(2); // Let ADC settle
}
