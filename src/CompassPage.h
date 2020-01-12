#ifndef CompassPage_h
#define CompassPage_h

#include "Arduino.h"
#include <TFT_eSPI.h>

class CompassPage
{
  public:
    CompassPage(TFT_eSPI *tft);
    void draw();
    void refresh(double mag_heading);

  private:
    TFT_eSPI *_tft;

};

#endif
