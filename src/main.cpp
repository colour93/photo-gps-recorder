#include "BLEManager.h"
#include "PowerManager.h"
#include "GPSManager.h"

#define ENABLE_GPS 1
#define ENABLE_BATTERY 0

#define GPS_TX_PIN 4
#define GPS_RX_PIN 3

#define BATTERY_VOLTAGE_PIN 35

BLEManager bleManager;
GPSManager gpsManager(GPS_RX_PIN, GPS_TX_PIN);
PowerManager powerManager(BATTERY_VOLTAGE_PIN);

void setup()
{
  Serial.begin(115200);

  Serial.println("▗▄▄▄▖▄▄▄▄ ▄▄▄▄ \n▐▌   █  █    █ \n▐▛▀▀▘▀▀▀█ ▀▀▀█ \n▐▌   ▄▄▄█ ▄▄▄█ \nx@fur93.icu\nGitHub: @colour93\nProduct Name: F93-PGR1 GPS Recorder");

  Serial.println("Starting...");

  bleManager.begin();

  if (ENABLE_BATTERY)
    powerManager.begin();
  if (ENABLE_GPS)
  {
    gpsManager.begin();
  }
}

void loop()
{
  if (ENABLE_GPS)
  {
    if (gpsManager.available())
    {
      gpsManager.update();
      Serial.println("GPS Time: " + String(gpsManager.getTime()));
    }
  }
  delay(500);
}