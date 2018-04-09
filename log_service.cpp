#include <ArduinoSTL.h>
#include <RTClib.h>
#include "constants.h"
#include "file_service.h"
#include "log_service.h"
#include "string.h"

using namespace std;

LogService & LogService::getInstance() {
    static LogService instance;    // Guaranteed to be destroyed.
                                        // Instantiated on first use.
    return instance;
}

void LogService::log(FeederEvent eventType, char * info) {
    char line[120];

    timestampToDate(line, rtc.now());
    strcat(line, " ");
    typeToText(line, eventType);
    if (info != NULL) {
        strcat(line, " ");
        strcat(line, info);
    }

    writeToFile(line);
}

void LogService::log(FeederEvent eventType, const char * info) {
    log(eventType, info);
}

void LogService::log(FeederEvent eventType, int info) {
    char line[120];
    
    timestampToDate(line, rtc.now());
    strcat(line, " ");
    typeToText(line, eventType);
    strcat(line, " ");
    strcat(line, info);

    writeToFile(line);
}

void LogService::flush() {
    // FileService fileService = FileService();
    // File logFile = fileService.openFile(getString(FILE_LOGS), FILE_WRITE);
    
    // char line[120];
    // for (auto const & event : eventsBuffer) {
    //     timestampToDate(line, event.timestamp);
    //     strcat(line, " ");
    //     typeToText(line, event.type);
    //     strcat(line, " ");
    //     strcat(line, event.info);

    //     // Write log line to file
    //     logFile.println(line);
    //     Serial.println(line);

    //     // Clear line
    //     line[0] = 0;
    // }

    // logFile.close();
    // eventsBuffer.clear();
}

int LogService::getBufferSize() {
    return eventsBuffer.size();
}

// PRIVATE

LogService::LogService() : rtc(RTC_DS1307()) {
    rtc.begin();
}

void LogService::writeToFile(char * logLine) {
    FileService fileService = FileService();
    File logFile = fileService.openFile(getString(FILE_LOGS), FILE_WRITE);

    logFile.println(logLine);
    Serial.println(logLine);

    logFile.close();
}

void LogService::timestampToDate(char * date, DateTime timestamp) {
  int year = timestamp.year();
  int month = timestamp.month();
  int day = timestamp.day();
  int hour = timestamp.hour();
  int minute = timestamp.minute();
  int second = timestamp.second();

  sprintf(date, "%d-%d-%d %02d:%02d:%02d", year, month, day, hour, minute, second);
}

void LogService::typeToText(char * line, FeederEvent type) {
    const static char type0[] PROGMEM = "STARTUP";
    const static char type1[] PROGMEM = "MEAL_DISTRIBUTION_START";
    const static char type2[] PROGMEM = "MEAL_DISTRIBUTION_END";
    const static char type3[] PROGMEM = "MANUAL_MEAL_SELECTION";
    const static char type4[] PROGMEM = "ROUTE_START";
    const static char type5[] PROGMEM = "ROUTE_END";
    const static char type6[] PROGMEM = "SAFETY_BAR_PRESSED";
    const static char type7[] PROGMEM = "SAFETY_MODE_ENGAGED";
    const static char type8[] PROGMEM = "SAFETY_MODE_DISENGAGED";
    const static char type9[] PROGMEM = "MANUAL_CONTROL_START";
    const static char type10[] PROGMEM = "MANUAL_CONTROL_END";
    const static char type11[] PROGMEM = "MANUAL_RESTART";

    const char * const textTypes[] PROGMEM = {
        type0,
        type1,
        type2,
        type3,
        type4,
        type5,
        type6,
        type7,
        type8,
        type9,
        type10,
        type11
    };

    strcat(line, getString(textTypes[type]));
}