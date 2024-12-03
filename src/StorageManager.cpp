#include "StorageManager.h"

StorageManager::StorageManager(StorageType type)
{
  storageType = type;
  initialized = false;
  sd_cs = -1;
  sd_sck = -1;
  sd_mosi = -1;
  sd_miso = -1;
}

StorageManager::StorageManager(StorageType type, int8_t cs, int8_t sck, int8_t mosi, int8_t miso)
{
  storageType = type;
  initialized = false;
  sd_cs = cs;
  sd_sck = sck;
  sd_mosi = mosi;
  sd_miso = miso;
}

bool StorageManager::begin()
{
  if (storageType == STORAGE_SPIFFS)
  {
    if (!SPIFFS.begin(true))
    {
      Serial.println("SPIFFS Init Failed");
      return false;
    }
    initialized = true;
  }
  else
  {
    if (sd_cs != -1) {
      if (sd_sck != -1 && sd_mosi != -1 && sd_miso != -1) {
        SPI.begin(sd_sck, sd_miso, sd_mosi, sd_cs);
      }
      if (!SD.begin(sd_cs)) {
        Serial.println("SD Card Init Failed");
        return false;
      }
    } else {
      if (!SD.begin()) {
        Serial.println("SD Card Init Failed");
        return false;
      }
    }
    initialized = true;
  }
  return true;
}

bool StorageManager::writeData(String path, String data)
{
  if (!initialized)
  {
    Serial.println("Storage Not Initialized");
    return false;
  }

  File file;
  if (storageType == STORAGE_SPIFFS)
  {
    file = SPIFFS.open(path.c_str(), FILE_APPEND);
  }
  else
  {
    file = SD.open(path.c_str(), FILE_APPEND);
  }

  if (!file)
  {
    Serial.println("File " + path + " Open Failed");
    return false;
  }

  file.println(data);
  file.close();
  return true;
}

String StorageManager::readData(String path)
{
  if (!initialized)
  {
    Serial.println("Storage Not Initialized");
    return "";
  }

  File file;
  if (storageType == STORAGE_SPIFFS)
  {
    file = SPIFFS.open(path.c_str(), FILE_READ);
  }
  else
  {
    file = SD.open(path.c_str(), FILE_READ);
  }

  if (!file)
  {
    Serial.println("File " + path + " Open Failed");
    return "";
  }

  String data = file.readString();
  file.close();
  return data;
}

bool StorageManager::clearData(String path)
{
  if (storageType == STORAGE_SPIFFS)
  {
    return SPIFFS.remove(path.c_str());
  }
  else
  {
    return SD.remove(path.c_str());
  }

}