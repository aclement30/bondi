#include <ArduinoSTL.h>
#include <RTClib.h>
#include "constants.h"

#ifndef LOG_SERVICE_H
#define LOG_SERVICE_H

using namespace std;

struct LogEvent {
    DateTime timestamp;
    FeederEvent type;
    char * info;
};

class LogService {
    public:
        static LogService & getInstance();
        void log(FeederEvent eventType, char * info = NULL);
        void log(FeederEvent eventType, const char * info);
        void log(FeederEvent eventType, int info);
        void flush();
        int getBufferSize();

    private:
        vector<LogEvent> eventsBuffer;
        RTC_DS1307 rtc;

        LogService();

        void writeToFile(char * logLine);
        void timestampToDate(char * date, DateTime timestamp);
        void typeToText(char * line, FeederEvent type);

        void operator = (LogService const&); // Don't implement
};

#endif
