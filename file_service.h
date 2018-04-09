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

        bool validateSDCard();
        bool validateConfigFiles();
        File openFile(const char * filename, uint8_t mode = FILE_READ);
        static bool readLine(File & file, char * line, size_t maxLen);
        static void splitLine(char * line, char * segments[]);
};

#endif
