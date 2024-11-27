#ifndef GPS_MANAGER_H
#define GPS_MANAGER_H

#include <TinyGPS++.h>
#include <HardwareSerial.h>

class GPSManager
{
public:
  GPSManager(int rxPin, int txPin, int baud);

  TinyGPSPlus gps;
  HardwareSerial serialGPS;

  void begin();
  void update();
  bool available();
  String getTimeString();
  unsigned long getTime();
  unsigned char isDataReady();
  void sleep();
  void wake();

private:
  int rxPin;
  int txPin;
  int baud;
};

#endif