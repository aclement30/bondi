#include <ArduinoSTL.h>

#ifndef MEAL_H
#define MEAL_H

using namespace std;

struct Meal {
    int id;
    const char * name;
    int startMoment;
    int routeId;

    Meal(
        int mealId,
        const char * mealName,
        int mealStartMoment, 
        int mealRouteId
    );
};

int getMealIndexById(vector<Meal> meals, int mealId);

#endif