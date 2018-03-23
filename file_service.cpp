#include <SD.h>
#include "constants.h"
#include "file_service.h"

FileService::FileService() {
    if (!SD.begin(SD_CARD_CS)) {
        Serial.println("SD Card initialization failed!");
    }
}

File FileService::openFile(const char * filename) {
    return SD.open(filename);
}