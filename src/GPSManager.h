#ifndef GPS_MANAGER_H
#define GPS_MANAGER_H

#include <TinyGPS++.h>
#include <HardwareSerial.h>

class GPSManager
{
public:
  GPSManager(int rxPin, int txPin);

  // 初始化 GPS 模块
  void begin();

  // 更新 GPS 数据，返回是否有新数据
  bool update();

  // 获取经纬度和时间
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