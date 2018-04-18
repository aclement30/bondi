#include <Arduino.h>
#include "display_service.h"
#include "file_service.h"
#include "keypad_service.h"
#include "meal.h"
#include "rail_point.h"
#include "string.h"
#include "config.h"

const ConfigurationSource configSource = SDCard;

AppConfig & AppConfig::getInstance() {
    static AppConfig instance;      // Guaranteed to be destroyed.
                                    // Instantiated on first use.
    return instance;
}

AppConfig::AppConfig() {}

std::vector<RailPoint> loadRailPoints(const char * filename) {
    std::vector<RailPoint> points;
    
    Serial.println(F("* chargement de la configuration des points"));

    FileService fileService = FileService();
    File file = fileService.openFile(filename);
    if (!file) {
        Serial.println(F("Erreur configuration: le fichier points.csv ne peut être ouvert!"));
        return points;
    }

    int lineNumber = 0;
    char line[40];
    while (FileService::readLine(file, line, sizeof(line))) {
        // Skip first line (headers)
        if (lineNumber == 0) {
            lineNumber += 1;
            continue;
        }

        // Split line into segments
        char * segments[3];
        FileService::splitLine(line, segments);
        
        // Remove line returns
        int length = strlen(segments[2]);
        if (segments[2][length-1] == '\r') {
            segments[2][length-1]  = '\0';
        }

        RailPoint point(atoi(segments[0]), segments[1], segments[2]);
        points.push_back(point);
        
        lineNumber += 1;
    }

    file.close();

    return points;
}

std::vector<Meal> loadMeals(const char * filename) {
    std::vector<Meal> meals;
    
    Serial.println(F("* chargement de la configuration des repas"));

    FileService fileService = FileService();
    File file = fileService.openFile(filename);
    if (!file) {
        Serial.println(F("Erreur configuration: le fichier meals.csv ne peut être ouvert!"));
        return meals;
    }

    int lineNumber = 0;
    char line[30];
    while (FileService::readLine(file, line, sizeof(line))) {
        // Skip first line (headers)
        if (lineNumber == 0) {
            lineNumber += 1;
            continue;
        }

        // Split line into segments
        char * segments[4];
        FileService::splitLine(line, segments);
        
        Meal meal(atoi(segments[0]), segments[1], atoi(segments[2]), atoi(segments[3]));
        meals.push_back(meal);
        
        lineNumber += 1;
    }

    file.close();

    return meals;
}

std::vector<MealSequence> loadStaticMealSequences(int mealId) {
    std::vector<MealSequence> sequences;

    switch(mealId) {
        case 1:
            sequences.push_back(MealSequence("G1", 1000, 102, 25, 50));
            sequences.push_back(MealSequence("G2", 102, 1000, 50, 75));
        break;
        case 2:
            sequences.push_back(MealSequence("P1", 1000, 1, 50, 25));
            sequences.push_back(MealSequence("P2", 1, 1000, 25, 0));
        break;
    }

    return sequences;
}

std::vector<MealSequence> loadMealSequences(const char * filename, int mealId) {
    if (configSource == Static) {
        return loadStaticMealSequences(mealId);
    }

    std::vector<MealSequence> mealSequences;

    Serial.println(F("* chargement de la configuration des séquences de repas"));

    FileService fileService = FileService();
    File file = fileService.openFile(filename);
    if (!file) {
        Serial.println(F("Erreur configuration: le fichier meal_seq.csv ne peut être ouvert!"));
        return mealSequences;
    }

    int lineNumber = 0;
    char line[60];
    while (FileService::readLine(file, line, sizeof(line))) {
        // Skip first line (headers)
        if (lineNumber == 0) {
            lineNumber += 1;
            continue;
        }

        // Split line into segments
        char * segments[6];
        FileService::splitLine(line, segments);
        
        // Only keep sequences from selected meal
        int sequenceMealId = atoi(segments[0]);
        if (sequenceMealId != mealId) {
            continue;
        }

        MealSequence mealSequence(segments[1], atoi(segments[2]), atoi(segments[3]), atoi(segments[4]), atoi(segments[5]));
        mealSequences.push_back(mealSequence);
        
        lineNumber += 1;
    }

    file.close();

    return mealSequences;
}

