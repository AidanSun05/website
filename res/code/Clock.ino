// Keyboard shortcuts:
// TIME+ and ALARM+  loads alarm time
// TIME- and ALARM-  saves alarm time
// TIME+ and SLEEP   clears alarm time

#include <DS130X.h>     // RTC
#include <LedControl.h> // MAX7219
#include <EEPROM.h>     // EEPROM

// Pin definitions

// RTC Pins
#define RTC_CLK 2 // CLK pin on DS1302
#define RTC_DAT 3 // DAT pin on DS1302
#define RTC_RST 4 // RST pin on DS1302

// LED Pins
#define LED_DIN    5 // DIN pin on MAX7219
#define LED_CS     6 // LOAD pin on MAX7219
#define LED_CLK    7 // CLK pin on MAX7219
#define COLON      8 // Pin 8 on LED display
#define INDICATOR 10 // LED to indicate if alarm is on

// Button pins
#define TIME_PLUS    11
#define TIME_MINUS   12
#define SNOOZE_1     14
#define SNOOZE_2     15
#define SLEEP        16
#define ALARM_ON_OFF 17
#define ALARM_MINUS  18
#define ALARM_PLUS   19

#define BUZZER  9 // Buzzer pin

// Variables
DS1302 rtc(RTC_DAT, RTC_CLK, RTC_RST);
LedControl lc(LED_DIN, LED_CLK, LED_CS, 1);

int h, m, s; // Variables for time
int alarm_hr = 12;
int alarm_min = 0;
boolean alarm = false;
boolean pressed = false;
boolean prev = false;
boolean done = false;
int del = 250; // Milliseconds between time changes if the TIME +/- button is held down
int snooze_time = 5; // Minutes in between pressing snooze and alarm going off again

void setup() {
  // Configure digital pins
  // The pins A6/7 do not have internal pullup resistors; therefore, they are not used
  pinMode(COLON, OUTPUT);
  pinMode(INDICATOR, OUTPUT);
  pinMode(TIME_PLUS, INPUT_PULLUP);
  pinMode(TIME_MINUS, INPUT_PULLUP);
  pinMode(SNOOZE_1, INPUT_PULLUP);
  pinMode(SNOOZE_2, INPUT_PULLUP);
  pinMode(SLEEP, INPUT_PULLUP);
  pinMode(ALARM_ON_OFF, INPUT_PULLUP);
  pinMode(ALARM_MINUS, INPUT_PULLUP);
  pinMode(ALARM_PLUS, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);

  // RTC setup
  rtc.setHalt(false); // Set the RTC to run mode
  rtc.setWriteProtect(false); // Disable write protection

  // MAX7219 setup
  const int displayBrightness = 8;
  lc.shutdown(0, false); // Turn off power saving and enable display
  lc.setIntensity(0, displayBrightness); // Set brightness (Possible values 0 to 15)
  lc.clearDisplay(0); // Clear the display
}

void loop() {
  // Get h, m, and s
  h = rtc.read(HOURS);
  m = rtc.read(MINUTES);
  s = rtc.read(SECONDS);

  check12h(); // Check the current time and adjust if necessary
  displayNum(getTens(h), getOnes(h), getTens(m), getOnes(m)); // Display the time

  if (not(digitalRead(SLEEP)) && done) {
    alarm = false; // no alarm
    digitalWrite(BUZZER, LOW);
  }

  if (((not(digitalRead(SNOOZE_1))) || (not(digitalRead(SNOOZE_2)))) && done) {
    // Snooze pressed, add 5 minutes
    alarm_min += snooze_time;
    check12hAlarm();
    digitalWrite(BUZZER, LOW); // turn buzzer off
    done = false; // not done yet
  }

  // Check if the Alarm On/Off button has been pressed
  pressed = !digitalRead(ALARM_ON_OFF);
  // Only adtivate/deactivate alarm once button is released
  if (pressed != prev) {
    if (pressed) {
      alarm = !alarm;
    }
  }
  prev = pressed;
  digitalWrite(INDICATOR, alarm); // Indicator to tell if alarm is on

  // Alarm setting/checking
  setAlarm();
  checkAlarm();

  // Flash colon
  if ((s % 2) == 0) {
    // Turn on the colon if the amount of seconds is even
    digitalWrite(COLON, HIGH);
  } else {
    // Turn off the colon if the amount of seconds is odd
    digitalWrite(COLON, LOW);
  }

  if (not(digitalRead(TIME_PLUS))) {
    // Time+ button is pressed
    // Check for keyboard shortcut
    if (not(digitalRead(ALARM_PLUS))) {
      // Load alarm time
      loadAlarmTime();
    }
    else if (not(digitalRead(SLEEP))) {
      // Clear alarm time (these two buttons are far apart so no accidental clearing)
      clearAlarmTime();
    }
    else {
      // Increment minutes
      m += 1; // Increment m
      adjustTime();
      rtc.setTime(h, m, 0); // Set the new time
      delay(del); // Delay between changes if the button is held down
    }
  }

  if (not(digitalRead(TIME_MINUS))) {
    // Time- button is pressed
    // Check for keyboard shortcut
    if (not(digitalRead(ALARM_MINUS))) {
      // Save alarm time
      saveAlarmTime();
    }
    else {
      m -= 1; // Decrement m
      adjustTime();
      rtc.setTime(h, m, 0); // Set the new time
      delay(del); // Delay between changes if the button is held down
    }
  }
}

