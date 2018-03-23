#include "display_service.h"
#include "keypad_service.h"
#include "meal_service.h"
#include "navigation_menu.h"
#include "state_manager.h"
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
        Serial.println("Attente...");
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
    DisplayService::getInstance().clearScreen();
    DisplayService::getInstance().printTitle("MODE: AUTO");
    DisplayService::getInstance().addBorder();
    DisplayService::getInstance().printCenter("En attente", 2);
}

bool AutomaticController::displayEscapeConfirmationScreen() {
    vector<string> errorMessage = {
        "La distribution du",
        "repas sera annulee !"
    };
    
    DisplayService::getInstance().showWarningScreen(errorMessage, "Continuer");
    return KeypadService::getInstance().waitForConfirmation();
}