#include <ArduinoSTL.h>
#include "constants.h"
#include "config.h"
#include "file_service.h"
#include "log_service.h"
#include "meal_distribution.h"
#include "string.h"
#include "time_service.h"

using namespace std;

LogService & LogService::getInstance() {
    static LogService instance;    // Guaranteed to be destroyed.
                                        // Instantiated on first use.
    return instance;
}

void LogService::log(FeederEvent eventType, char * info) {
    char line[150];

    TimeService::getInstance().timestamp(line);
    strcat(line, " ");
    typeToText(line, eventType);
    if (info != NULL) {
        strcat(line, " ");
        strcat(line, info);
    }

    writeToFile(line);
}

void LogService::log(FeederEvent eventType, const char * info) {
    char line[150];

    TimeService::getInstance().timestamp(line);
    strcat(line, " ");
    typeToText(line, eventType);
    if (info != NULL) {
        strcat(line, " ");
        strcat(line, info);
    }

    writeToFile(line);
}

void LogService::log(FeederEvent eventType, int info) {
    char line[150];
    
    TimeService::getInstance().timestamp(line);
    strcat(line, " ");
    typeToText(line, eventType);
    strcat(line, " ");
    
    char number[10];
    sprintf(number, "%d", info);
    strcat(line, number);

    writeToFile(line);
}

void LogService::logDistribution(int mealId, time_t startTime, time_t endTime, char * missingSequences) {
    char filename[30];
    getDistributionLogFilename(filename);

    // Create CSV log file if it does not already exists
    createDistributionLogFile(filename);

    char line[150];
    sprintf(line, "%s;%d;%d;%ld;%ld;%s", getString(APP_VERSION), AppConfig::getInstance().configVersion, mealId, startTime, endTime, missingSequences);

    Serial.print("[LOG] (distribution) ");
    Serial.println(line);

    FileService fileService = FileService();
    File logFile = fileService.openFile(filename, FILE_WRITE);
    logFile.println(line);
    logFile.close();
}

// Returns list of distributed meal for the current day
vector<MealDistribution> LogService::getDistributionHistory() {
    vector<MealDistribution> distributedMeals;

    char filename[30];
    getDistributionLogFilename(filename);

    FileService fileService = FileService();
    if (!fileService.fileExists(filename)) {
        return distributedMeals;
    }

    File file = fileService.openFile(filename);
    if (!file) {
        return distributedMeals;
    }

    int lineNumber = 0;
    char line[70];    
    while (FileService::readLine(file, line, sizeof(line))) {
        // Skip first line (headers)
        if (lineNumber == 0) {
            lineNumber += 1;
            continue;
        }

        // Split line into segments
        char * segments[6];
        FileService::splitLine(line, segments);
        
        // Remove line returns
        int length = strlen(segments[6]);
        if (segments[6][length-1] == '\r') {
            segments[6][length-1]  = '\0';
        }

        MealDistribution distributedMeal(atoi(segments[2]), atol(segments[3]), atol(segments[4]), segments[5]);
        distributedMeals.push_back(distributedMeal);
        
        lineNumber += 1;
    }

    file.close();

    return distributedMeals;
}

vector<int> LogService::getDistributedMealIds() {
    vector<int> distributedMealIds;

    char filename[30];
    getDistributionLogFilename(filename);

    FileService fileService = FileService();
    if (!fileService.fileExists(filename)) {
        return distributedMealIds;
    }

    File file = fileService.openFile(filename);
    if (!file) {
        return distributedMealIds;
    }

    int lineNumber = 0;
    char line[70];    
    while (FileService::readLine(file, line, sizeof(line))) {
        // Skip first line (headers)
        if (lineNumber == 0) {
            lineNumber += 1;
            continue;
        }

        // Split line into segments
        char * segments[6];
        FileService::splitLine(line, segments);
        
        int mealId = atoi(segments[2]);

        if (find(distributedMealIds.begin(), distributedMealIds.end(), mealId) == distributedMealIds.end()) {
            distributedMealIds.push_back(mealId);
        }
        
        lineNumber += 1;
    }

    file.close();

    return distributedMealIds;
}

// PRIVATE

LogService::LogService() {}

void LogService::writeToFile(char * logLine) {
    char filename[30];
    getSystemLogFilename(filename);

    FileService fileService = FileService();
    File logFile = fileService.openFile(filename, FILE_WRITE);

    logFile.println(logLine);

    Serial.print("[LOG] ");
    Serial.println(logLine);

    logFile.close();
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
    const static char type8[] PROGMEM = "SAFETY_MODE_RETRY";
    const static char type9[] PROGMEM = "SAFETY_MODE_DISENGAGED";
    const static char type10[] PROGMEM = "MANUAL_CONTROL_START";
    const static char type11[] PROGMEM = "MANUAL_CONTROL_END";
    const static char type12[] PROGMEM = "MANUAL_RESTART";

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
        type11,
        type12
    };

    strcat(line, getString(textTypes[type]));
}

void LogService::getSystemLogFilename(char * filename) {
    sprintf(filename, "%s/%d%02d%02d.LOG", getString(PATH_SYSTEM_LOGS), year(), month(), day());
}

void LogService::getDistributionLogFilename(char * filename) {
    sprintf(filename, "%s/%d%02d%02d.CSV", getString(PATH_DISTRIB_LOGS), year(), month(), day());
}

void LogService::createDistributionLogFile(char * filename) {
    FileService fileService = FileService();

    // Skip if file already exists
    if (fileService.fileExists(filename)) return;

    const static char header[] PROGMEM = "appVersion;configVersion;mealId;startTime;endTime;missingSequences";

    File logFile = fileService.openFile(filename, FILE_WRITE);
    logFile.println(getString(header));
    logFile.close();
}
