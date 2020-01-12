#ifndef TrackPage_h
#define TrackPage_h

#include "Arduino.h"
#include <TFT_eSPI.h>

class TrackPage
{
  public:
    TrackPage(TFT_eSPI *tft);
    void draw();
    void refresh(float distance, float bearing, float course, float speed, float gps_altitude, float mag_heading, float baro_altitude);

  private:
    TFT_eSPI *_tft;
    float angle, x0, y0, x1, y1;

};

#endif