int getTens(int num) {
  return int(num / 10);
}

int getOnes(int num) {
  return num % 10;
}

void displayNum(int d1, int d2, int d3, int d4) {
  lc.setDigit(0, 0, d1, false);
  lc.setDigit(0, 1, d2, false);
  lc.setDigit(0, 2, d3, false);
  lc.setDigit(0, 3, d4, false);
}

void check12h() {
  if (h > 12) {
    h -= 12;
  }
  else if (h == 0) {
    h = 12;
  }
}

void check12hAlarm() {
  if (alarm_hr > 12) {
    alarm_hr -= 12;
  }
  else if (alarm_hr == 0) {
    alarm_hr = 12;
  }
}

void setAlarm() {
  if (alarm) {
    if (!digitalRead(ALARM_PLUS) && !done) {
      alarm_min++;
      adjustAlarm();
      displayNum(getTens(alarm_hr), getOnes(alarm_hr), getTens(alarm_min), getOnes(alarm_min));
      delay(del + 150); // more time to see the set alarm before displaying current time
    }

    if (!digitalRead(ALARM_MINUS) && !done) {
      alarm_min--;
      adjustAlarm();
      displayNum(getTens(alarm_hr), getOnes(alarm_hr), getTens(alarm_min), getOnes(alarm_min));
      delay(del + 150);
    }
  }
}

void checkAlarm() {
  if ((((h == alarm_hr) && (m == alarm_min)) || done) && (alarm && !checkAlarmButtonPress())) {
    done = true;
    if ((s % 2) == 0) { // Turn on the alarm if the amount of seconds is even
      digitalWrite(BUZZER, HIGH);
    }
    else { // Turn off the alarm if the amount of seconds is odd
      digitalWrite(BUZZER, LOW);
    }
  }
}

boolean checkAlarmButtonPress() {
  return !digitalRead(ALARM_PLUS) || !digitalRead(ALARM_MINUS);
}

void adjustTime() {
  if (m == 60) {
    // Overflow in minutes, set minutes to 0 and increment hour
    m = 0;
    h++;
  }
  if (m < 0) {
    // Underflow in minutes, set minutes to 59 and decrement hour
    m = 59;
    h--;
  }
  check12h(); // adjust if necessary
}

void adjustAlarm() {
  if (alarm_min == 60) {
    // Overflow in minutes, set minutes to 0 and increment hour
    alarm_min = 0;
    alarm_hr++;
  }
  if (alarm_min == -1) {
    // Underflow in minutes, set minutes to 59 and decrement hour
    alarm_min = 59;
    alarm_hr--;
  }
  check12hAlarm(); // adjust if necessary
}

void saveAlarmTime() {
  // Index 0 is hour
  // Index 1 is minute
  EEPROM.update(0, alarm_hr); // Only write if it is a new time
  EEPROM.update(1, alarm_min);
}

void loadAlarmTime() {
  alarm_hr = EEPROM.read(0);
  alarm_min = EEPROM.read(1);
}

void clearAlarmTime() {
  // Reset alarm values to default (12:00)
  EEPROM.update(0, 12); // hour
  EEPROM.update(1, 0);  // minute
}
