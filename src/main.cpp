#include <Ticker.h>
#include "BLEManager.h"
#include "PowerManager.h"
#include "GPSManager.h"
#include "LBSManager.h"

#define ENABLE_GPS 1
#define ENABLE_BATTERY 0
#define ENABLE_LBS 0

#define GPS_TX_PIN 4
#define GPS_RX_PIN 3

#define LBS_TX_PIN 5
#define LBS_RX_PIN 6

#define BATTERY_VOLTAGE_PIN 35

int samplingInterval = 10;

BLEManager bleManager;
GPSManager gpsManager(GPS_RX_PIN, GPS_TX_PIN);
PowerManager powerManager(BATTERY_VOLTAGE_PIN);
LBSManager lbsManager(LBS_RX_PIN, LBS_TX_PIN);

Ticker timer;

void updateData();

void setup()
{
  Serial.begin(115200);

  Serial.println("▗▄▄▄▖▄▄▄▄ ▄▄▄▄ \n▐▌   █  █    █ \n▐▛▀▀▘▀▀▀█ ▀▀▀█ \n▐▌   ▄▄▄█ ▄▄▄█ \nx@fur93.icu\nGitHub: @colour93\nProduct Name: F93-PGR1 GPS Recorder");

  Serial.println("Starting...");

  bleManager.begin();

  if (ENABLE_BATTERY)
    powerManager.begin();
  if (ENABLE_GPS)
    gpsManager.begin();
  if (ENABLE_LBS)
    lbsManager.begin();

  timer.attach(samplingInterval, updateData);
}

void updateData() {
  if (!gpsManager.available())
    return;

  Serial.println("Updating Data...");

  gpsManager.update();

  // 将数据广播
  bleManager.updateTime(gpsManager.getTime());
  bleManager.updateLocation(String(gpsManager.getLatitude(), 6) + "," + String(gpsManager.getLongitude(), 6));

  Serial.println("Time: " + gpsManager.getTimeString());
  Serial.println("Location: " + String(gpsManager.getLatitude(), 6) + "," + String(gpsManager.getLongitude(), 6));

  Serial.println("Data Updated.");
}

void loop()
{
}