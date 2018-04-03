#include "config.h"
#include "display_service.h"
#include "list_menu.h"
#include "meal_service.h"
#include "meal.h"
#include "state_manager.h"
#include "string.h"
#include "history_controller.h"

using namespace std;

HistoryController::HistoryController() {}

void HistoryController::handle() {
    displayHistoryScreen();
}

void HistoryController::resume() {
    handle();
}

void HistoryController::escape() {
    StateManager::getInstance().changeState(MainMenu);
}

// PRIVATE

void HistoryController::displayHistoryScreen() {
    const static char title[] PROGMEM = "SELECTION REPAS";
    vector<string> menuItems;
    for(int n = 0; n < AppConfig::getInstance().meals.size(); n++) {
        Meal & meal = AppConfig::getInstance().meals[n];
        char mealName[19];

        int hour = meal.startMoment / 60;
        const char *hourPrefix = hour >= 10 ? "" : "0";
        int minutes = meal.startMoment % 60;
        const char *minutesPrefix = minutes >= 10 ? "" : "0";
        
        // Add meal start hour
        sprintf(mealName, "%s%d:%s%d ", hourPrefix, hour, minutesPrefix, minutes);

        // Add meal name
        strcat(mealName, meal.name);

        if (MealService::isMealDistributed(meal.id)) {
            mealName[18] = '*';
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