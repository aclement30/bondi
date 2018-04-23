#include <ArduinoSTL.h>
#include <DS1302RTC.h>
#include <Time.h>
#include "constants.h"
#include "meal_distribution.h"

#ifndef LOG_SERVICE_H
#define LOG_SERVICE_H

using namespace std;

// struct LogEvent {
//     Time timestamp;
//     FeederEvent type;
//     char * info;
// };

class LogService {
    public:
        static LogService & getInstance();
        void log(FeederEvent eventType, char * info = NULL);
        void log(FeederEvent eventType, const char * info);
        void log(FeederEvent eventType, int info);
        void logDistribution(int mealId, time_t startTime, time_t endTime, char * missingSequences);
        void flush();
        time_t getTime();
        vector<MealDistribution> getDistributionHistory();
        vector<int> getDistributedMealIds();
        // int getBufferSize();

    private:
        // vector<LogEvent> eventsBuffer;
        DS1302RTC rtc;

        LogService();

        void writeToFile(char * logLine);
        void timestamp(char * date);
        void typeToText(char * line, FeederEvent type);
        void getDistributionLogFilename(char * filename);
        void createDistributionLogFile(char * filename);

        void operator = (LogService const&); // Don't implement
};

#endif
