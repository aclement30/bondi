#include <Arduino.h>
#include <ArduinoSTL.h>
#include "rail_point.h"
#include "route.h"
#include "meal.h"

using namespace std;

MealSequence::MealSequence(
    const char * sequenceName, 
    int sequenceStartPointInt, 
    int sequenceEndPointInt, 
    int sequenceFeed1Flow, 
    int sequenceFeed2Flow
) {
    name = sequenceName;
    startPointId = sequenceStartPointInt;
    endPointId = sequenceEndPointInt;
    feed1Flow = sequenceFeed1Flow;
    feed2Flow = sequenceFeed2Flow;
}

Meal::Meal(
    int mealId,
    const char * mealName,
    int mealStartMoment, 
    int mealRouteId,
    vector<MealSequence> mealSequences
) {
    id = mealId;
    name = mealName;
    startMoment = mealStartMoment;
    routeId = mealRouteId;
    sequences = mealSequences;
}

int Meal::getMealSequenceIndex(RailPoint * railPointPtr) {
    for(int n = 0; n < sequences.size(); n++) {
        const MealSequence sequence = sequences[n];
        if (sequence.startPointId == railPointPtr->id) {
            return n;
        }
    }

    return -1;
}

int getMealIndexById(vector<Meal> meals, int mealId) {
    vector<Meal>::iterator iterator = find_if (meals.begin(), meals.end(), [&](const Meal & meal) {
        return meal.id == mealId;
    });

    if (iterator == meals.end()) {
        return -1;
    }

    return iterator - meals.begin();
}