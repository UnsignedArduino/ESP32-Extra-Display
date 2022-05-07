#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_I2CDevice.h>  // PlatformIO dependency finder is weird
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "tft.h"
#include "config.h"

void setup() {
  Serial.begin(9600);
  Serial.println();

  setupTFT();
}

void loop() {
  ;
}
