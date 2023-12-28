/*
    DS130X.cpp - Library for interfacing with a DS1302 and DS1307
    Created August 28, 2020
    MIT License
*/

#include <Arduino.h>
#include <Wire.h>

#include "DS130X.h"

static uint8_t getBitMask(uint8_t reg) {
  // Only some registers require bit masks
  const uint8_t bitMasks[4] = { 0x7F, 0xFF, 0x3F, 0xFF };
  return bitMasks[min(reg, 3)];
}

static uint8_t dec2bcd(uint8_t val) {
  return (val / 10 * 16) + (val % 10);
}

static uint8_t bcd2dec(uint8_t val) {
  return (val / 16 * 10) + (val % 16);
}

void BaseRTC::setHalt(bool halt) {
  // The CH bit (#7) in seconds register is the halt bit.
  uint8_t output = readRaw(SECONDS); // Read seconds register
  bitWrite(output, 7, halt);         // Edit the byte
  writeRaw(SECONDS, output);         // Send the new byte
}

void BaseRTC::setTime(uint8_t hour, uint8_t minute, uint8_t second) {
  write(HOURS, hour);
  write(MINUTES, minute);
  write(SECONDS, second);
}

void BaseRTC::setDate(uint8_t day, uint8_t month, uint8_t year, uint8_t dayOfWk) {
  write(DAY_OF_MONTH, day);
  write(MONTHS, month);
  write(YEARS, year);
  write(DAY_OF_WEEK, dayOfWk);
}

String BaseRTC::dateStr() {
  char output[9];
  snprintf(output, 9, "%.2d/%.2d/%.2d", read(DAY_OF_MONTH), read(MONTHS), read(YEARS));
  return String(output);
}

String BaseRTC::timeStr() {
  char output[9];
  snprintf(output, 9, "%.2d:%.2d:%.2d", read(HOURS), read(MINUTES), read(SECONDS));
  return String(output);
}

String BaseRTC::dowAbbr() {
  // Create an array of days
  static const String days[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
  return days[read(3) - 1]; // Return the corresponding day
}

void DS1302::_startTransmission() {
  digitalWrite(_ce, LOW);  // RTC not enabled (chip select low)
  pinMode(_ce, OUTPUT);    // Set chip select pin as output
  digitalWrite(_clk, LOW); // Disable RTC (chip select low)
  pinMode(_clk, OUTPUT);   // Set clock pin as output
  pinMode(_dat, OUTPUT);   // Set data pin as output
  digitalWrite(_ce, HIGH); // Enable RTC (chip select high)
  delayMicroseconds(4);    // CE -> CLK setup (tCC) is 4uS
}

void DS1302::_stopTransmission() {
  digitalWrite(_ce, LOW); // Disable RTC (chip select low)
  delayMicroseconds(4);   // CE inactive time (tCWH) is 4uS
}

uint8_t DS1302::_readByte() {
  uint8_t data = 0; // Empty data field
  for (int i = 0; i < 8; i++) {
    // Clock pulse for next bit
    digitalWrite(_clk, HIGH); // Clock high
    delayMicroseconds(1);     // 1uS between high and low
    digitalWrite(_clk, LOW);  // Clock low, data will be ready
    delayMicroseconds(1);     // CLK low time (tCL) is 1000 nS, CLK to data delay (tCDD) is 800nS
    bitWrite(data, i, digitalRead(_dat)); // Read in and process the new bit
  }
  return data; // Return processed data
}

void DS1302::_writeByte(uint8_t d, bool stop) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(_dat, bitRead(d, i)); // Send the next bit in data
    delayMicroseconds(1);              // tDC (data -> CLK setup) is 200 nS (round up to one whole uS)
    digitalWrite(_clk, HIGH);          // Clock pin high
    delayMicroseconds(1);              // CLK low time (tCL) is 1000 nS, CLK to data delay (tCDD) is 800nS

    // Communication stop, I/O line should be released after last bit to prevent a spike.
    if (stop && (i == 7)) {
      pinMode(_dat, INPUT);
    } else {
      digitalWrite(_clk, LOW);
      delayMicroseconds(1); // Disable RTC by setting CLK low
    }
  }
}

uint8_t DS1302::read(int reg) {
  uint8_t data = readRaw(reg);
  return bcd2dec(data & getBitMask(reg));
}

uint8_t DS1302::readRaw(int reg) {
  // Remap registers for DS1307 compatibility
  uint8_t r[] = { 0x81, 0x83, 0x85, 0x8B, 0x87, 0x89, 0x8D, reg };
  reg = r[min(reg, 7)];       // Assign new register value
  _startTransmission();       // Initialize communication
  _writeByte(reg, true);      // Request from register
  uint8_t data = _readByte(); // Get register data
  _stopTransmission();        // End communication

  // Return data (no bit masking or conversions)
  return data;
}

void DS1302::write(int reg, uint8_t data) {
  writeRaw(reg, dec2bcd(data));
}

void DS1302::writeRaw(int reg, uint8_t data) {
  // Remap registers for DS1307 compatibility
  uint8_t r[] = { 0x80, 0x82, 0x84, 0x8A, 0x86, 0x88, 0x8C, reg };
  reg = r[min(reg, 7)];    // Assign new register value
  _startTransmission();    // Initialize communication
  _writeByte(reg, false);  // Write to register
  _writeByte(data, false); // Write data
  _stopTransmission();     // End communication
}

uint8_t DS1307::read(int reg) {
  return bcd2dec(readRaw(reg) & getBitMask(reg));
}

uint8_t DS1307::readRaw(int reg) {
  Wire.beginTransmission(_addr);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(_addr, 1);
  return Wire.read();
}

void DS1307::write(int reg, uint8_t value) {
  writeRaw(reg, dec2bcd(value));
}

void DS1307::writeRaw(int reg, uint8_t value) {
  Wire.beginTransmission(_addr);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}
