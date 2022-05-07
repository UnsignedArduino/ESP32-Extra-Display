#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_I2CDevice.h>  // PlatformIO dependency finder is weird
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <JPEGDEC.h>
#include "config.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
JPEGDEC jpeg;

bool setupTFT() {
  Serial.println("Setup ILI9341"); 
 
  tft.begin(SPI_SPEED);

  // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDPIXFMT);
  Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDIMGFMT);
  Serial.print("Image Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 

  tft.setRotation(TFT_ROTATION);

  tft.fillScreen(ILI9341_BLACK);

  return true;
}

int JPEGDraw(JPEGDRAW* draw) {
  tft.dmaWait(); // Wait for prior writePixels() to finish
  tft.setAddrWindow(draw->x, draw->y, draw->iWidth, draw->iHeight);
  tft.writePixels(draw->pPixels, draw->iWidth * draw->iHeight, true, false); // Use DMA, big-endian
  return 1;
}

bool drawJPEG(byte* data, long size, int x, int y) {
  bool success = false;
  tft.startWrite();
  if (jpeg.openRAM(data, size, JPEGDraw)) {
    if (jpeg.decode(x, y, 0)) {
      success = true;
    }
    jpeg.close();
  }
  tft.endWrite();
  return success;
}
