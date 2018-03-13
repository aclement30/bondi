#include "./controller.h"
#include "../display_service.h"
#include "../meal_service.h"
#include "../state_manager.h"

#ifndef CONTROLLERS_AUTOMATICCONTROLLER_H
#define CONTROLLERS_AUTOMATICCONTROLLER_H

using namespace std;

class AutomaticController: public Controller {
    public:
        AutomaticController(
            //MealService &mealServiceRef
        ) 
           // mealService(mealServiceRef)
        {}

        void handle() {
            displayAutomaticModeScreen();

            // mealService.refreshCurrentMeal();

            // if (mealService.hasCurrentMeal()) {
            //     Meal currentMeal = mealService.getCurrentMeal();

            //     // TODO: Show meal distribution screen

            //     mealService.distributeMeal(currentMeal);
            // } else {
            //     Serial.println("Attente...");
            //     delay(1000);
            // }
        }

    private:
        // MealService &mealService;

        void displayAutomaticModeScreen() {
            DisplayService::getInstance().clearScreen();
            DisplayService::getInstance().printTitle("MODE: AUTO");
            DisplayService::getInstance().addBorder();
            DisplayService::getInstance().printCenter("En attente", 2);

            // delay(2000);

            // DisplayScreens::mealDistribution();
        }
};

#endif