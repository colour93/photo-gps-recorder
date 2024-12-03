#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <FS.h>
#include <SPIFFS.h>
#include <SD.h>

enum StorageType {
  STORAGE_SPIFFS,
  STORAGE_SD 
};

class StorageManager {
private:
  StorageType storageType;
  int8_t sd_cs;
  int8_t sd_sck;
  int8_t sd_mosi;
  int8_t sd_miso;

public:
  StorageManager(StorageType STORAGE_SPIFFS);
  StorageManager(StorageType STORAGE_SD, int8_t cs, int8_t sck = -1, int8_t mosi = -1, int8_t miso = -1);

  bool initialized;
  bool begin();
  bool writeData(String path, String data);
  String readData(String path);
  bool clearData(String path);
};

#endif
