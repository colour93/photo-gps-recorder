#include "BLEManager.h"

BLEManager::BLEManager() {}

void BLEManager::begin()
{
  String wifiMac = this->getWifiMac();
  String deviceId = wifiMac.substring(wifiMac.length() - 5);
  deviceId.replace(":", "");
  String deviceName = String("F93_PGR1_" + deviceId);

  // 初始化 BLE 设备
  BLEDevice::init(deviceName.c_str());
  pServer = BLEDevice::createServer();

  BLEAddress address = BLEDevice::getAddress();
  String macSuffix = String(address.toString().substr(9, 8).c_str());

  // 创建服务和特征
  pService = pServer->createService(BLEUUID((uint16_t)0x1810));
  pCharacteristic = pService->createCharacteristic(
      BLEUUID((uint16_t)0x2A1C), // 特征 UUID
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);

  // 启动服务
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->setScanResponse(true);
  pAdvertising->start();

  // 打印设备名称
  Serial.print("BLE Device Name: ");
  Serial.println(deviceName);
}

String BLEManager::getWifiMac()
{
  uint8_t mac[6];
  esp_wifi_get_mac(WIFI_IF_STA, mac);

  // 将 MAC 地址转换为字符串并返回
  String macAddress = "";
  for (int i = 0; i < 6; i++)
  {
    if (mac[i] < 16)
    {
      macAddress += "0";
    }
    macAddress += String(mac[i], HEX);
    if (i < 5)
      macAddress += ":";
  }

  return macAddress;
}
