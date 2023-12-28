/*
    DS130X.h - Library for interfacing with a DS1302 and DS1307
    Created August 28, 2020
    MIT License
*/

#ifndef DS130X_H
#define DS130X_H

#include <Arduino.h>

// Common constants
#define SUNDAY    1
#define MONDAY    2
#define TUESDAY   3
#define WEDNESDAY 4
#define THURSDAY  5
#define FRIDAY    6
#define SATURDAY  7

// Time keeping registers
#define SECONDS      0x00
#define MINUTES      0x01
#define HOURS        0x02
#define DAY_OF_WEEK  0x03
#define DAY_OF_MONTH 0x04
#define MONTHS       0x05
#define YEARS        0x06
#define SQW_OUT      0x07 // Applicable to DS1307 only
#define TCR_SET      0x90 // Applicable to DS1302 only
#define TCR_GET      0x91 // Applicable to DS1302 only

// DS1302 TCR  constants
#define TCR_DISABLE 0x00
#define TCR_1D_2R   0xA5
#define TCR_1D_4R   0xA6
#define TCR_1D_8R   0xA7
#define TCR_2D_2R   0xA9
#define TCR_2D_4R   0xAA
#define TCR_2D_8R   0xAB
#define TCR_INITIAL 0x5C

// DS1307 SQW constants
#define SQW_LOW    0x00
#define SQW_HIGH   0x80
#define SQW_1HZ    0x10
#define SQW_4KHZ   0x11
#define SQW_8KHZ   0x12
#define SQW_32KHZ  0x13

// Base RTC structure. DS1302 and DS1307 inherit from this.
struct BaseRTC {
  // Methods common to both RTCs (will be overridden)
  virtual uint8_t read(int reg);
  virtual uint8_t readRaw(int reg);
  virtual void write(int reg, uint8_t value);
  virtual void writeRaw(int reg, uint8_t value);

  // Methods common to both RTCs (identical code, won't be overridden)
  void setHalt(bool halt);
  void setTime(uint8_t hour, uint8_t minute, uint8_t second);
  void setDate(uint8_t day, uint8_t month, uint8_t year, uint8_t dayOfWk);
  String dateStr();
  String timeStr();
  String dowAbbr();
};

class DS1302 : public BaseRTC {
  int _ce;
  int _dat;
  int _clk;

  void _startTransmission();
  void _stopTransmission();
  uint8_t _readByte();
  void _writeByte(uint8_t data, bool stop);

public:
  // Methods unique to DS1302
  DS1302(int dataPin, int clockPin, int cePin)  : _dat(dataPin), _clk(clockPin), _ce(cePin) {}

  void setWriteProtect(bool protect) {
    writeRaw(0x8E, protect);
  }

  uint8_t ramRStart() {
    return 0xC1; // Start address for RAM read
  }

  uint8_t ramWStart() {
    return 0xC0; // Start address for RAM write
  }

  uint8_t ramREnd() {
    return 0xFD; // End address for RAM read
  }

  uint8_t ramWEnd() {
    return 0xFC; // End address for RAM write
  }

  // Common RTC methods
  uint8_t read(int reg) override;
  uint8_t readRaw(int reg) override;
  void write(int reg, uint8_t value) override;
  void writeRaw(int reg, uint8_t value) override;
};

class DS1307 : public BaseRTC {
  int _addr = 0x68; // DS1307 has fixed address of 68h

public:
  // Methods unique to DS1307
  uint8_t ramStart() {
    return 0x08; // RAM starting address is 08h
  }

  uint8_t ramEnd() {
    return 0x3F; // RAM ending address is 3Fh
  }

  // Common RTC methods
  uint8_t read(int reg) override;
  uint8_t readRaw(int reg) override;
  void write(int reg, uint8_t value) override;
  void writeRaw(int reg, uint8_t value) override;
};
#endif
