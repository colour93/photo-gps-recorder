#include "GPSManager.h"

// 构造函数，指定 RX 和 TX 引脚，并初始化 Serial 对象
GPSManager::GPSManager(int rxPin, int txPin, int baud)
    : rxPin(rxPin), txPin(txPin), baud(baud), serialGPS(0) {}

// 初始化 GPS 模块
void GPSManager::begin()
{
  Serial.println("GPS manager is enabled");
  serialGPS.begin(baud, SERIAL_8N1, rxPin, txPin);
}

void GPSManager::update()
{
  while (serialGPS.available())
  {
    gps.encode(serialGPS.read());
  }
}

bool GPSManager::available()
{
  return serialGPS.available();
}

// 获取时间
String GPSManager::getTimeString()
{
  char timeStr[25];
  if (gps.time.isValid() && gps.date.isValid())
  {
    sprintf(timeStr, "%04d-%02d-%02d %02d:%02d:%02d+8",
            gps.date.year(),
            gps.date.month(),
            gps.date.day(),
            (gps.time.hour() + 8) % 24, // 转换为东八区时间
            gps.time.minute(),
            gps.time.second());
    return String(timeStr);
  }
  return "";
}

// 获取时间戳
unsigned long GPSManager::getTime()
{
  if (gps.time.isValid() && gps.date.isValid())
  {
    // 创建tm结构体
    struct tm timeinfo;
    timeinfo.tm_year = gps.date.year() - 1900; // 年份从1900年开始
    timeinfo.tm_mon = gps.date.month() - 1;    // 月份从0开始
    timeinfo.tm_mday = gps.date.day();
    timeinfo.tm_hour = (gps.time.hour() + 8) % 24; // 转换为东八区
    timeinfo.tm_min = gps.time.minute();
    timeinfo.tm_sec = gps.time.second();

    // 转换为时间戳
    return (unsigned long)mktime(&timeinfo);
  }
  return 0;
}