int loadConfigVersion(const char * filename) {
    FileService fileService = FileService();
    File file = fileService.openFile(filename);
    if (!file) {
        Serial.println(F("Erreur configuration: le fichier de version ne peut être ouvert!"));
        return 0;
    }

    char line[8];
    FileService::readLine(file, line, sizeof(line));
    file.close();

    // Remove line returns
    int length = strlen(line);
    if (line[length-1] == '\r') {
        line[length-1]  = '\0';
    }
    
    int configVersion = atoi(line);

    return configVersion;
}

void displaySDCardErrorScreen() {
    const static char okButtonText[] PROGMEM = "Redemarrer";
    const static char errorMsg1[] PROGMEM = "Carte SD invalide";
    const static char errorMsg2[] PROGMEM = "ou manquante.";

    vector<string> errorMessage = {
        getString(errorMsg1),
        getString(errorMsg2)
    };
    
    DisplayService::getInstance().showErrorScreen(errorMessage, getString(okButtonText));
    bool restart = KeypadService::getInstance().waitForConfirmation();
    if (restart) {
        asm volatile ("  jmp 0");
    }
}

void displayConfigurationErrorScreen() {
    const static char okButtonText[] PROGMEM = "Redemarrer";
    const static char errorMsg1[] PROGMEM = "Fichiers de config.";
    const static char errorMsg2[] PROGMEM = "manquants.";

    vector<string> errorMessage = {
        getString(errorMsg1),
        getString(errorMsg2)
    };
    
    DisplayService::getInstance().showErrorScreen(errorMessage, getString(okButtonText));
    bool restart = KeypadService::getInstance().waitForConfirmation();
    if (restart) {
        asm volatile ("  jmp 0");
    }
}

Config loadStaticConfiguration() {
    std::vector<RailPoint> railPoints = {
        RailPoint(1000, "DOCK", "155.198.195.233.119"),
        RailPoint(2, "G-2", "0.0.0.0.2"),
        RailPoint(4, "G-4", "0.0.0.0.4"),
        RailPoint(102, "REV", "197.8.161.185.213"),
        RailPoint(1, "G-1", "43.103.109.43.10"),
        RailPoint(3, "G-3", "68.103.110.43.102"),
        RailPoint(5, "P-5",  "0.0.0.0.7"),
        RailPoint(7, "P-7", "0.0.0.0.8"),
        RailPoint(9, "P-9", "0.0.0.0.9"),
        RailPoint(101, "REV", "0.0.0.0.11"),
        RailPoint(8, "P-8", "0.0.0.0.12"),
        RailPoint(10, "P-10", "0.0.0.0.13"),
        RailPoint(12, "P-12", "0.0.0.0.14"),
        RailPoint(14, "P-14", "0.0.0.0.15")
    };

    std::vector<Route> routes = {
        Route(1, MOVING_FORWARD, 1000, 1000),
        Route(2, MOVING_BACKWARD, 1000, 1000)
    };

    std::vector<Meal> meals = {
        Meal(1, "Matin Gd.E", 420, 1),
        Meal(2, "Matin Pt.E", 450, 2)
    };

    Config config = {
        railPoints,
        routes,
        meals,
        1,
    };

    return config;
}

Config loadSDCardConfiguration() {    
    Config config = Config();

    FileService fileService = FileService();
    if (!fileService.validateSDCard()) {
        displaySDCardErrorScreen();
        return config;
    }

    if (!fileService.validateConfigFiles()) {
        displayConfigurationErrorScreen();
        return config;
    }

    config.routes = {
        Route(1, MOVING_FORWARD, 1000, 1000),
        Route(2, MOVING_BACKWARD, 1000, 1000)
    };
    config.railPoints = loadRailPoints(getString(FILE_POINTS));
    config.meals = loadMeals(getString(FILE_MEALS));
    config.version = loadConfigVersion(getString(FILE_CONFIG_VERSION));

    return config;
}

Config loadConfiguration() {
    if (configSource == Static) {
        return loadStaticConfiguration();
    }

    return loadSDCardConfiguration();
}