#ifndef Geodesic_h
#define Geodesic_h

#include "Arduino.h"

class Geodesic
{
  public:
    Geodesic(float destination_latitude,float destination_longitude);
    float bearing(float from_latitude,float from_longitude);
    float distance(float from_latitude,float from_longitude);
    void setDestination(float destination_latitude,float destination_longitude);
  private:
    float _destination_latitude;
    float _destination_longitude;
};

#endif
