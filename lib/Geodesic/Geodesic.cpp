/*
 * Class to compute distance and bearing from geographical coordinates
 *
 * formulas and explanation, see https://www.movable-type.co.uk/scripts/latlong.html
*/
#include "Arduino.h"
#include "math.h"
#include "Geodesic.h"

Geodesic::Geodesic(float destination_latitude,float destination_longitude)
{
  _destination_latitude=destination_latitude;
  _destination_longitude=destination_longitude;
}

float Geodesic::bearing(float from_latitude,float from_longitude)
{
  float teta_from_latitude = radians(from_latitude);
  float teta_destination_latitude = radians(_destination_latitude);
  float delta_longitude = radians(_destination_longitude-from_longitude);

  float y = sin(delta_longitude) * cos(teta_destination_latitude);
  float x = cos(teta_from_latitude)*sin(teta_destination_latitude) - sin(teta_from_latitude)*cos(teta_destination_latitude)*cos(delta_longitude);
  float brng = atan2(y,x);
  brng = degrees(brng);// radians to degrees
  brng = fmod(brng + 360, 360 );

  return brng;
}

float Geodesic::distance(float from_latitude,float from_longitude)
{

  float radius = 6371;   // hearth radius, in kilometers
  float delta_latitude = radians(_destination_latitude - from_latitude);
  float delta_longitude = radians(_destination_longitude - from_longitude);

  // compute distance with help of haversine formula
  float a = (sin(delta_latitude/2.0)*sin(delta_latitude/2.0)) + cos(radians(from_latitude)) * cos(radians(_destination_latitude)) * (sin(delta_longitude/2.0)*sin(delta_longitude/2.0));
  float c = 2.0 * asin(sqrtf(a));
  float distance = radius * c;

  return distance;
}

void Geodesic::setDestination(float destination_latitude,float destination_longitude)
{
  _destination_latitude=destination_latitude;
  _destination_longitude=destination_longitude;
}
