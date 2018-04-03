#include "config.h"
#include "display_service.h"
#include "keypad_service.h"
#include "location_service.h"
#include "meal_service.h"
#include "navigation_menu.h"
#include "state_manager.h"
#include "string.h"
#include "automatic_controller.h"

using namespace std;

AutomaticController::AutomaticController() {}

AutomaticController::~AutomaticController() {
    if (mealServicePtr != NULL) {
        delete mealServicePtr;
    }
}

void AutomaticController::handle() {
    // Skip if there is already a meal being distributed
    if (hasCurrentMeal()) {
        if (!mealServicePtr->isDistributionCompleted()) {
            mealServicePtr->distributeMeal();
        } else {
            mealServicePtr->displayMealCompletionScreen();

            delete mealServicePtr;
            mealServicePtr = NULL;
        }
    } else {
        displayAutomaticModeScreen();

        KeypadService::getInstance().waitForActivity(5000);

        // Do nothing until feeder is docked
        if (!LocationService::getInstance().isDocked()) {
            return;
        }

        int scheduledMealId = MealService::getScheduledMealId(AppConfig::getInstance().meals);
        if (scheduledMealId == 0) {
            return;
        }

        if (mealServicePtr != NULL) {
            delete mealServicePtr;
            mealServicePtr = NULL;
        }

        mealServicePtr = new MealService(scheduledMealId);
        mealServicePtr->displayMealDistributionScreen();
        mealServicePtr->startDistribution();
        return;
    }
}

void AutomaticController::resume() {
    if (hasCurrentMeal()) {
        mealServicePtr->displayMealDistributionScreen();
    }
    
    handle();
}

void AutomaticController::escape() {
    if (!hasCurrentMeal()) {
        // Immediately go back to main menu
        StateManager::getInstance().changeState(MainMenu);
        return;
    }

    // Pause distribution
    mealServicePtr->stopFeeding();

    // Ask if distribution should be cancelled
    bool cancelDistribution = displayEscapeConfirmationScreen();

    if (cancelDistribution) {
        StateManager::getInstance().changeState(MainMenu);
    }
}

// PRIVATE

bool AutomaticController::hasCurrentMeal() {
    return mealServicePtr != NULL;
}

void AutomaticController::displayAutomaticModeScreen() {
    const static char title[] PROGMEM = "MODE: AUTO";
    const static char waitingMsg[] PROGMEM = "En attente";

    DisplayService::getInstance().clearScreen();
    DisplayService::getInstance().printTitle(getString(title));
    DisplayService::getInstance().addBorder();
    DisplayService::getInstance().printCenter(getString(waitingMsg), 2);
}

bool AutomaticController::displayEscapeConfirmationScreen() {
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