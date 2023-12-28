// Define pin constants
#define EN  5
#define IN1 4
#define IN2 3

boolean isReversed = false;

void setup() {
  // Configure pins as output
  pinMode(EN,  OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
}

void loop() {
  for (int i = 0; i < 256; i++) {
    // Increment the speed from 0 to 255
    setMotorSpeed(i, isReversed);
    delay(10);
  }

  delay(3000); // Run at full speed for 3 seconds

  for (int i = 255; i > 0; i--) {
    // Decrement the speed from 255 to 0
    setMotorSpeed(i, isReversed);
    delay(10);
  }

  // Stop the motor and wait a little bit
  stopMotor();
  delay(2000);
  isReversed = !isReversed; // Reverse the motor
}

// Motor control functions

void setMotorSpeed(int speed, boolean reverse) {
  // Set the speed of the motor.
  analogWrite(EN, speed);
  digitalWrite(IN1, reverse);
  digitalWrite(IN2, !reverse);
}

void stopMotor() {
  // Stop the motor.
  analogWrite(EN, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}
