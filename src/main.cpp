#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_I2CDevice.h>  // PlatformIO dependency finder is weird
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "tft.h"
#include "config.h"

byte jpegBuffer[MAX_JPEG_SIZE] = {};

#define TFT_WIDTH_FILLER "                                                            "

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

  Serial.begin(SERIAL_BAUD_RATE);
  Serial.setTimeout(SERIAL_TIMEOUT);
  Serial.println();

  setupTFT();

  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextWrap(false);
}

void loop() {
  #if defined(SERIAL_DONT_TIMEOUT)
  waitForAvailable();
  #endif

  long expectedSize = Serial.parseInt();

  if (expectedSize == 0) {
    #if defined(SHOW_STATS)
    tft.setCursor(0, tft.height() - FONT_HEIGHT);
    tft.print("Waiting for image (baud rate: ");
    tft.print(SERIAL_BAUD_RATE);
    tft.print(" bps)");
    tft.print(TFT_WIDTH_FILLER);
    #endif
    return;
  }

  #if defined(USE_BUILTIN_LED)
  digitalWrite(LED_BUILTIN, HIGH);
  #endif

  memset(jpegBuffer, 0, MAX_JPEG_SIZE);

  #if defined(SERIAL_DONT_TIMEOUT)
  waitForAvailable();
  #endif

  unsigned long startRead = millis();
  long readInto = Serial.readBytes(jpegBuffer, expectedSize);
  unsigned long endRead = millis();
  unsigned long readTime = endRead - startRead;

  if (readInto != expectedSize) {
    #if defined(SHOW_STATS)
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    tft.setCursor(0, tft.height() - FONT_HEIGHT);
    tft.print("Got bad sized image! (");
    tft.print(readInto);
    tft.print(" b != ");
    tft.print(expectedSize);
    tft.print(" b)");
    tft.print(TFT_WIDTH_FILLER);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    #endif
    return;
  }

  unsigned long startDraw = millis();
  drawJPEG(jpegBuffer, expectedSize, 0, 0);
  unsigned long endDraw = millis();
  unsigned long drawTime = endDraw - startDraw;

  #if defined(SHOW_STATS)
  tft.setCursor(0, tft.height() - FONT_HEIGHT);
  tft.print("R: ");
  tft.print(readTime);
  tft.print(" ms D: ");
  tft.print(drawTime);
  tft.print(" ms");
  tft.print(TFT_WIDTH_FILLER);
  #endif

  #if defined(USE_BUILTIN_LED)
  digitalWrite(LED_BUILTIN, LOW);
  #endif
}
