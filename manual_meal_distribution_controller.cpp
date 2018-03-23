#include "display_service.h"
#include "meal_service.h"
#include "navigation_menu.h"
#include "state_manager.h"
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

bool ManualMealDistributionController::displayEscapeConfirmationScreen() {
    vector<string> errorMessage = {
        "La distribution du",
        "repas sera annulee !"
    };
    
    DisplayService::getInstance().showWarningScreen(errorMessage, "Continuer");
    return KeypadService::getInstance().waitForConfirmation();
}