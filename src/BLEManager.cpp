#include "BLEManager.h"

// 定义BLE服务和特性UUID
#define DEVICE_INFO_SERVICE_UUID 0x180A
#define DEVICE_NAME_UUID 0x2A00
#define LOCATION_SERVICE_UUID 0x1819
#define LATITUDE_UUID 0x2AAE
#define LONGITUDE_UUID 0x2AAF
#define ALTITUDE_UUID 0x2AB3
#define SATELLITES_UUID "9949ccda-9bbf-4a7d-b269-b5f0ac9f309b"
#define STATUS_UUID "9949ccda-9bbf-4a7d-b269-b5f0ac9f309c"
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

  pLatitudeCharacteristic = pService->createCharacteristic(
    BLEUUID((uint16_t)LATITUDE_UUID),
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );

  pLongitudeCharacteristic = pService->createCharacteristic(
    BLEUUID((uint16_t)LONGITUDE_UUID),
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );

  pAltitudeCharacteristic = pService->createCharacteristic(
    BLEUUID((uint16_t)ALTITUDE_UUID),
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );

  pSatellitesCharacteristic = pService->createCharacteristic(
    BLEUUID(SATELLITES_UUID),
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );

  pStatusCharacteristic = pService->createCharacteristic(
    BLEUUID(STATUS_UUID),
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

void BLEManager::updateLocation(float latitude, float longitude, float altitude) {
  String latitudeStr = String(latitude, 6);
  String longitudeStr = String(longitude, 6);
  String altitudeStr = String(altitude, 6);

  pLatitudeCharacteristic->setValue(latitudeStr.c_str());
  pLatitudeCharacteristic->notify();

  pLongitudeCharacteristic->setValue(longitudeStr.c_str());
  pLongitudeCharacteristic->notify();

  pAltitudeCharacteristic->setValue(altitudeStr.c_str());
  pAltitudeCharacteristic->notify();
}

void BLEManager::updateSatellites(int satellites) {
  pSatellitesCharacteristic->setValue(String(satellites).c_str());
  pSatellitesCharacteristic->notify();
}

void BLEManager::updateStatus(unsigned char status) {
  pStatusCharacteristic->setValue(String(status).c_str());
  pStatusCharacteristic->notify();
}
