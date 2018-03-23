#include "display_service.h"
#include "navigation_menu.h"
#include "state_manager.h"
#include "manual_menu_controller.h"

using namespace std;

void ManualMenuController::handle() {
    displayManualMenuScreen();
}

void ManualMenuController::escape() {
    StateManager::getInstance().changeState(MainMenu);
    return;
}

// PRIVATE

void ManualMenuController::displayManualMenuScreen() {
    vector<string> menuOptions = {
        "Selection repas",
        "Controle manuel"
    };

    NavigationMenu menu;
    menu.build("MODE: MANUEL", menuOptions);
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