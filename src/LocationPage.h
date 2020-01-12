#ifndef LocationPage_h
#define LocationPage_h

#include "Arduino.h"
#include <TFT_eSPI.h>

class LocationPage
{
  public:
    LocationPage(TFT_eSPI *tft);
    void draw();
    void refresh(float distance, float bearing,float flat, float flon, unsigned long fdate, unsigned long ftime, float gps_altitude);

  private:
    TFT_eSPI *_tft;
    
};

#endif
