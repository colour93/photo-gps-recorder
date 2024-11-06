#ifndef BLE_MANAGER_H
#define BLE_MANAGER_H

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLEAddress.h>
#include <Arduino.h>
#include <esp_wifi.h>

class BLEManager
{
public:
  BLEManager();
  void begin();
  String getWifiMac();

private:
  BLEServer *pServer;                 // BLEServer 实例，用于 BLE 服务
  BLEService *pService;               // BLE 服务
  BLECharacteristic *pCharacteristic; // BLE 特征
};

#endif
