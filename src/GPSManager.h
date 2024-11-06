#ifndef GPS_MANAGER_H
#define GPS_MANAGER_H

#include <TinyGPS++.h>
#include <HardwareSerial.h>

class GPSManager
{
public:
  GPSManager(int rxPin, int txPin);

  void begin();

  bool available();
  bool update();
  double getLatitude();
  double getLongitude();
  unsigned long getTime();

private:
  int rxPin;
  int txPin;
  TinyGPSPlus gps;
  HardwareSerial serialGPS;
};

#endif