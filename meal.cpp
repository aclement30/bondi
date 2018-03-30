#include <Arduino.h>
#include <ArduinoSTL.h>
#include "meal.h"

using namespace std;

Meal::Meal(
    int mealId,
    const char * mealName,
    int mealStartMoment, 
    int mealRouteId
) {
    id = mealId;
    name = mealName;
    startMoment = mealStartMoment;
    routeId = mealRouteId;
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