#include <ArduinoSTL.h>
#include <RTClib.h>
#include "constants.h"
#include "config.h"
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

void LogService::logDistribution(int mealId, char * startTime, char * endTime, char * missingSequences, int safetyStops) {
    char filename[30];
    getDistributionLogFilename(filename);

    // Create CSV log file if it does not already exists
    createDistributionLogFile(filename);

    char line[120];
    sprintf(line, "%s;%d;%d;%s;%s;%s;%d", getString(APP_VERSION), AppConfig::getInstance().configVersion, mealId, startTime, endTime, missingSequences, safetyStops);

    FileService fileService = FileService();
    File logFile = fileService.openFile(filename, FILE_WRITE);
    logFile.println(line);
    logFile.close();
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

void LogService::getTime(char * time) {
    DateTime today = rtc.now();
    int hour = today.hour();
    int minute = today.minute();
    int second = today.second();

    sprintf(time, "%02d:%02d:%02d", hour, minute, second);
}

int LogService::getBufferSize() {
    return eventsBuffer.size();
}

// PRIVATE

LogService::LogService() : rtc(RTC_DS1307()) {
    rtc.begin();
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
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

  sprintf(date, "%d-%02d-%02dT%02d:%02d:%02d-05:00", year, month, day, hour, minute, second);
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

void LogService::getDistributionLogFilename(char * filename) {
    DateTime today = rtc.now();
    int year = today.year();
    int month = today.month();
    int day = today.day();

    sprintf(filename, "%s/%d-%02d-%02d.CSV", getString(PATH_DISTRIB_LOGS), year, month, day);
}

void LogService::createDistributionLogFile(char * filename) {
    FileService fileService = FileService();

    // Skip if file already exists
    if (fileService.fileExists(filename)) return;

    const static char header[] PROGMEM = "appVersion;configVersion;mealId;startTime;endTime;missingSequences;safetyStops";

    File logFile = fileService.openFile(filename, FILE_WRITE);
    logFile.println(getString(header));
    logFile.close();
}
