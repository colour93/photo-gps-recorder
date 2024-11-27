#include <Ticker.h>
#include "BLEManager.h"
#include "PowerManager.h"
#include "GPSManager.h"
#include "LBSManager.h"

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

int samplingInterval = 10;

BLEManager bleManager;
GPSManager gpsManager(GPS_RX_PIN, GPS_TX_PIN, GPS_BAUD);
PowerManager powerManager(BATTERY_VOLTAGE_PIN);
LBSManager lbsManager(LBS_RX_PIN, LBS_TX_PIN);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Ticker timer;

void updateData();
void updateDataDisplay();
void updateIntervalDisplay();

void setup()
{
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

  timer.attach(1, updateIntervalDisplay);
}

void updateDataDisplay()
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Time: " + gpsManager.getTimeString());
  display.println("Location: " + String(gpsManager.gps.location.lat(), 6) + "," + String(gpsManager.gps.location.lng(), 6));
  display.display();
}

void updateIntervalDisplay()
{
  display.fillRect(0, SCREEN_HEIGHT - 10 * 2, SCREEN_WIDTH, 20, SSD1306_BLACK);
  display.setCursor(0, SCREEN_HEIGHT - 10 * 2);
  display.println("Interval: " + String(samplingInterval) + "s");
  display.println("Next Update: " + String(samplingInterval - (millis() / 1000) % samplingInterval) + "s");
  display.display();
  if ((millis() / 1000) % samplingInterval == 0)
    updateData();
}

void updateData()
{
  if (!gpsManager.available())
  {
    Serial.println("GPS Not Available");
    return;
  }

  Serial.println("Updating Data...");

  gpsManager.update();

  Serial.println("Time: " + String(gpsManager.gps.time.value()));
  Serial.println("Location: " + String(gpsManager.gps.location.lat(), 6) + "," + String(gpsManager.gps.location.lng(), 6));

  // 将数据广播
  bleManager.updateTime(gpsManager.getTime());
  bleManager.updateLocation(String(gpsManager.gps.location.lat(), 6) + "," + String(gpsManager.gps.location.lng(), 6));

  // 更新OLED显示
  if (ENABLE_OLED)
    updateDataDisplay();

  Serial.println("Time: " + gpsManager.getTimeString());
  Serial.println("Location: " + String(gpsManager.gps.location.lat(), 6) + "," + String(gpsManager.gps.location.lng(), 6));

  Serial.println("Data Updated.");
}

void loop()
{
}
