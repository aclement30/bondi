#include "display_service.h"
#include "meal_service.h"
#include "state_manager.h"
#include "manual_control_controller.h"

ManualControlController::ManualControlController(MealService & mealServiceRef) : mealService(mealServiceRef) {}

void ManualControlController::handle() {
    displayManualControlScreen();
}

void ManualControlController::escape() {
    // TODO: Stop conveyors, if needed

    // Go back to main menu
    StateManager::getInstance().changeState(ManualMenu);
    return;
}

// PRIVATE

void ManualControlController::displayManualControlScreen() {
    DisplayService::getInstance().clearScreen();
    DisplayService::getInstance().printTitle("CONTROLE MANUEL");
    DisplayService::getInstance().addBorder();
    DisplayService::getInstance().printCenter("En attente", 2);

    KeypadService::getInstance().waitForActivity(5000);
}