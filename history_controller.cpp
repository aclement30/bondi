#include "config.h"
#include "display_service.h"
#include "list_menu.h"
#include "log_service.h"
#include "meal_service.h"
#include "meal_distribution.h"
#include "meal.h"
#include "state_manager.h"
#include "string.h"
#include "history_controller.h"

using namespace std;

HistoryController::HistoryController() {}

void HistoryController::handle() {
    displayHistoryScreen();
}

void HistoryController::escape() {
    StateManager::getInstance().changeState(MainMenu);
}

// PRIVATE

void HistoryController::displayHistoryScreen() {
    const static char title[] PROGMEM = "HISTORIQUE";
    vector<string> menuItems;

    vector<MealDistribution> distributedMeals = LogService::getInstance().getDistributionHistory();

    for(int n = 0; n < distributedMeals.size(); n++) {
        MealDistribution & distributedMeal = distributedMeals[n];

        int mealIndex = getMealIndexById(AppConfig::getInstance().meals, distributedMeal.id);
        if (mealIndex == -1) continue;

        Meal & meal = AppConfig::getInstance().meals[mealIndex];

        char mealName[19];

        tmElements_t startTime;
        breakTime(distributedMeal.startTime, startTime);
        
        // Add meal start time
        sprintf(mealName, "%02d:%02d ", startTime.Hour, startTime.Minute);

        // Add meal name
        strcat(mealName, meal.name);
        strncat(mealName, "              ", 18 - strlen(mealName));

        // Show (!) when there are missing sequences for distributed meal
        if (strlen(distributedMeal.missingSequences) > 1) {
            mealName[17] = '!';
        }

        menuItems.push_back(string(mealName));
    }

    ListMenu menu;
    menu.build(getString(title), menuItems);
    menu.show();
    int selectedOption = menu.waitForSelection();

    if (selectedOption != -1) {
        selectedOption = menu.waitForSelection();
    } else {
        StateManager::getInstance().changeState(MainMenu);
    }
}