#include "display_service.h"
#include "keypad_service.h"
#include "state_manager.h"
#include "string.h"
#include "manual_control_controller.h"

ManualControlController::ManualControlController() {}

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
    const static char title[] PROGMEM = "CONTROLE MANUEL";
    const static char waitingMsg[] PROGMEM = "En attente";

    DisplayService::getInstance().clearScreen();
    DisplayService::getInstance().printTitle(getString(title));
    DisplayService::getInstance().addBorder();
    DisplayService::getInstance().printCenter(getString(waitingMsg), 2);

    KeypadService::getInstance().waitForActivity(5000);
}