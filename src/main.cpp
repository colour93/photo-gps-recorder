#include <Arduino.h>
#include "BluetoothManager.h"
#include "PowerManager.h"
#include "GPSManager.h"

#define ENABLE_GPS 0
#define ENABLE_BATTERY 0

#define GPS_TX_PIN 17
#define GPS_RX_PIN 18

#define BATTERY_VOLTAGE_PIN 35

BluetoothManager bluetoothManager;
GPSManager gpsManager(GPS_RX_PIN, GPS_TX_PIN);
PowerManager powerManager(BATTERY_VOLTAGE_PIN);

void setup()
{
  Serial.begin(115200);

  Serial.println("▗▄▄▄▖▄▄▄▄ ▄▄▄▄ \n▐▌   █  █    █ \n▐▛▀▀▘▀▀▀█ ▀▀▀█ \n▐▌   ▄▄▄█ ▄▄▄█ \nx@fur93.icu\nGitHub: @colour93\nProduct Name: F93-GR1 GPS Recorder");

  Serial.println("Starting...");

  bluetoothManager.begin();

  if (ENABLE_BATTERY)
    powerManager.begin();
  if (ENABLE_GPS)
    gpsManager.begin();
}

void loop()
{
}