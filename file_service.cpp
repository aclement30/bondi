#include <SD.h>
#include "constants.h"
#include "string.h"
#include "file_service.h"

FileService::FileService() {
    if (!SD.begin(SD_CARD_CS)) {
        Serial.println(F("Erreur lors de l'initialisation du lecteur de carte SD"));
    }
}

bool FileService::validateSDCard() {
    // Serial.println(F("Start SD card check"));
    
    // Check if a SD card is present
    Sd2Card card;
    if (!card.init(SPI_HALF_SPEED, SD_CARD_CS)) {
        // Serial.println(F("No valid SD card found!"));
        return false;
    }

    // Serial.println(F("SD card found"));

    if (!SD.begin(SD_CARD_CS)) {
        // Serial.println(F("SD card initialization failed!"));
        return false;
    }

    return true;
}

bool FileService::validateConfigFiles() {
    File root = SD.open("/");

    // Serial.println(F("Listing files"));

    int matchingFilesCount = 0;
    int configFilesCount = 3;
    char * configFiles[3] = {
        getString(FILE_MEALS),
        getString(FILE_POINTS),
        getString(FILE_MEAL_SEQUENCES)
    };

    char * filename;
    
    while (true) {
        File entry = root.openNextFile();
        if (!entry) {
            break;  // no more files
        }

        // Find matching filename in config files list
        filename = entry.name();
        for (int i = 0; i < configFilesCount; i++) {
            if (strcmp(configFiles[i], filename) == 0) {
                matchingFilesCount += 1;
            }
        }
        
        entry.close();

        // Stop here if we already have the count
        if (matchingFilesCount == configFilesCount) {
            break;
        }
    }

    if (matchingFilesCount != configFilesCount) {
        // Serial.println(F("Missing config files"));
        return false;
    }

    return true;
}

File FileService::openFile(const char * filename) {
    return SD.open(filename);
}

bool FileService::readLine(File & file, char * line, size_t maxLen) {
  for (size_t n = 0; n < maxLen; n++) {
    int c = file.read();
    if ( c < 0 && n == 0) return false;  // EOF
    if (c < 0 || c == '\n') {
      line[n] = 0;
      return true;
    }
    line[n] = c;
  }
  
  return false; // line too long
}

void FileService::splitLine(char * line, char * segments[]) {
    int i = 0;
    char * segment;
    char * string = strdup(line);
    
    while ((segment = strsep(&string, ";")) != NULL) {
        segments[i] = segment;
        i += 1;
    }
}