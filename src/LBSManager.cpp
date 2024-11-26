#include "LBSManager.h"

// 构造函数，指定 RX 和 TX 引脚，并初始化 Serial 对象
LBSManager::LBSManager(int rxPin, int txPin)
    : rxPin(rxPin), txPin(txPin), serialLBS(1) {}

// 初始化 LBS 模块
void LBSManager::begin()
{
  Serial.println("LBS manager is enabled");
  serialLBS.begin(115200, SERIAL_8N1, rxPin, txPin);
  
  // 初始化模块
  delay(1000);
  serialLBS.println("AT");
  delay(100);
  serialLBS.println("AT+CPIN?"); // 检查SIM卡
  delay(100);
}

// 获取信号强度
int LBSManager::getSignalStrength()
{
  serialLBS.println("AT+CSQ");
  delay(100);
  if(serialLBS.available()) {
    String response = serialLBS.readString();
    Serial.println(response);
    // 解析CSQ响应获取信号强度值
    int start = response.indexOf(": ") + 2;
    int end = response.indexOf(",");
    if(start > 0 && end > 0) {
      return response.substring(start, end).toInt();
    }
  }
  return -1;
}

// 检查网络状态
bool LBSManager::isNetworkConnected() 
{
  serialLBS.println("AT+CREG?");
  delay(100);
  if(serialLBS.available()) {
    String response = serialLBS.readString();
    return response.indexOf(",1") > 0 || response.indexOf(",5") > 0;
  }
  return false;
}

// 设置休眠状态
void LBSManager::setSleepMode(bool enable)
{
  if(enable) {
    serialLBS.println("AT+QSCLK=1");
  } else {
    serialLBS.println("AT+QSCLK=0"); 
  }
  delay(100);
}

// 获取休眠状态
bool LBSManager::getSleepMode()
{
  serialLBS.println("AT+QSCLK?");
  delay(100);
  if(serialLBS.available()) {
    String response = serialLBS.readString();
    return response.indexOf(": 1") > 0;
  }
  return false;
}

// 设置LBS服务token
void LBSManager::setLBSToken(String token)
{
  this->lbsToken = token;
}

// 获取LBS位置
bool LBSManager::getLBSLocation(double &latitude, double &longitude)
{
  // 发送AT指令获取基站信息
  serialLBS.println("AT+QCELLLOC");
  delay(1000);
  
  if(serialLBS.available()) {
    String response = serialLBS.readString();
    
    // 解析返回的经纬度信息
    int start = response.indexOf(": ") + 2;
    int comma = response.indexOf(",", start);
    if(start > 0 && comma > 0) {
      latitude = response.substring(start, comma).toDouble();
      longitude = response.substring(comma + 1).toDouble();
      return true;
    }
  }
  
  return false;
}
