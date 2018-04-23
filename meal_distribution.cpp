#include <Arduino.h>
#include <ArduinoSTL.h>
#include <Time.h>
#include "meal_distribution.h"

using namespace std;

MealDistribution::MealDistribution(
    int mealId,
    time_t mealStartTime, 
    time_t mealEndTime,
    char * distributionMissingSequences
) {
    id = mealId;
    startTime = mealStartTime;
    endTime = mealEndTime;
    missingSequences = distributionMissingSequences;
}