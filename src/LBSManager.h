#ifndef LBSMANAGER_H
#define LBSMANAGER_H

#include <Arduino.h>

class LBSManager {
public:
    // 构造函数
    LBSManager(int rxPin, int txPin);
    
    // 初始化 LBS 模块
    void begin();
    
    // 获取信号强度
    int getSignalStrength();
    
    // 检查网络状态
    bool isNetworkConnected();
    
    // 设置休眠状态
    void setSleepMode(bool enable);
    
    // 获取休眠状态
    bool getSleepMode();
    
    // 设置LBS服务token
    void setLBSToken(String token);
    
    // 获取LBS位置
    bool getLBSLocation(double &latitude, double &longitude);

private:
    int rxPin;                // RX 引脚
    int txPin;                // TX 引脚
    HardwareSerial serialLBS; // Serial 对象
    String lbsToken;          // LBS 服务 token
};

#endif // LBSMANAGER_H
