#include "DebugMacros.h"
#include "Arduino.h"
#include "TrackPage.h"
#include <TFT_eSPI.h>
#include <math.h>

#define COMPASS_X 180
#define COMPASS_Y 90
#define COMPASS_R 40
#define COMPASS_RI 38

TrackPage::TrackPage(TFT_eSPI *tft)
{
  _tft=tft;
  _tft->fillScreen(TFT_BLACK);
  _tft->setCursor(0, 0, 2);
  _tft->setTextColor(TFT_WHITE,TFT_BLACK);
  
  DPRINTLN("Appel constructeur TRACKPAGE");
}

void TrackPage::draw()
{
  _tft->fillScreen(TFT_BLACK);
  _tft->setCursor(0, 0, 2);
  _tft->setTextColor(TFT_WHITE,TFT_BLACK);
  
  _tft->setTextSize(2);
  _tft->print("TRK");
  _tft->setTextSize(1);
  _tft->setCursor(0, 30, 2);
  _tft->print("to_dist");
  //_tft->print("km");
  _tft->setCursor(0, 60, 2);
  _tft->setTextColor(TFT_GREEN,TFT_BLACK);
  _tft->print("to_brg");
  _tft->setTextColor(TFT_WHITE,TFT_BLACK);
  //_tft->print("deg");
  _tft->setCursor(0, 90, 2);
  _tft->print("course");
  _tft->setCursor(0, 120, 2);
  _tft->print("speed");
  //_tft->print("deg");
  _tft->setCursor(0, 150, 2);
  _tft->print("gps_altitude");
  //_tft->print("m");
  _tft->setCursor(0, 180, 2);
  _tft->print("baro_altitude");
  //_tft->print("m");
  _tft->setCursor(0, 210, 2);
  _tft->setTextColor(TFT_RED,TFT_BLACK);
  _tft->print("mag_bearing");
  _tft->setTextColor(TFT_WHITE,TFT_BLACK);
  //_tft->print("deg");

  // compass
  _tft->fillCircle(COMPASS_X, COMPASS_Y, 40, TFT_WHITE);
  _tft->drawLine(COMPASS_X, COMPASS_Y - COMPASS_R, COMPASS_X, COMPASS_Y - COMPASS_R - 6, TFT_WHITE);
}

void TrackPage::refresh(float distance, float bearing, float course, float speed, float gps_altitude, float mag_heading, float baro_altitude)
{
  
  _tft->setCursor(50, 30, 2);
  _tft->print(distance);
  _tft->setCursor(50, 60, 2);
  _tft->setTextColor(TFT_GREEN,TFT_BLACK);
  _tft->print(bearing);
  _tft->setTextColor(TFT_WHITE,TFT_BLACK);
  _tft->setCursor(50, 90, 2);
  _tft->print(course);
  _tft->setCursor(50, 120, 2);
  _tft->print(speed);
  _tft->setCursor(100, 150, 2);
  _tft->print(gps_altitude);
  _tft->setCursor(100, 180, 2);
  _tft->print(baro_altitude);
  _tft->setCursor(100, 210, 2);
  _tft->setTextColor(TFT_RED,TFT_BLACK);
  _tft->print(mag_heading);
  _tft->setTextColor(TFT_WHITE,TFT_BLACK);

  // draw compass
  _tft->setCursor(COMPASS_X - 10, COMPASS_Y - COMPASS_R - 20 , 2);
  _tft->print((int)round(mag_heading));
  _tft->print("  ");
  _tft->fillCircle(COMPASS_X, COMPASS_Y, COMPASS_RI , TFT_BLACK);
  // draw needle
  angle=mag_heading-bearing;
  x0=cos((angle-90)*PI/180)*COMPASS_RI+COMPASS_X;
  y0=sin((angle-90)*PI/180)*COMPASS_RI+COMPASS_Y;
  x1=cos((angle+90)*PI/180)*COMPASS_RI+COMPASS_X;
  y1=sin((angle+90)*PI/180)*COMPASS_RI+COMPASS_Y;  
  _tft->drawLine(x0, y0, x1, y1, TFT_WHITE);
  x1=cos((angle-80)*PI/180)*30+COMPASS_X;
  y1=sin((angle-80)*PI/180)*30+COMPASS_Y;  
  _tft->drawLine(x0, y0, x1, y1, TFT_WHITE);
  x1=cos((angle-100)*PI/180)*30+COMPASS_X;
  y1=sin((angle-100)*PI/180)*30+COMPASS_Y;  
  _tft->drawLine(x0, y0, x1, y1, TFT_WHITE);
  
}
