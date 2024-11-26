#include "BLEManager.h"

// 定义BLE服务和特性UUID
#define DEVICE_INFO_SERVICE_UUID 0x180A
#define DEVICE_NAME_UUID 0x2A00
#define LOCATION_SERVICE_UUID 0x1819
#define LATITUDE_UUID 0x2AAE
#define LONGITUDE_UUID 0x2AAF
#define LOCAL_TIME_UUID 0x2A0F

BLEManager::BLEManager() {}

void BLEManager::begin()
{
  String deviceName = String("F93_PGR1_0001");

  // 初始化 BLE 设备
  BLEDevice::init(deviceName.c_str());
  pServer = BLEDevice::createServer();
  // 创建设备信息服务
  BLEService *pDeviceInfoService = pServer->createService(BLEUUID((uint16_t)DEVICE_INFO_SERVICE_UUID));
  BLECharacteristic *pDeviceNameChar = pDeviceInfoService->createCharacteristic(
    BLEUUID((uint16_t)DEVICE_NAME_UUID),
    BLECharacteristic::PROPERTY_READ
  );
  pDeviceNameChar->setValue(deviceName.c_str());
  pDeviceInfoService->start();

  // 创建定位服务
  pService = pServer->createService(BLEUUID((uint16_t)LOCATION_SERVICE_UUID));

  // 创建特性
  pTimeCharacteristic = pService->createCharacteristic(
    BLEUUID((uint16_t)LOCAL_TIME_UUID),
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );

  pLocationCharacteristic = pService->createCharacteristic(
    BLEUUID((uint16_t)LATITUDE_UUID),
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );

  pModeCharacteristic = pService->createCharacteristic(
    BLEUUID((uint16_t)LONGITUDE_UUID), 
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );

  // 启动服务
  pService->start();
  
  // 开始广播
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(BLEUUID((uint16_t)DEVICE_INFO_SERVICE_UUID));
  pAdvertising->addServiceUUID(BLEUUID((uint16_t)LOCATION_SERVICE_UUID));
  pAdvertising->start();

  // 打印设备名称
  Serial.print("BLE Device Name: ");
  Serial.println(deviceName);
}

void BLEManager::updateTime(unsigned long time) {
  pTimeCharacteristic->setValue(String(time).c_str());
  pTimeCharacteristic->notify();
}

void BLEManager::updateLocation(String location) {
  // 分割经纬度字符串
  int commaIndex = location.indexOf(',');
  String latitude = location.substring(0, commaIndex);
  String longitude = location.substring(commaIndex + 1);
  
  pLocationCharacteristic->setValue(latitude.c_str());
  pLocationCharacteristic->notify();
  
  pModeCharacteristic->setValue(longitude.c_str());
  pModeCharacteristic->notify();
}