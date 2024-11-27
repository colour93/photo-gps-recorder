#ifndef BLE_MANAGER_H
#define BLE_MANAGER_H

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLEAddress.h>
#include <Arduino.h>
#include <esp_wifi.h>
#include <SPIFFS.h>

class BLEManager
{
public:
  BLEManager();
  void begin();
  void updateTime(unsigned long time);
  void updateLocation(float latitude, float longitude, float altitude);
  void updateSatellites(int satellites);
  void updateStatus(unsigned char status);

private:
  BLEServer *pServer;                           // BLEServer 实例，用于 BLE 服务
  BLEService *pService;                         // BLE 服务
  BLECharacteristic *pTimeCharacteristic;       // BLE 特征
  BLECharacteristic *pLatitudeCharacteristic;   // BLE 特征
  BLECharacteristic *pLongitudeCharacteristic;  // BLE 特征
  BLECharacteristic *pAltitudeCharacteristic;   // BLE 特征
  BLECharacteristic *pSatellitesCharacteristic; // BLE 特征
  BLECharacteristic *pStatusCharacteristic;     // BLE 特征
  BLECharacteristic *pUserDataCharacteristic;   // BLE 特征
};

#endif
