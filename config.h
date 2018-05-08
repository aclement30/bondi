#include "ArduinoJson.h"
#include <SD.h>

#include "constants.h"
#include "meal.h"
#include "meal_sequence.h"
#include "rail_point.h"
#include "route.h"

#ifndef CONFIG_H
#define CONFIG_H

using namespace std;

class AppConfig {
    public:
        static AppConfig & getInstance();

        vector<RailPoint> railPoints;
        vector<Route> routes;
        vector<Meal> meals;
        int configVersion;

    private:
        AppConfig();
        void operator = (AppConfig const&); // Don't implement
};

struct Config {
    vector<RailPoint> railPoints;
    vector<Route> routes;
    vector<Meal> meals;
    int version;
};

enum ConfigurationSource {
    Static,
    SDCard
};

vector<RailPoint> loadRailPoints(const char * filename);
vector<Meal> loadMeals(const char * filename);
vector<MealSequence> loadStaticMealSequences(int mealId);
vector<MealSequence> loadMealSequences(const char * filename, int mealId);
int loadConfigVersion(const char * filename);

void displaySDCardErrorScreen();
void displayConfigFilesErrorScreen();
void displayConfigurationErrorScreen(ConfigurationError configError);

Config loadStaticConfiguration();
Config loadSDCardConfiguration();
Config loadConfiguration();

ConfigurationError validateConfiguration(Config config);

#endif