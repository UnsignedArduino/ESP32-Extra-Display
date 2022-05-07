#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_I2CDevice.h>  // PlatformIO dependency finder is weird
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <JPEGDEC.h>
#include "config.h"

#define FONT_WIDTH 6
#define FONT_HEIGHT 8

extern Adafruit_ILI9341 tft;
extern JPEGDEC jpeg;

bool setupTFT();

int JPEGDraw(JPEGDRAW *draw);

bool drawJPEG(byte* data, long size, int x, int y);
