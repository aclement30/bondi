#include <ArduinoSTL.h>
#include <Time.h>

#ifndef MEAL_DISTRIBUTION_H
#define MEAL_DISTRIBUTION_H

using namespace std;

struct MealDistribution {
    int id;
    time_t startTime;
    time_t endTime;
    char * missingSequences;

    MealDistribution(
        int mealId,
        time_t mealStartTime, 
        time_t mealEndTime,
        char * distributionMissingSequences
    );
};

#endif