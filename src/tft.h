#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_I2CDevice.h>  // PlatformIO dependency finder is weird
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "config.h"

extern Adafruit_ILI9341 tft;

bool setupTFT();
