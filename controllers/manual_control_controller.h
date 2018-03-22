#include "./controller.h"
#include "../display_service.h"
#include "../meal_service.h"
#include "../state_manager.h"

#ifndef CONTROLLERS_MANUALCONTROLCONTROLLER_H
#define CONTROLLERS_MANUALCONTROLCONTROLLER_H

using namespace std;

class ManualControlController: public Controller {
    public:
        ManualControlController(
            MealService & mealServiceRef
        ) :
           mealService(mealServiceRef)
        {}

        void handle() {
            displayManualControlScreen();
        }

        void escape() {
            // TODO: Stop conveyors, if needed

            // Go back to main menu
            StateManager::getInstance().changeState(ManualMenu);
            return;
        }

    private:
        MealService & mealService;

        void displayManualControlScreen() {
            DisplayService::getInstance().clearScreen();
            DisplayService::getInstance().printTitle("CONTROLE MANUEL");
            DisplayService::getInstance().addBorder();
            DisplayService::getInstance().printCenter("En attente", 2);

            KeypadService::getInstance().waitForActivity(5000);
        }
};

#endif