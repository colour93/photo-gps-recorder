#include "BluetoothManager.h"

BluetoothManager::BluetoothManager()
    : serialBT() {}

void BluetoothManager::begin()
{
  // 先启动 BluetoothSerial
  serialBT.begin("F93_GR1_TEMP"); // 临时设备名称，仅用于确保蓝牙子系统启动

  // 获取 Bluetooth MAC 地址
  const uint8_t *btMac = esp_bt_dev_get_address();
  if (!btMac)
  {
    Serial.println("Failed to get Bluetooth MAC address");
    return;
  }

  // 提取 MAC 地址的后 4 位并转换为字符串
  String macSuffix = "";
  for (int i = 4; i < 6; i++)
  {
    macSuffix += String(btMac[i], HEX);
  }
  macSuffix.toUpperCase();

  // 设置设备名称为 "ESP32_" + MAC 后 4 位
  String deviceName = "F93_GR1_" + macSuffix;

  // 重新启动 BluetoothSerial，并使用带有 MAC 后缀的设备名称
  serialBT.end(); // 先关闭再重启，以便重新设置名称
  serialBT.begin(deviceName);

  Serial.print("Bluetooth Device Name: ");
  Serial.println(deviceName);
}

String BluetoothManager::getMac()
{
  const uint8_t *btMac = esp_bt_dev_get_address();
  String macAddress = "";

  for (int i = 0; i < 6; i++)
  {
    if (i > 0)
    {
      macAddress += ":"; // 分隔符
    }
    macAddress += String(btMac[i], HEX);
  }

  macAddress.toUpperCase(); // 转换为大写
  return macAddress;
}