#include "ArduinoJson.h"
#include <SD.h>

#include "constants.h"
#include "meal.h"
#include "route.h"

#ifndef CONFIG_H
#define CONFIG_H

struct Config {
    std::vector<RailPoint> railPoints;
    std::vector<Route> routes;
    std::vector<Meal> meals;
};

std::vector<RailPoint> loadRailPoints(JsonArray &data);
std::vector<Route> loadRoutes(JsonArray &data, std::vector<RailPoint> railPoints);
// std::vector<MealSequence> loadMealSequences(JsonArray &data, std::vector<RailPoint> railPoints);
// std::vector<Meal> loadMeals(JsonArray &data, std::vector<Route> routes, std::vector<RailPoint> railPoints);

Config loadStaticConfiguration();
Config loadSDCardConfiguration();

#endif