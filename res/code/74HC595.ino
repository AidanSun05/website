// Define constants for pins
#define LATCH 5
#define CLOCK 6
#define DATA  4

byte out = 0;
int del = 70; // Delay time between segments

void setup() {
  // Configure all pins as outputs
  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA, OUTPUT);
}

void loop() {
  // Incremented for loop
  for (int i = 0; i < 8; i++) {
    // Clear the output, then set one bit
    out = 0;
    bitSet(out, i);

    // Send to shift register
    sendData(out);
    delay(del); // Delay
  }

  // Decremented for loop
  for (int i = 7; i > -1; i--) {
    // Clear the output, then set one bit
    out = 0;
    bitSet(out, i);

    // Send to shift register
    sendData(out);
    delay(del); // Delay
  }
}

void sendData(byte out) {
  // Send data to shift register
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, out);
  digitalWrite(LATCH, HIGH);
}
