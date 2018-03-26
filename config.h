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

std::vector<RailPoint> loadRailPoints(const char * filename);
std::vector<Meal> loadMeals(const char * filename);
std::vector<MealSequence> loadMealSequences(const char * filename);

void displaySDCardErrorScreen();
void displayConfigurationErrorScreen();

Config loadStaticConfiguration();
Config loadSDCardConfiguration();

#endif