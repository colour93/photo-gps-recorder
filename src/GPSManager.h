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
  void update();
  bool isLocationValid();
  double getLatitude();
  double getLongitude();
  bool isTimeValid();
  String getTimeString();
  unsigned long getTime();

private:
  int rxPin;
  int txPin;
  TinyGPSPlus gps;
  HardwareSerial serialGPS;
};

#endif