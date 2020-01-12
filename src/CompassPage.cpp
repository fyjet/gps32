#include "DebugMacros.h"
#include "Arduino.h"
#include "CompassPage.h"
#include <TFT_eSPI.h>

CompassPage::CompassPage(TFT_eSPI *tft)
{
  _tft=tft;
  _tft->fillScreen(TFT_BLACK);
  _tft->setCursor(0, 0, 2);
  _tft->setTextColor(TFT_WHITE,TFT_BLACK);
  
  DPRINTLN("Appel constructeur COMPASSPAGE");
}

void CompassPage::draw()
{
  _tft->fillScreen(TFT_BLACK);
  _tft->setCursor(0, 0, 2);
  _tft->setTextColor(TFT_WHITE,TFT_BLACK);
  
  _tft->setTextSize(2);
  _tft->print("HDG");
  _tft->setTextSize(1);
  _tft->setCursor(0, 30, 2);
  _tft->print("mag heading");
}

void CompassPage::refresh(double mag_heading)
{
  _tft->setCursor(100, 30, 2);
  _tft->print(mag_heading, 6);
}
