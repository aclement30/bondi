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
    
    for (auto const & event : eventsBuffer) {
        dataFile.print(timestampToDate(event.timestamp));
        dataFile.print(event.type);
        dataFile.println(event.info);
    }

    eventsBuffer.clear();
}

// PRIVATE

LogService::LogService() : rtc(RTC_DS1307()) {
    rtc.begin();
}


char[120] LogService::timestampToDate(DateTime timestamp) {
  char message[120];

  int year = timestamp.year();
  int month = timestamp.month();
  int day = timestamp.day();
  int hour = timestamp.hour();
  int minute = timestamp.minute();
  int second = timestamp.second();

  sprintf(message, "%d-%d-%d %02d:%02d:%02d", year, month, day, , hour, minute, second);
  
  return message;
}