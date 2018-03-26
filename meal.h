#include <ArduinoSTL.h>
#include "rail_point.h"
#include "route.h"

#ifndef MEAL_H
#define MEAL_H

using namespace std;

// TODO : Transform into struct
struct MealSequence {
    const char * name;
    int mealId;
    int startPointId;
    int endPointId;
    int feed1Flow;
    int feed2Flow;

    MealSequence(
        const char * sequenceName, 
        int sequenceMealId,
        int sequenceStartPointId, 
        int sequenceEndPointId, 
        int sequenceFeed1Flow, 
        int sequenceFeed2Flow
    );
};

// TODO : Transform into struct
struct Meal {
    int id;
    const char * name;
    int startMoment;
    int routeId;
    vector<MealSequence> sequences;

    Meal(
        int mealId,
        const char * mealName,
        int mealStartMoment, 
        int mealRouteId,
        vector<MealSequence> mealSequences
    );

    int getMealSequenceIndex(RailPoint * railPointPtr);
};

int getMealIndexById(vector<Meal> meals, int mealId);

#endif