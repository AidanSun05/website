// Pin constants
#define A_PIN      4
#define B_PIN      3
#define BUTTON_PIN 2

int state, lastState, counter;

void setup() {
  Serial.begin(9600);
  pinMode(A_PIN, INPUT);
  pinMode(B_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT);

  // Read initial
  lastState = digitalRead(A_PIN);
}

void loop() {
  state = digitalRead(A_PIN); // Read output of output A
  if (state != lastState) {
    // State has changed, so a pulse has occurred
    // If the outputB state is different to the outputA state, that means the encoder is rotating CW
    if (digitalRead(B_PIN) != state) {
      // Two output states are different, so encoder is rotating CW
      counter++; // Increment counter
    } else {
      counter--; // Rotating CCW, decrement counter
    }
  }
  int btn = digitalRead(BUTTON_PIN); // Read button
  lastState = state; // Update last state

  // Print values
  Serial.print("Counter: "); Serial.print(counter); // Print the counter
  Serial.print(" Button: "); Serial.println(btn);   // Print button state
}
