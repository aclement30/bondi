#include "display_service.h"
#include "keypad_service.h"
#include "meal_service.h"
#include "navigation_menu.h"
#include "state_manager.h"
#include "string.h"
#include "automatic_controller.h"

using namespace std;

AutomaticController::AutomaticController(
    MealService & mealServiceRef
) :
    mealService(mealServiceRef)
{}

void AutomaticController::handle() {
    displayAutomaticModeScreen();

    if (!mealService.hasCurrentMeal()) {
        mealService.refreshCurrentMeal();
    }

    if (mealService.hasCurrentMeal()) {
        mealService.distributeMeal();
    } else {
        Serial.println(F("Attente..."));
    }

    KeypadService::getInstance().waitForActivity(5000);
}

void AutomaticController::escape() {
    if (!mealService.hasCurrentMeal()) {
        // Immediately go back to main menu
        StateManager::getInstance().changeState(MainMenu);
        return;
    }

    // Pause distribution
    mealService.stopFeeding();

    // Ask if distribution should be cancelled
    bool cancelDistribution = displayEscapeConfirmationScreen();

    if (cancelDistribution) {
        mealService.abortDistribution();
        StateManager::getInstance().changeState(MainMenu);
    }
}

// PRIVATE

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