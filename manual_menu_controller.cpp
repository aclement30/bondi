#include "display_service.h"
#include "navigation_menu.h"
#include "state_manager.h"
#include "string.h"
#include "manual_menu_controller.h"

using namespace std;

void ManualMenuController::handle() {
    displayManualMenuScreen();
}

void ManualMenuController::resume() {
    handle();
}

void ManualMenuController::escape() {
    StateManager::getInstance().changeState(MainMenu);
    return;
}

// PRIVATE

void ManualMenuController::displayManualMenuScreen() {
    const static char title[] PROGMEM = "MODE: MANUEL";
    const static char menuItem1[] PROGMEM = "Selection repas";
    const static char menuItem2[] PROGMEM = "Controle manuel";

    vector<string> menuOptions = {
        getString(menuItem1),
        getString(menuItem2)
    };

    NavigationMenu menu;
    menu.build(getString(title), menuOptions);
    menu.show();
    int selectedOption = menu.waitForSelection();

    switch(selectedOption) {
        case 1:
            StateManager::getInstance().changeState(ManualMealDistribution);
            break;
        case 2:
            StateManager::getInstance().changeState(ManualControl);
            break;
        case -1:
            StateManager::getInstance().changeState(MainMenu);
            break;
    }
}