#include "display_service.h"
#include "meal_service.h"
#include "navigation_menu.h"
#include "state_manager.h"
#include "string.h"
#include "manual_meal_distribution_controller.h"

using namespace std;

ManualMealDistributionController::ManualMealDistributionController(MealService & mealServiceRef) : mealService(mealServiceRef) {}

void ManualMealDistributionController::handle() {
    if (mealService.hasCurrentMeal()) {
        mealService.distributeMeal();
    } else {
        displayMealSelectionScreen();
    }
}

void ManualMealDistributionController::escape() {
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

void ManualMealDistributionController::displayMealSelectionScreen() {
    const static char title[] PROGMEM = "SELECTION REPAS";
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
    menu.build(getString(title), menuOptions);
    menu.show();
    int selectedOption = menu.waitForSelection();

    if (selectedOption != -1) {
        mealService.selectMeal(mealService.meals[selectedOption - 1].id);
    } else {
        StateManager::getInstance().changeState(ManualMenu);
    }
}

bool ManualMealDistributionController::displayEscapeConfirmationScreen() {
    const static char confirmButtonText[] PROGMEM = "Continuer";
    const static char errorMsg1[] PROGMEM = "La distribution du";
    const static char errorMsg2[] PROGMEM = "repas sera annulee !";

    vector<string> errorMessage = {
        getString(errorMsg1),
        getString(errorMsg2)
    };
    
    DisplayService::getInstance().showWarningScreen(errorMessage, getString(confirmButtonText));
    return KeypadService::getInstance().waitForConfirmation();
}