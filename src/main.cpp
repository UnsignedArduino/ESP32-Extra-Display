#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_I2CDevice.h>  // PlatformIO dependency finder is weird
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "tft.h"
#include "config.h"

byte jpegBuffer[MAX_JPEG_SIZE] = {};

#if defined(SERIAL_DONT_TIMEOUT)

void waitForAvailable() {
  while (Serial.available() == 0) {
    ;
  }
}

#endif

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(9600);
  Serial.println();

  setupTFT();
}

void loop() {
  #if defined(SERIAL_DONT_TIMEOUT)
  waitForAvailable();
  #endif

  long expectedSize = Serial.parseInt();
  if (expectedSize == 0) {
    return;
  }

  digitalWrite(LED_BUILTIN, HIGH);

  memset(jpegBuffer, 0, MAX_JPEG_SIZE);

  #if defined(SERIAL_DONT_TIMEOUT)
  waitForAvailable();
  #endif

  long readInto = Serial.readBytes(jpegBuffer, expectedSize);

  if (readInto != expectedSize) {
    return;
  }

  drawJPEG(jpegBuffer, expectedSize, 0, 0);

  digitalWrite(LED_BUILTIN, LOW);
}
