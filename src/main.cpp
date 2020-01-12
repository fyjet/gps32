/*
 * GPS ESP32
 * Bull's eye single GPS
 * 
 * wiring
 * 
 * ESP32 GPIO    TFT display ILI9341
 * 15            TFT_CS 
 * 4             TFT_RESET 
 * 2             TFT_DC
 * 23            TFT_SDI/MOSI
 * 18            TFT_SCK
 * 3.3v          TFT_LED
 * 19            TFT_SDO/MISO
 * 18            T_CLK
 * 25            T_CS
 * 23            T_DIN
 * 19            T_DO
 * 26            T_IRQ (not used yet but wired)
 * 
 * ESP32 GPIO    GPS Module NEO 6M
 * 17            RXD
 * 16            TXD
 * 
 * ESP32 GPIO    Magnetometer GY271 (chip HMC5883L)
 * 21            SCL
 * 22            SDA
 * 
 * ESP32 GPIO    Pressure and temp sensor BMP280
 * 21            SCL
 * 22            SDA
 * 
 */


#include "Arduino.h"

#define DEBUG
#include "DebugMacros.h"

#include <HardwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <TFT_eSPI.h>   // /!\Manually copy include/User_Setup.h to .pio/libdeps/esp32dev/TFT_eSPI_ID1559 if you get a undefined reference to TFT_eSPI::getTouch
#include <TinyGPS.h>
#include "Geodesic.h"
#include "Modified_BMP280.h"

// Initialisation variable
#define BULLSEYE_LATITUDE 43.0    // in degrees
#define BULLSEYE_LONGITUDE 1.0   // in degrees
#define MAGNETIC_DECLINATION 1       // in degrees
#define QNH 1013.25                  // in Pa

// Device graphical pages
#include "LocationPage.h"
#include "TrackPage.h"
#include "CompassPage.h"

#include <Adafruit_Sensor.h>
#include "Modified_Adafruit_HMC5883_U.h"

// global variables
// magnetometer

double mag_heading;
// GPS statistics
unsigned short sentences, failed;
unsigned long chars;
bool signalLocked = false;

// device classes
HardwareSerial hsport(1);
TFT_eSPI tft = TFT_eSPI();
BMP280 bmp;
// sensor ID, and SDA,SCL pin (adafruit modified library)
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345,22,21);

// logical classes
TinyGPS gps;

double destination_latitude = BULLSEYE_LATITUDE;
double destination_longitude = BULLSEYE_LONGITUDE;
Geodesic myGeodesic(destination_latitude, destination_longitude);

// pages and current index
int screenMode=1;
LocationPage * locationPage;
TrackPage * trackPage;
CompassPage * compassPage;

// touchscreen coordinates
uint16_t x = 0, y = 0;

void buttonBar()
{
  tft.setCursor(150, 290, 2);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK,TFT_WHITE);
  tft.print(" PAGE ");
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
}

void setup()
{
  #ifdef DEBUG
    Serial.begin(115200);
  #endif
  
  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    DPRINTLN("Ooops, no HMC5883 detected ... Check your wiring!");
  }
  #ifdef DEBUG
    sensor_t sensor;
  #endif
  DPRINTLN("MAgSensor:"); 
  DPRINTLN(sensor.name);

  if(!bmp.begin(22,21)){
    DPRINTLN("BMP init failed!");
  }
  bmp.setOversampling(4);
  
  DPRINTLN("Opening hardware serial");
  hsport.begin(9600, SERIAL_8N1, 16, 17);

  DPRINTLN("Init TFT display");
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  // PAges creation
  locationPage = new LocationPage(&tft);
  trackPage = new TrackPage(&tft);
  compassPage = new CompassPage(&tft);

  // first display locatopn page
  locationPage->draw();

}

// display status bar
void statusBar()
{
  gps.stats(&chars, &sentences, &failed);
  DPRINT(" CHARS=");
  DPRINT(chars);
  DPRINT(" SENTENCES=");
  DPRINT(sentences);
  DPRINT(" CSUM ERR=");
  DPRINTLN(failed);
  
  tft.setCursor(120, 0, 2);
  
  if (signalLocked)
  {
    tft.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    tft.print(" sats  ");  
  } 
  else
  {
    if (chars == 0)
      {
      DPRINTLN("** No characters received from GPS: check wiring **");
      tft.print("no GPS");
      }
    else
    {  
      tft.print("Search the sky");
    }
  }
}

void loop()
{
  
  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (hsport.available())
    {
      char c = hsport.read();
      //Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        signalLocked = true;
    }

    if (tft.getTouch(&x, &y))
    {
      // Page button click, roll over each page
      if (x>215 and x<234 and y>200 and y<296)
      {
        if (screenMode==1)
        {
          screenMode=2;
          trackPage->draw();
        }
        else if (screenMode==2)
        {
          screenMode=3;
          compassPage->draw();
        }
        else if (screenMode==3)
        {
          screenMode=1;
          locationPage->draw();
        }
        delay(100);   
      }
    }   
  }

  // compute from sensors
  // read from GPS
  float flat, flon,rflat, rflon;
  unsigned long ftime, fdate, age;
  gps.f_get_position(&flat, &flon, &age);
  gps.get_datetime(&fdate, &ftime, &age);
  rflat= flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat;
  rflon= flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon;

  // read from magnetometer
  sensors_event_t event; 
  mag.getEvent(&event);
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  heading += MAGNETIC_DECLINATION;
  
  if(heading < 0)
    heading += 2*PI;
    
  if(heading > 2*PI)
    heading -= 2*PI;
   
  mag_heading = heading * 180/PI;

  // Pressure/temp
  double baro_temperature,baro_pressure;
  double baro_altitude = 0;
  char result = bmp.startMeasurment();
 
  if(result!=0){
    delay(result);
    result = bmp.getTemperatureAndPressure(baro_temperature,baro_pressure);
    
      if(result!=0)
      {
        // Altitude (QNH is AMSL pressure)
        baro_altitude = bmp.altitude(baro_pressure,QNH);
      }
  }
  
  // refresh display
  switch (screenMode)
  {
    case 1:
      locationPage->refresh(myGeodesic.distance(flat, flon), myGeodesic.bearing(flat, flon), rflat, rflon, fdate, ftime, gps.f_altitude());
      break;
    case 2:
      trackPage->refresh(myGeodesic.distance(flat, flon), myGeodesic.bearing(flat, flon), gps.f_course(), gps.f_speed_kmph(), gps.f_altitude(), mag_heading, baro_altitude);
      break;
    case 3:
      compassPage->refresh(mag_heading);
      break;  
  }
  
  statusBar();
  buttonBar();

}
