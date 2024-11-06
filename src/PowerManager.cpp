#include "PowerManager.h"
#include <Arduino.h>

// 构造函数，设置引脚
PowerManager::PowerManager(int batteryVoltagePin)
    : batteryVoltagePin(batteryVoltagePin) {}

// 初始化引脚模式
void PowerManager::begin()
{
    Serial.println("Power manager is enabled");
    pinMode(batteryVoltagePin, INPUT);
}

// 获取电池电压
float PowerManager::getBatteryVoltage()
{
    int rawValue = analogRead(batteryVoltagePin);
    return (rawValue / 4095.0) * 3.3 * (147.0 / 47.0); // 使用分压系数计算
}