#ifndef GPS_MANAGER_H
#define GPS_MANAGER_H

#include <TinyGPS++.h>
#include <HardwareSerial.h>

class GPSManager
{
public:
  GPSManager(int rxPin, int txPin, int baud);

  TinyGPSPlus gps;

  void begin();
  void update();
  bool available();
  String getTimeString();
  unsigned long getTime();

private:
  int rxPin;
  int txPin;
  int baud;
  HardwareSerial serialGPS;
};

#endif