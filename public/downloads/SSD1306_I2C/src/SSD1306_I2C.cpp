/*
   SSD_1306.cpp - Library for controlling a 128x64 SSD1306 OLED
   Created June 28, 2020
   MIT License
*/

#include <Arduino.h>
#include <Wire.h>

#include "SSD1306_I2C.h"
#include "font.h"

void SSD1306::_drawCircleBase(int x0, int y0, int x, int y, bool color, bool fill) {
  if (fill) {
    int x1Coords[] = { x0 + x, x0 + x, x0 + y, x0 + y };
    int y1Coords[] = { y0 + y, y0 - y, y0 + x, y0 - x };
    int x2Coords[] = { x0 - x, x0 - x, x0 - y, x0 - y };
    int y2Coords[] = { y0 + y, y0 - y, y0 + x, y0 - x };

    for (int i = 0; i < 4; i++) drawLine(x1Coords[i], y1Coords[i], x2Coords[i], y2Coords[i], color);
  } else {
    int xCoords[] = { x0 + x, x0 - x, x0 + x, x0 - x, x0 + y, x0 - y, x0 + y, x0 - y };
    int yCoords[] = { y0 + y, y0 + y, y0 - y, y0 - y, y0 + x, y0 + x, y0 - x, y0 - x };

    for (int i = 0; i < 8; i++) drawPixel(xCoords[i], yCoords[i], color);
  }
}

void SSD1306::_newLine() {
  setCursor(_curFront, _curY + _textSize * 8); // Change cursor position to new line
}

void SSD1306::begin(uint8_t addr) {
  _addr = addr;

  // Send initialization commands
  setState(false);                      // Display off while being configured
  sendCommand(0xD5); sendCommand(0x80); // Set display clock divide
  sendCommand(0xA8); sendCommand(0x3F); // Set multiplex ratio
  sendCommand(0xD3); sendCommand(0x00); // Set display offset
  sendCommand(0x40);                    // Set display start line
  sendCommand(0x8D); sendCommand(0x14); // Set charge pump setting
  sendCommand(0x20); sendCommand(0x00); // Set memory addressing mode
  sendCommand(0xA1);                    // Set segment re-map
  sendCommand(0xC8);                    // Set COM output scan direction
  sendCommand(0xDA); sendCommand(0x12); // Config COM pins
  sendCommand(0x81); sendCommand(0xCF); // Set contrast
  sendCommand(0xD9); sendCommand(0xF1); // Set pre-charge period
  sendCommand(0xDB); sendCommand(0x40); // Set VCOMH deselect level
  setInvert(false);                     // Display is not inverted
  clear();                              // Clear display
  updateDisplay();                      // Send buffer to display
  resumeDisplay();                      // RAM data is displayed
  setState(true);                       // Display on
}

void SSD1306::sendCommand(uint8_t x) {
  Wire.beginTransmission(_addr);
  Wire.write(0x00);
  Wire.write(x);
  Wire.endTransmission();
}

void SSD1306::setInvert(bool invert) {
  sendCommand(invert ? 0xA7 : 0xA6);
}

void SSD1306::setState(bool on) {
  sendCommand(on ? 0xAF : 0xAE);
}

void SSD1306::displayAllOn() {
  sendCommand(0xA5);
}

void SSD1306::resumeDisplay() {
  sendCommand(0xA4);
}

void SSD1306::updateDisplay() {
  // Set column address to 0
  sendCommand(0x21);
  sendCommand(0);   // Column start address
  sendCommand(127); // Column end address

  // Set page address to 0
  sendCommand(0x22);
  sendCommand(0); // Start page address
  sendCommand(7); // End page address

  // Send display buffer
  for (int i = 0; i < 1024; i++) {
    Wire.beginTransmission(_addr);
    Wire.write(0x40);
    Wire.write(_displayBuf[i]);
    Wire.endTransmission();
  }
}

void SSD1306::startScroll() {
  sendCommand(0x2F);
}

void SSD1306::stopScroll() {
  sendCommand(0x2E);
}

void SSD1306::setupScrollH(bool dir, uint8_t start, uint8_t end, uint8_t interval) {
  stopScroll();
  sendCommand(0x26 + int(dir));
  sendCommand(0x00);
  sendCommand(start);
  sendCommand(interval);
  sendCommand(end);
  sendCommand(0x00);
  sendCommand(0xFF);
}

void SSD1306::setupScrollHV(bool dirX, bool dirY, uint8_t start, uint8_t end, uint8_t interval) {
  stopScroll();

  // Command values for horizontal and vertical
  uint8_t addrs[] = { 0x29, 0x2A };
  uint8_t dirs[]  = { 1, 63 };

  sendCommand(addrs[dirX]);
  sendCommand(0x00);
  sendCommand(start);
  sendCommand(interval);
  sendCommand(end);
  sendCommand(dirs[dirY]);
}

void SSD1306::clear() {
  memset(_displayBuf, 0, 1024);
}

void SSD1306::drawPixel(int x, int y, bool color) {
  // Draw a single point on the screen
  bitWrite(_displayBuf[x + (y / 8) * 128], y % 8, color);
}

