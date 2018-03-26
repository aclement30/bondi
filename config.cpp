#include <Arduino.h>
#include "display_service.h"
#include "file_service.h"
#include "keypad_service.h"
#include "meal.h"
#include "rail_point.h"
#include "string.h"
#include "config.h"

std::vector<RailPoint> loadRailPoints(const char * filename) {
    std::vector<RailPoint> points;
    
    Serial.println(F("* chargement de la configuration des points"));

    FileService fileService = FileService();
    File file = fileService.openFile(filename);
    if (!file) {
        Serial.println(F("Erreur configuration: le fichier points.csv ne peut être ouvert!"));
        return;
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
        return;
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
        
        // TODO: Remove sequence
        std::vector<MealSequence> sequence;
        
        Meal meal(atoi(segments[0]), segments[1], atoi(segments[2]), atoi(segments[3]), sequence);
        meals.push_back(meal);
        
        lineNumber += 1;
    }

    file.close();

    return meals;
}

std::vector<MealSequence> loadMealSequences(const char * filename) {
    std::vector<MealSequence> mealSequences;

    Serial.println(F("* chargement de la configuration des séquences de repas"));

    FileService fileService = FileService();
    File file = fileService.openFile(filename);
    if (!file) {
        Serial.println(F("Erreur configuration: le fichier meal_seq.csv ne peut être ouvert!"));
        return;
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
        
        MealSequence mealSequence(segments[1], atoi(segments[0]), atoi(segments[2]), atoi(segments[3]), atoi(segments[4]), atoi(segments[5]));
        mealSequences.push_back(mealSequence);
        
        lineNumber += 1;
    }

    file.close();

    return mealSequences;
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
        RailPoint(2, "G-2", "197.8.161.185.213"),
        RailPoint(4, "G-4", "0.0.0.0.2"),
        RailPoint(101, "REV", "0.0.0.0.4"),
        RailPoint(1, "G-1", "197.8.161.185.213"),
        RailPoint(3, "G-3", "0.0.0.0.6"),
        RailPoint(5, "P-5",  "0.0.0.0.7"),
        RailPoint(7, "P-7", "0.0.0.0.8"),
        RailPoint(9, "P-9", "0.0.0.0.9"),
        RailPoint(102, "REV", "0.0.0.0.11"),
        RailPoint(8, "P-8", "0.0.0.0.12"),
        RailPoint(10, "P-10", "0.0.0.0.13"),
        RailPoint(12, "P-12", "0.0.0.0.14"),
        RailPoint(14, "P-14", "0.0.0.0.15")
    };

    std::vector<Route> routes = {
        Route(1, MOVING_FORWARD, 1000, 1000),
        Route(2, MOVING_BACKWARD, 1000, 1000)
    };

    std::vector<MealSequence> sequence1 = {
        MealSequence("G1", 1, 1000, 2, 25, 50),
        MealSequence("G2", 1, 2, 1000, 50, 75)
    };

    std::vector<MealSequence> sequence2 = {
        MealSequence("P1", 2, 000, 1, 50, 25),
        MealSequence("P2", 2, 1, 1000, 25, 0),
    };

    std::vector<Meal> meals = {
        Meal(1, "Matin Gd.E", 420, 1, sequence1),
        Meal(2, "Matin Pt.E", 450, 2, sequence2)
    };

    Config config = {
        railPoints,
        routes,
        meals
    };

    return config;
}

Config loadSDCardConfiguration() {    
    Config config = Config();

    FileService fileService = FileService();
    if (!fileService.validateSDCard()) {
        displaySDCardErrorScreen();
        return;
    }

    if (!fileService.validateConfigFiles()) {
        displayConfigurationErrorScreen();
        return;
    }

    config.routes = {
        Route(1, MOVING_FORWARD, 1000, 1000),
        Route(2, MOVING_BACKWARD, 1000, 1000)
    };
    config.railPoints = loadRailPoints("points.csv");
    config.meals = loadMeals("meals.csv");

    return config;
}