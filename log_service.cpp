#include <ArduinoSTL.h>
#include <RTClib.h>
#include "constants.h"
#include "file_service.h"
#include "log_service.h"

using namespace std;

LogService & LogService::getInstance() {
    static LogService instance;    // Guaranteed to be destroyed.
                                        // Instantiated on first use.
    return instance;
}

void LogService::log(FeederEvent eventType, char * info) {
    LogEvent event = {
        rtc.now(),
        eventType,
        info
    };

    eventsBuffer.push_back(event);
}

void LogService::flush() {
    FileService fileService = FileService();
    File logFile = fileService.openFile(getString(FILE_LOGS), FILE_WRITE);
    
    char logLine[120];
    for (auto const & event : eventsBuffer) {
        timestampToDate(logLine, event.timestamp);
        strcat(logLine, " ");
        typeToText(logLine, event.type);
        strcat(logLine, " ");
        strcat(logLine, event.info);

        // Write log line to file
        dataFile.println(logLine);

        // Clear line
        logLine[0] = 0;
    }

    eventsBuffer.clear();
}

// PRIVATE

LogService::LogService() : rtc(RTC_DS1307()) {
    rtc.begin();
}


void LogService::timestampToDate(char * date, DateTime timestamp) {
  int year = timestamp.year();
  int month = timestamp.month();
  int day = timestamp.day();
  int hour = timestamp.hour();
  int minute = timestamp.minute();
  int second = timestamp.second();

  sprintf(date, "%d-%d-%d %02d:%02d:%02d ", year, month, day, , hour, minute, second);
}

void LogService::typeToText(char * line, FeederEvent type) {
    switch (type) {
        case STARTUP: {
            const static char typeText[] PROGMEM = "STARTUP";
            strcat(line, typeText);
            break;
        }
        case MEAL_DISTRIBUTION_START: {
            const static char typeText[] PROGMEM = "MEAL_DISTRIBUTION_START";
            strcat(line, typeText);
            break;
        }
        case MEAL_DISTRIBUTION_END: {
            const static char typeText[] PROGMEM = "MEAL_DISTRIBUTION_END";
            strcat(line, typeText);
            break;
        }
        case MANUAL_MEAL_SELECTION: {
            const static char typeText[] PROGMEM = "MANUAL_MEAL_SELECTION";
            strcat(line, typeText);
            break;
        }
        case ROUTE_START: {
            const static char typeText[] PROGMEM = "ROUTE_START";
            strcat(line, typeText);
            break;
        }
        case ROUTE_END: {
            const static char typeText[] PROGMEM = "ROUTE_END";
            strcat(line, typeText);
            break;
        }
        case SAFETY_BAR_PRESSED: {
            const static char typeText[] PROGMEM = "SAFETY_BAR_PRESSED";
            strcat(line, typeText);
            break;
        }
        case SAFETY_MODE_ENGAGED: {
            const static char typeText[] PROGMEM = "SAFETY_MODE_ENGAGED";
            strcat(line, typeText);
            break;
        }
        case SAFETY_MODE_DISENGAGED: {
            const static char typeText[] PROGMEM = "SAFETY_MODE_DISENGAGED";
            strcat(line, typeText);
            break;
        }
        case MANUAL_CONTROL_START: {
            const static char typeText[] PROGMEM = "MANUAL_CONTROL_START";
            strcat(line, typeText);
            break;
        }
        case MANUAL_CONTROL_END: {
            const static char typeText[] PROGMEM = "MANUAL_CONTROL_END";
            strcat(line, typeText);
            break;
        }
        case MANUAL_RESTART: {
            const static char typeText[] PROGMEM = "MANUAL_RESTART";
            strcat(line, typeText);
            break;
        }
    }
}