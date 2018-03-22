#include "./controller.h"
#include "../display_service.h"
#include "../meal_service.h"
#include "../state_manager.h"

#ifndef CONTROLLERS_MANUALMEALDISTRIBUTIONCONTROLLER_H
#define CONTROLLERS_MANUALMEALDISTRIBUTIONCONTROLLER_H

using namespace std;

class ManualMealDistributionController: public Controller {
    public:
        ManualMealDistributionController(
            MealService & mealServiceRef
        ) :
           mealService(mealServiceRef)
        {}

        void handle() {
            if (mealService.hasCurrentMeal()) {
                mealService.distributeMeal();
            } else {
                displayMealSelectionScreen();
            }
        }

        void escape() {
            if (!mealService.hasCurrentMeal()) {
                // Immediately go back to main menu
                StateManager::getInstance().changeState(ManualMenu);
                return;
            }

            // Pause distribution
            mealService.stopFeeding();

            // Ask if distribution should be cancelled
            bool cancelDistribution = displayEscapeConfirmationScreen();

            if (cancelDistribution) {
                mealService.abortDistribution();
                StateManager::getInstance().changeState(ManualMenu);
            }
        }

    private:
        MealService & mealService;

        void displayMealSelectionScreen() {
            vector<string> menuOptions;
            for(int n = 0; n < mealService.meals.size(); n++) {
                char mealName[15];
                strcpy(mealName, mealService.meals[n].name);
                strncat(mealName, "              ", 14 - strlen(mealName));

                if (mealService.isMealDistributed(mealService.meals[n].id)) {
                    mealName[13] = '*';
                }

                menuOptions.push_back(string(mealName));
            }

            NavigationMenu menu;
            menu.build("SELECTION REPAS", menuOptions);
            menu.show();
            int selectedOption = menu.waitForSelection();

            if (selectedOption != -1) {
                mealService.selectMeal(mealService.meals[selectedOption - 1].id);
            } else {
                StateManager::getInstance().changeState(ManualMenu);
            }
        }

        bool displayEscapeConfirmationScreen() {
            vector<string> errorMessage = {
                "La distribution du",
                "repas sera annulee !"
            };
            
            DisplayService::getInstance().showWarningScreen(errorMessage, "Continuer");
            return KeypadService::getInstance().waitForConfirmation();
        }
};

#endif