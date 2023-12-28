/*
   SSD_1306.h - Library for controlling a 128x64 SSD1306 OLED
   Created June 28, 2020
   MIT License
*/

#ifndef SSD1306_I2C_H
#define SSD1306_I2C_H

#include <Arduino.h>

// Constants
#define FRAMES_2   0b111
#define FRAMES_3   0b100
#define FRAMES_4   0b101
#define FRAMES_5   0b000
#define FRAMES_25  0b110
#define FRAMES_64  0b001
#define FRAMES_128 0b010
#define FRAMES_256 0b011

#define OUTLINE false
#define FILL    true

#define RIGHT false
#define LEFT  true

#define UP   false
#define DOWN true

class SSD1306 {
  uint8_t _displayBuf[1024]; // Display buffer
  uint8_t _addr;             // I2C address
  uint8_t _curX     = 0;
  uint8_t _curY     = 0;
  uint8_t _curFront = 0;
  uint8_t _textSize = 1;

  void _drawCircleBase(int x0, int y0, int x, int y, bool color, bool fill);

  void _newLine();

public:
  // Initialization and control
  void begin(uint8_t addr);
  void sendCommand(uint8_t x);
  void setInvert(bool invert);
  void setState(bool on);
  void displayAllOn();
  void resumeDisplay();
  void updateDisplay();

  // Scrolling
  void startScroll();
  void stopScroll();
  void setupScrollH(bool dir, uint8_t start, uint8_t end, uint8_t interval);
  void setupScrollHV(bool dirX, bool dirY, uint8_t start, uint8_t end, uint8_t interval);

  // Graphics
  void clear();
  void drawPixel(int x, int y, bool color);
  void drawLine(int x0, int y0, int x1, int y1, bool color);
  void drawAngledLine(int x, int y, int length, int angle, bool color);
  void drawRect(int x0, int y0, int w, int h, bool color, bool fill);
  void drawRectByCoords(int x0, int y0, int x1, int y1, bool color, bool fill);
  void drawCircle(int x, int y, int r, bool color, bool fill);
  void displayBitmapImage(const unsigned char *img, int x, int y, int w, int h);

  // Text
  void setCursor(int x, int y);
  void setTextSize(int size);

  template <typename T>
  void print(T s, bool color) {
    print(String(s), color);
  }

  void print(char s, bool color);
  void print(String s, bool color);

  void print(float s, bool color, int nplaces);
  void print(double s, bool color, int nplaces);

  template <typename T>
  void println(T s, bool color) {
    print(s, color);
    _newLine();
  }

  void println() {
    _newLine();
  }

  void println(float s, bool color, int nplaces);
  void println(double s, bool color, int nplaces);
};
#endif
