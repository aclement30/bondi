#include "./controller.h"
#include "../display_service.h"
#include "../meal_service.h"
#include "../state_manager.h"

#ifndef HISTORY_CONTROLLER_H
#define HISTORY_CONTROLLER_H

using namespace std;

class HistoryController: public Controller {
    public:
        HistoryController(
            MealService & mealServiceRef
        ) :
           mealService(mealServiceRef)
        {}

        void handle() {
            displayHistoryScreen();
        }

        void escape() {
            StateManager::getInstance().changeState(MainMenu);
        }

    private:
        MealService & mealService;

        void displayHistoryScreen() {
            vector<string> menuItems;
            for(int n = 0; n < mealService.meals.size(); n++) {
                Meal & meal = mealService.meals[n];
                char mealName[19];

                int hour = meal.startMoment / 60;
                const char *hourPrefix = hour >= 10 ? "" : "0";
                int minutes = meal.startMoment % 60;
                const char *minutesPrefix = minutes >= 10 ? "" : "0";
                
                // Add meal start hour
                sprintf(mealName, "%s%d:%s%d ", hourPrefix, hour, minutesPrefix, minutes);

                // Add meal name
                strcat(mealName, meal.name);

                if (mealService.isMealDistributed(meal.id)) {
                    mealName[18] = '*';
                }

                menuItems.push_back(string(mealName));
            }

            ListMenu menu;
            menu.build("HISTORIQUE", menuItems);
            menu.show();
            int selectedOption = menu.waitForSelection();

            if (selectedOption != -1) {
                selectedOption = menu.waitForSelection();
            } else {
                StateManager::getInstance().changeState(MainMenu);
            }
        }
};

#endif