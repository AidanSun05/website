/*
    SevenSegmentShield.cpp
    Library for interfacing with the Gravitech Seven Segment Shield
    Created on 8 April 2020 by Aidan Sun
*/

#include <Arduino.h>
#include <Wire.h>

#include "SevenSegmentShield.h"

static void writeRegister(uint8_t address, uint8_t reg, uint8_t value) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

void Display::begin() {
  writeBrightness(BRIGHTNESS_12MA); // Default segment brightness
}

void Display::writeBrightness(int brightness) {
  // Write to control register
  writeRegister(_addr, 0, brightness);
}

void Display::writeDigit(int digit, int value, bool decimalPoint) {
  writeDigitData(digit, _digits[value] + (uint8_t)decimalPoint * 128);
}

void Display::writeDigitData(int digit, uint8_t value) {
  writeRegister(_addr, digit, value);
}

void Display::displayInt(int number) {
  int loopEnd = 4;

  if (number < 0) {
    // Negative integer, display a dash in digit 1
    writeDigitData(4, 64);
    loopEnd = 3;
  }

  for (int i = 0; i < loopEnd; i++) {
    int val = (int)(abs(number) / pow(10, i)) % 10;
    writeDigit(i + 1, val, false); // Display the digit
  }
}

void Display::clear() {
  Wire.beginTransmission(_addr);
  Wire.write(1);

  for (int i = 0; i < 4; i++) Wire.write(0);

  Wire.endTransmission();
}

void TMP75::begin() {
  writeRegister(_addr, 1, 0x60); // Config register, 12-bit resolution
}

float TMP75::read() {
  Wire.beginTransmission(_addr);
  Wire.write(0);
  Wire.endTransmission();

  Wire.requestFrom(_addr, (uint8_t)2);
  uint8_t hByte = Wire.read();
  uint8_t lByte = Wire.read();

  return (((hByte << 8) | lByte) >> 4) * 0.0625;
}

void MemoryChip::writeByte(int address, uint8_t data) {
  Wire.beginTransmission(_addr);
  Wire.write(address >> 8);
  Wire.write(address & 0xFF);
  Wire.write(data);
  Wire.endTransmission();
}

uint8_t MemoryChip::readByte(int address) {
  Wire.beginTransmission(_addr);
  Wire.write(address >> 8);
  Wire.write(address & 0xFF);
  Wire.endTransmission();
  Wire.requestFrom(_addr, (uint8_t)1);
  return Wire.read();
}

RgbLed::RgbLed() {
  pinMode(_red, OUTPUT);
  pinMode(_green, OUTPUT);
  pinMode(_blue, OUTPUT);
}

void RgbLed::displayRGBColor(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(_red, r);
  analogWrite(_green, g);
  analogWrite(_blue, b);
}

void RgbLed::displayHexColor(long hh) {
  int r = hh >> 16;
  int g = (hh >> 8) & 0xFF;
  int b = hh & 0xFF;

  displayRGBColor(r, g, b);
}

void RgbLed::turnOff() {
  displayRGBColor(0, 0, 0);
}
