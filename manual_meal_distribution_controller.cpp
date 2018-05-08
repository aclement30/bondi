#include "config.h"
#include "display_service.h"
#include "location_service.h"
#include "log_service.h"
#include "meal_service.h"
#include "navigation_menu.h"
#include "safety_service.h"
#include "state_manager.h"
#include "string.h"
#include "manual_meal_distribution_controller.h"

using namespace std;

ManualMealDistributionController::ManualMealDistributionController() {}

ManualMealDistributionController::~ManualMealDistributionController() {
    if (mealServicePtr != NULL) {
        delete mealServicePtr;
    }
}

void ManualMealDistributionController::handle() {
    if (hasCurrentMeal()) {
        if (!mealServicePtr->isDistributionCompleted()) {
            mealServicePtr->distributeMeal();
        } else {
            mealServicePtr->displayMealCompletionScreen();

            delete mealServicePtr;
            mealServicePtr = NULL;
        }
    } else {
        displayMealSelectionScreen();
    }
}

void ManualMealDistributionController::resume() {
    if (hasCurrentMeal()) {
        mealServicePtr->displayMealDistributionScreen();
    }
    
    handle();
}

void ManualMealDistributionController::escape() {
    if (!hasCurrentMeal()) {
        // Immediately go back to main menu
        StateManager::getInstance().changeState(ManualMenu);
        return;
    }

    // Pause distribution
    mealServicePtr->stopFeeding();

    // Ask if distribution should be cancelled
    bool cancelDistribution = displayEscapeConfirmationScreen();

    if (cancelDistribution) {
        StateManager::getInstance().changeState(ManualMenu);
    }
}

void ManualMealDistributionController::safetyStop() {
    if (mealServicePtr != NULL) {
        mealServicePtr->safetyStop();
    }
}

// PRIVATE

bool ManualMealDistributionController::hasCurrentMeal() {
    return mealServicePtr != NULL;
}

void ManualMealDistributionController::displayMealSelectionScreen() {
    const static char title[] PROGMEM = "SELECTION REPAS";
    vector<string> menuOptions;
    
    for(int n = 0; n < AppConfig::getInstance().meals.size(); n++) {
        char mealName[15];
        strcpy(mealName, AppConfig::getInstance().meals[n].name);
        strncat(mealName, "              ", 14 - strlen(mealName));

        if (MealService::isMealDistributed(AppConfig::getInstance().meals[n].id)) {
            mealName[13] = '*';
        }

        menuOptions.push_back(string(mealName));
    }

    NavigationMenu menu;
    menu.build(getString(title), menuOptions);
    menu.show();
    int selectedOption = menu.waitForSelection();

    if (selectedOption != -1) {
        if (!LocationService::getInstance().isDocked()) {
            displayDockingErrorScreen();
            StateManager::getInstance().changeState(ManualMenu);
            return;
        }

        if (mealServicePtr != NULL) {
            delete mealServicePtr;
            mealServicePtr = NULL;
        }

        LogService::getInstance().log(MANUAL_MEAL_SELECTION, AppConfig::getInstance().meals[selectedOption - 1].name);
        
        // Display short warning notice before moving
        SafetyService::getInstance().displayMovingWarning();

        int mealId = AppConfig::getInstance().meals[selectedOption - 1].id;
        mealServicePtr = new MealService(mealId);
        mealServicePtr->displayMealDistributionScreen();
        mealServicePtr->startDistribution();

        return;
    } else {
        StateManager::getInstance().changeState(ManualMenu);
    }
}

void ManualMealDistributionController::displayDockingErrorScreen() {
    const static char errorMsg1[] PROGMEM = "Le robot doit etre";
    const static char errorMsg2[] PROGMEM = "positionne au dock.";
    const static char okBtn[] PROGMEM = "OK";

    vector<string> errorMessage = {
        getString(errorMsg1),
        getString(errorMsg2)
    };
    DisplayService::getInstance().showErrorScreen(errorMessage, getString(okBtn));
    KeypadService::getInstance().waitForConfirmation();
    return;
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