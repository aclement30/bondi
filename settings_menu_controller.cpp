#include "display_service.h"
#include "navigation_menu.h"
#include "state_manager.h"
#include "string.h"
#include "settings_menu_controller.h"

using namespace std;

void SettingsMenuController::handle() {
    displaySettingsMenuScreen();
}

void SettingsMenuController::escape() {
    StateManager::getInstance().changeState(MainMenu);
    return;
}

// PRIVATE

void SettingsMenuController::displaySettingsMenuScreen() {
    const static char title[] PROGMEM = "CONFIGURATION";
    const static char menuItem1[] PROGMEM = "Date & heure";

    vector<string> menuOptions = {
        getString(menuItem1)
    };

    NavigationMenu menu;
    menu.build(getString(title), menuOptions);
    menu.show();
    int selectedOption = menu.waitForSelection();

    switch(selectedOption) {
        case 1:
            StateManager::getInstance().changeState(TimeSetting);
            break;
        case -1:
            StateManager::getInstance().changeState(MainMenu);
            break;
    }
}