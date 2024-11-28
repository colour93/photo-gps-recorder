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
  
public:
  StorageManager(StorageType type = STORAGE_SPIFFS);

  bool initialized;

  bool begin();
  bool writeData(String path, String data);
  String readData(String path);
  bool clearData(String path);
};

#endif
