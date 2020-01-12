#include "DebugMacros.h"
#include "Arduino.h"
#include "LocationPage.h"
#include <TFT_eSPI.h>


LocationPage::LocationPage(TFT_eSPI *tft)
{
  _tft=tft;
  _tft->fillScreen(TFT_BLACK);
  _tft->setCursor(0, 0, 2);
  _tft->setTextColor(TFT_WHITE,TFT_BLACK);
  
  DPRINTLN("Appel constructeur LOCATIONPAGE");
}

void LocationPage::draw()
{
  _tft->fillScreen(TFT_BLACK);
  _tft->setCursor(0, 0, 2);
  _tft->setTextColor(TFT_WHITE,TFT_BLACK);
  
  _tft->setTextSize(2);
  _tft->print("LOC");
  _tft->setTextSize(1);
  _tft->setCursor(0, 30, 2);
  _tft->print("lat");
  //_tft->print(" deg  ");
  _tft->setCursor(0, 60, 2);
  _tft->print("lon");
  //_tft->print(" deg  ");
  _tft->setCursor(0, 90, 2);
  _tft->print("day time");
  _tft->setCursor(0, 120, 2);
  _tft->print("altitude");
  _tft->setCursor(0, 150, 2);
  _tft->print("to_dist");
  //_tft->print("km");
  _tft->setCursor(0, 180, 2);
  _tft->print("to_brg");
  //_tft->print("deg");
}

void LocationPage::refresh(float distance, float bearing,float flat, float flon, unsigned long fdate, unsigned long ftime, float gps_altitude)
{
  
  _tft->setCursor(100, 30, 2);
  _tft->print(flat, 6);
  _tft->setCursor(100, 60, 2);
  _tft->print(flon, 6);
  _tft->setCursor(100, 90, 2);
  _tft->print(fdate);
  _tft->print(" ");
  _tft->print(ftime);
  _tft->setCursor(100, 120, 2);
  _tft->print(gps_altitude);
  _tft->setCursor(100, 150, 2);
  _tft->print(distance);
  _tft->setCursor(100, 180, 2);
  _tft->print(bearing);
}
