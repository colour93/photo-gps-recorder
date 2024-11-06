#ifndef BLUETOOTHMANAGER_H
#define BLUETOOTHMANAGER_H

#include <BluetoothSerial.h>
#include <esp_bt_device.h>
#include <Arduino.h>

class BluetoothManager
{
public:
  // 构造函数
  BluetoothManager();

  // 初始化蓝牙并设置设备名称
  void begin();

  // 获取蓝牙 MAC 地址的字符串格式
  String getMac();

private:
  BluetoothSerial serialBT; // BluetoothSerial 实例，用于蓝牙通信
};

#endif // BLUETOOTHMANAGER_H