#include <ArduinoJson.h>
#include <ArduinoSTL.h>
#include <SPI.h>
#include <SD.h>

#ifndef FILE_SERVICE_H
#define FILE_SERVICE_H

using namespace std;

class FileService {
    public:
        FileService();

        File openFile(const char * filename);
};

#endif