void SSD1306::drawLine(int x0, int y0, int x1, int y1, bool color) {
  // Draw a line with the Digital Differential Analyzer (DDA) algorithm
  // Get difference between endpoints
  int dx = x1 - x0;
  int dy = y1 - y0;

  // Get # of steps needed to put pixels
  int steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);

  // Get increment in X and Y
  float xInc = dx / float(steps);
  float yInc = dy / float(steps);

  // Put the line
  float x = float(x0), y = float(y0);
  for (int i = 0; i < steps; i++) {
    x += xInc;
    y += yInc;
    drawPixel(round(x), round(y), color);
  }
}

void SSD1306::drawAngledLine(int x, int y, int length, int angle, bool color) {
  // Use trigonometry to draw an angled line
  float x1 = cos(angle * (PI / 180)) * length;
  float y1 = sin(angle * (PI / 180)) * length;

  // Draw the line
  drawLine(x, y, x + x1, y + y1, color);
}

void SSD1306::drawRect(int x0, int y0, int w, int h, bool color, bool fill) {
  int y1 = y0 + h;
  int x1 = x0 + w;
  drawRectByCoords(x0, y0, x1, y1, color, fill);
}

void SSD1306::drawRectByCoords(int x0, int y0, int x1, int y1, bool color, bool fill) {
  if (fill) {
    for (int i = x0; i <= x1; i++) drawLine(i, y0, i, y1, color);
  } else {
    drawLine(x0, y0, x0, y1, color); // Corner 1 -> 2
    drawLine(x0, y0, x1, y0, color); // Corner 1 -> 3
    drawLine(x0, y1, x1, y1, color); // Corner 2 -> 4
    drawLine(x1, y0, x1, y1, color); // Corner 3 -> 4
  }
}

void SSD1306::drawCircle(int x, int y, int r, bool color, bool fill) {
  // Draw with Bresenham's Algorithm
  int x1 = 0, y1 = r;
  int d = 3 - 2 * r;
  _drawCircleBase(x, y, x1, y1, color, fill);

  while (x1 < y1) {
    x1++;
    if (d > 0) {
      y1--;
      d += 4 * (x1 - y1) + 10;
    } else {
      d += 4 * x1 + 6;
    }
    _drawCircleBase(x, y, x1, y1, color, fill);
  }
}

void SSD1306::displayBitmapImage(const unsigned char* img, int x, int y, int w, int h) {
  uint8_t currentByte = 0;
  // Iterate through the area (w * h) of the given bitmap
  // Begin at (x, y), the top-left corner, and go until (x+w, y+h), the bottom-right corner.
  for (int i = 0; i < h; i++) {   // Height
    for (int j = 0; j < w; j++) { // Width
      // Bitshift left all bits until there is room to fit another byte, then read in that byte.
      // A byte represents a 1x8 vertical column of pixels.
      currentByte = ((j % 8) > 0) ? currentByte << 1 : pgm_read_byte(&img[i * ((w + 7) / 8) + (j / 8)]);
      // Only draw a pixel if there is a 1 in the MSB and the byte is 8 bits long (8px tall column)
      // This is the top-most pixel in the column, the only one we care about in this iteration.
      drawPixel(x + j, y + i, (currentByte & 128) > 0);
    }
  }
}

void SSD1306::setCursor(int x, int y) {
  _curX = x;
  _curY = y;
  _curFront = x;
}

void SSD1306::setTextSize(int size) {
  _textSize = size;
}

void SSD1306::print(char c, bool color) {
  int chrVal = (int)c - 32;  // Convert char to ASCII value, subract 32 for offset

  // Iterate through pixel columns
  for (int i = 0; i < 5; i++) {
    // Copy character data from progmem to RAM
    uint8_t currentCol = pgm_read_byte(&ASCII_5X7[chrVal][i]);

    // Iterate through vertical pixels
    for (int j = 0; j < 7; j++) {
      bool pixelValue = bitRead(currentCol, j) ^ !color;

      // This loop changes the character's size
      for (int k = 0; k < _textSize * _textSize; k++)
        drawPixel(_curX + k % _textSize, _curY + j * _textSize + floor(k / _textSize), pixelValue); // Draw the pixel
    }
    _curX += _textSize; // Reposition cursor's X location
  }
  _curX += _textSize; // Padding between chars is proportional to font size
}

void SSD1306::print(String s, bool color) {
  s.replace("\t", "    "); // Replace tabs with four spaces

  for (int i = 0; i < s.length(); i++) {
    if (s[i] == '\n') _newLine(); // Reposition cursor to next line
    else print(s[i], color);
  }
}

void SSD1306::print(float s, bool color, int nplaces) {
  println((double)s, color, nplaces);
}

void SSD1306::print(double s, bool color, int nplaces) {
  print(String(s, nplaces), color);
}

void SSD1306::println(float s, bool color, int nplaces) {
  println((double)s, color, nplaces);
}

void SSD1306::println(double s, bool color, int nplaces) {
  println(String(s, nplaces), color);
}
