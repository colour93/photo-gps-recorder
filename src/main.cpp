#include <Ticker.h>
#include <ArduinoJson.h>
#include "BLEManager.h"
#include "PowerManager.h"
#include "GPSManager.h"
#include "LBSManager.h"
#include "StorageManager.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define ENABLE_OLED 1
#define ENABLE_GPS 1
#define ENABLE_BATTERY 0
#define ENABLE_LBS 0

#define GPS_TX_PIN 4
#define GPS_RX_PIN 3
#define GPS_BAUD 115200

#define LBS_TX_PIN 5
#define LBS_RX_PIN 6

#define BATTERY_VOLTAGE_PIN 35

#define OLED_SDA 0
#define OLED_SCL 1
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define STATUS_BAR_HEIGHT 8
#define STATUS_BAR_WIDTH SCREEN_WIDTH
#define STATUS_BAR_X 0
#define STATUS_BAR_Y SCREEN_HEIGHT - STATUS_BAR_HEIGHT

#define STORAGE_TYPE STORAGE_SPIFFS

#define STORAGE_DATA_PATH "/data.csv"
#define STORAGE_CONFIG_PATH "/config.json"

int samplingInterval = 10;

int debugMode = 0;

BLEManager bleManager;
GPSManager gpsManager(GPS_RX_PIN, GPS_TX_PIN, GPS_BAUD);
PowerManager powerManager(BATTERY_VOLTAGE_PIN);
LBSManager lbsManager(LBS_RX_PIN, LBS_TX_PIN);
StorageManager storageManager(STORAGE_TYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Ticker timer;

void updateData();
void updateDataDisplay();
void updateStatusBar();
void perSecondTask();
void updateGPSData();
JsonDocument loadConfig();
void setup()
{
  // 服务加载阶段
  Serial.begin(115200);

  Serial.println("▗▄▄▄▖▄▄▄▄ ▄▄▄▄ \n▐▌   █  █    █ \n▐▛▀▀▘▀▀▀█ ▀▀▀█ \n▐▌   ▄▄▄█ ▄▄▄█ \nx@fur93.icu\nGitHub: @colour93\nProduct Name: F93-PGR1 GPS Recorder");

  Serial.println("Starting...");

  if (ENABLE_OLED)
  {
    Wire.begin(OLED_SDA, OLED_SCL);
    if (display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
      Serial.println("OLED Module Enabled");
    }
    else
    {
      Serial.println("OLED Module Failed");
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("F93-PGR1 GPS Recorder");
  }

  bleManager.begin();

  if (ENABLE_BATTERY)
  {
    powerManager.begin();
    if (ENABLE_OLED)
      display.println("Battery Module Enabled");
  }
  if (ENABLE_GPS)
  {
    gpsManager.begin();
    if (ENABLE_OLED)
      display.println("GPS Module Enabled");
  }
  if (ENABLE_LBS)
  {
    lbsManager.begin();
    if (ENABLE_OLED)
      display.println("LBS Module Enabled");
  }

  if (ENABLE_OLED)
    display.display();

  if (!storageManager.begin())
  {
    Serial.println("Storage Mount Failed");
    return;
  }

  if (storageManager.initialized)
  {
    JsonDocument config = loadConfig();
    if (!config["samplingInterval"].isNull())
    {
      samplingInterval = config["samplingInterval"].as<int>();
      Serial.println("Loaded sampling interval: " + String(samplingInterval));
    }
    if (!config["debugMode"].isNull())
    {
      debugMode = config["debugMode"].as<int>();
      Serial.println("Loaded debug mode: " + String(debugMode));
    }
  }

  // 任务启动阶段
  if (ENABLE_GPS)
  {
    while (!gpsManager.available())
    {
      Serial.println("GPS Not Available");
      delay(1000);
    }
    timer.attach(1, perSecondTask);
  }
}

JsonDocument loadConfig()
{
  String configStr = storageManager.readData(STORAGE_CONFIG_PATH);
  JsonDocument config;
  if (configStr != "")
  {
    DeserializationError error = deserializeJson(config, configStr);
    if (error)
      Serial.println("Failed to parse config file");
  }
  else
  {
    Serial.println("No config file found, using default config");
  }
  return config;
}

void saveConfig()
{
  JsonDocument rawConfig = loadConfig();
  rawConfig["samplingInterval"] = samplingInterval;
  rawConfig["debugMode"] = debugMode;

  String configStr;
  serializeJson(rawConfig, configStr);
  storageManager.writeData(STORAGE_CONFIG_PATH, configStr);
}

void perSecondTask()
{
  if (ENABLE_OLED)
    updateStatusBar();
  if ((millis() / 1000) % samplingInterval == 0)
    updateData();
}

void updateDataDisplay()
{
  display.fillRect(0, 0, SCREEN_WIDTH, 8 * 5, SSD1306_BLACK);
  display.setCursor(0, 0);
  display.println(gpsManager.getTimeString());
  display.println(String(gpsManager.gps.location.lat(), 6) + "," + String(gpsManager.gps.location.lng(), 6));
  display.println("Altitude: " + String(gpsManager.gps.altitude.meters()));
  display.println("Satellites: " + String(gpsManager.gps.satellites.value()));
  unsigned char status = gpsManager.isDataReady();
  String statusString = "";
  if (status == 0)
    statusString = "Not ";
  if (status & 0b001)
    statusString += "L ";
  if (status & 0b010)
    statusString += "T ";
  if (status & 0b100)
    statusString += "D ";
  statusString += "Ready";
  display.println("Status: " + statusString);
  display.display();
}

void updateStatusBar()
{
  display.fillRect(STATUS_BAR_X, STATUS_BAR_Y, STATUS_BAR_WIDTH, STATUS_BAR_HEIGHT, SSD1306_BLACK);
  display.setCursor(STATUS_BAR_X, STATUS_BAR_Y);
  display.println(String(samplingInterval - (millis() / 1000) % samplingInterval) + "/" + String(samplingInterval) + " s");
  display.display();
}

void updateGPSData()
{
  if (!gpsManager.available())
  {
    Serial.println("GPS Not Available");
    return;
  }

  gpsManager.wake();

  if (debugMode)
    Serial.println("Updating Data...");

  gpsManager.update();

  if (debugMode)
  {
    Serial.println("Time: " + String(gpsManager.gps.time.value()));
    Serial.println("Location: " + String(gpsManager.gps.location.lat(), 6) + "," + String(gpsManager.gps.location.lng(), 6));
    Serial.println("Altitude: " + String(gpsManager.gps.altitude.meters()));
    Serial.println("Satellites: " + String(gpsManager.gps.satellites.value()));
  }

  // 将数据广播
  bleManager.updateTime(gpsManager.getTime());
  bleManager.updateLocation(gpsManager.gps.location.lat(), gpsManager.gps.location.lng(), gpsManager.gps.altitude.meters());
  bleManager.updateSatellites(gpsManager.gps.satellites.value());
  bleManager.updateStatus(gpsManager.isDataReady());

  // 更新OLED显示
  if (ENABLE_OLED)
    updateDataDisplay();

  if (gpsManager.isDataReady() == 0b111)
    storageManager.writeData(STORAGE_DATA_PATH, gpsManager.getTimeString() + "," + String(gpsManager.gps.location.lat(), 6) + "," + String(gpsManager.gps.location.lng(), 6) + "," + String(gpsManager.gps.altitude.meters()) + ",GPS");

  if (debugMode)
    Serial.println("Data Updated.");

  gpsManager.sleep();
}

void updateData()
{
  if (ENABLE_GPS)
    updateGPSData();
}

void loop()
{
  if (Serial.available())
  {
    String command = Serial.readStringUntil('\n'); // 读取直到换行符
    if (command.equals("AT+DATA=?"))
    {
      String data = storageManager.readData(STORAGE_DATA_PATH);
      if (data != "")
        Serial.println(data);
      else
        Serial.println("No Data.");
    }
    else if (command.equals("AT+CLRDATA=1"))
    {
      if (storageManager.clearData(STORAGE_DATA_PATH))
        Serial.println("Data Cleared.");
      else
        Serial.println("Data Clear Failed.");
    }
    else if (command.equals("AT+CFGINTERVAL=?"))
    {
      Serial.println(String(samplingInterval));
    }
    else if (command.startsWith("AT+CFGINTERVAL="))
    {
      int interval = command.substring(15).toInt();
      if (interval > 0)
      {
        samplingInterval = interval;
        saveConfig();
        Serial.println("Interval Saved.");
      }
      else
        Serial.println("Invalid Interval.");
    }
    else if (command.equals("AT+CFGDEBUG=?"))
    {
      Serial.println(String(debugMode));
    }
    else if (command.startsWith("AT+CFGDEBUG="))
    {
      debugMode = command.substring(12).toInt();
      saveConfig();
      Serial.println("Debug Mode Saved.");
    }
  }
}