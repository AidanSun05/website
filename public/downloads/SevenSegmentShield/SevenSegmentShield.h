/*
    SevenSegmentShield.h
    Library for interfacing with the Gravitech Seven Segment Shield
    Created on 8 April 2020 by Aidan Sun
*/

#ifndef SEVEN_SEGMENT_SHIELD_H
#define SEVEN_SEGMENT_SHIELD_H

#include <Arduino.h>

#define BRIGHTNESS_3MA  0b00010111
#define BRIGHTNESS_6MA  0b00100111
#define BRIGHTNESS_9MA  0b00110111
#define BRIGHTNESS_12MA 0b01000111
#define BRIGHTNESS_18MA 0b01100111
#define BRIGHTNESS_21MA 0b01110111

class Display {
  uint8_t _addr = 0x38;
  int _digits[10] = { 63, 6, 91, 79, 102, 109, 125, 7, 127, 111 };

public:
  void begin();
  void writeBrightness(int brightness);
  void writeDigit(int digit, int value, bool decimalPoint);
  void writeDigitData(int digit, uint8_t value);
  void displayInt(int number);
  void clear();
};

class TMP75 {
  uint8_t _addr = 0x49;

public:
  void begin();
  float read();
};

class MemoryChip {
  uint8_t _addr = 0x50;

public:
  void writeByte(int address, uint8_t data);
  uint8_t readByte(int address);
};

class RgbLed {
  int _red   = 3;
  int _green = 5;
  int _blue  = 6;

public:
  RgbLed();

  void displayRGBColor(uint8_t r, uint8_t g, uint8_t b);
  void displayHexColor(long hh);
  void turnOff();
};
#endif
