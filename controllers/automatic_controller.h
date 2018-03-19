#include "./controller.h"
#include "../display_service.h"
#include "../meal_service.h"
#include "../navigation_menu.h"
#include "../state_manager.h"

#ifndef CONTROLLERS_AUTOMATICCONTROLLER_H
#define CONTROLLERS_AUTOMATICCONTROLLER_H

using namespace std;

class AutomaticController: public Controller {
    public:
        AutomaticController(
            MealService & mealServiceRef
        ) :
           mealService(mealServiceRef)
        {}

        void handle() {
            displayAutomaticModeScreen();

            if (!mealService.hasCurrentMeal()) {
                mealService.refreshCurrentMeal();
            }

            if (mealService.hasCurrentMeal()) {
                mealService.distributeMeal();
            } else {
                Serial.println("Attente...");
            }

            delay(5000);
        }

        void escape() {
            if (!mealService.hasCurrentMeal()) {
                // Immediately go back to main menu
                StateManager::getInstance().changeState(MainMenu);
                return;
            }

            // Pause distribution
            mealService.stopFeeding();

            // Ask if distribution should be cancelled
            bool cancelDistribution = displayEscapeConfirmationScreen();

            if (cancelDistribution) {
                mealService.abortDistribution();
                StateManager::getInstance().changeState(MainMenu);
            }
        }

    private:
        MealService &mealService;

        void displayAutomaticModeScreen() {
            DisplayService::getInstance().clearScreen();
            DisplayService::getInstance().printTitle("MODE: AUTO");
            DisplayService::getInstance().addBorder();
            DisplayService::getInstance().printCenter("En attente", 2);
        }

        bool displayEscapeConfirmationScreen() {
            vector<string> errorMessage = {
                "La distribution du",
                "repas sera annulee !"
            };
            
            DisplayService::getInstance().showWarningScreen(errorMessage, "Continuer");
            return DisplayService::waitForConfirmation();
        }
};

#endif