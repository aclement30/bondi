#include <ArduinoSTL.h>
#include "constants.h"
#include "meal_distribution.h"
#include "time_service.h"

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
        time_t getTime();
        void getDateTime(char * date);
        vector<MealDistribution> getDistributionHistory();
        vector<int> getDistributedMealIds();
        // int getBufferSize();

    private:
        LogService();

        void writeToFile(char * logLine);
        void typeToText(char * line, FeederEvent type);
        void getSystemLogFilename(char * filename);
        void getDistributionLogFilename(char * filename);
        void createDistributionLogFile(char * filename);

        void operator = (LogService const&); // Don't implement
};

#endif
