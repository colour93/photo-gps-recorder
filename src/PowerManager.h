#ifndef POWER_MANAGER_H
#define POWER_MANAGER_H

class PowerManager
{
public:
  PowerManager(int batteryVoltagePin);

  // 初始化电源管理
  void begin();

  // 获取电池电压
  float getBatteryVoltage();

private:
  int batteryVoltagePin;
};

#endif