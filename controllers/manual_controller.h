#include "./controller.h"
#include "../display_service.h"
#include "../meal_service.h"
#include "../state_manager.h"

#ifndef CONTROLLERS_MANUALCONTROLLER_H
#define CONTROLLERS_MANUALCONTROLLER_H

using namespace std;

class ManualController: public Controller {
    public:
        ManualController(
            MealService & mealServiceRef
        ) :
           mealService(mealServiceRef)
        {}

        void handle() {
            displayManualModeScreen();

            // mealService.refreshCurrentMeal();

            // if (mealService.hasCurrentMeal()) {
            //     Meal currentMeal = mealService.getCurrentMeal();

            //     // TODO: Show meal distribution screen

            //     mealService.distributeMeal(currentMeal);
            // } else {
            //     Serial.println("Attente...");
            //     delay(1000);
            // }

            delay(2000);
        }

    private:
        MealService & mealService;

        void displayManualModeScreen() {
            vector<string> menuOptions = {
                "Selection repas",
                "Controle manuel"
            };

            NavigationMenu menu = NavigationMenu("MODE: MANUEL", menuOptions);
            menu.show();
            int selectedOption = menu.waitForSelection();

            switch(selectedOption) {
                case 1:
                    displayMealSelectionScreen();
                    break;
                case 2:
                    displayManualControlScreen();
                    break;
                case -1:
                    StateManager::getInstance().changeState(MainMenu);
                    break;
            }
        }

        void displayMealSelectionScreen() {
            vector<string> menuOptions;
            for(int n = 0; n < mealService.meals.size(); n++) {
                char mealName[15];
                strcpy(mealName, mealService.meals[n].name);
                strncat(mealName, "              ", 14 - strlen(mealName));
                mealName[13] = '*';
                menuOptions.push_back(string(mealName));
            }

            NavigationMenu menu = NavigationMenu("SELECTION REPAS", menuOptions);
            menu.show();
            int selectedOption = menu.waitForSelection();

            switch(selectedOption) {
                case 1:
                    //DisplayScreens::mealDistribution();
                    break;
                case 2:
                    //DisplayScreens::mealDistribution();
                    break;
                case -1:
                    displayManualModeScreen();
                    break;
            }
        }

        void displayManualControlScreen() {
            DisplayService::getInstance().clearScreen();
            DisplayService::getInstance().printTitle("CONTROLE MANUEL");
            DisplayService::getInstance().addBorder();
            DisplayService::getInstance().printCenter("En attente", 2);

            delay(2000);
        }
};

#endif