#include "GPSManager.h"

// 构造函数，指定 RX 和 TX 引脚，并初始化 Serial 对象
GPSManager::GPSManager(int rxPin, int txPin)
    : rxPin(rxPin), txPin(txPin), serialGPS(1) {}

// 初始化 GPS 模块
void GPSManager::begin()
{
  serialGPS.begin(9600, SERIAL_8N1, rxPin, txPin);
}

// 更新 GPS 数据，返回是否有新数据
bool GPSManager::update()
{
  while (serialGPS.available())
  {
    gps.encode(serialGPS.read());
  }
  return gps.location.isUpdated();
}

// 获取纬度
double GPSManager::getLatitude()
{
  return gps.location.lat();
}

// 获取经度
double GPSManager::getLongitude()
{
  return gps.location.lng();
}

// 获取时间
unsigned long GPSManager::getTime()
{
  return gps.time.value();
}