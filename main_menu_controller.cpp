#include "display_service.h"
#include "log_service.h"
#include "navigation_menu.h"
#include "state_manager.h"
#include "string.h"
#include "main_menu_controller.h"

using namespace std;

void MainMenuController::handle() {
    displayNavMenu();
}

// PRIVATE

void MainMenuController::displayNavMenu() {
    const static char title[] PROGMEM = "MENU PRINCIPAL";
    const static char menuItem1[] PROGMEM = "Mode AUTO.";
    const static char menuItem2[] PROGMEM = "Mode MANUEL";
    const static char menuItem3[] PROGMEM = "Historique";
    const static char menuItem4[] PROGMEM = "Diagnostic";
    const static char menuItem5[] PROGMEM = "Redemarrage";

    vector<string> menuOptions = {
        getString(menuItem1),
        getString(menuItem2),
        getString(menuItem3),
        getString(menuItem4),
        getString(menuItem5)
    };

    NavigationMenu menu;
    menu.escapable = false;
    menu.build(getString(title), menuOptions);
    menu.show();
    int selectedOption = menu.waitForSelection();

    switch(selectedOption) {
        case 1:
            StateManager::getInstance().changeState(Automatic);
            break;
        case 2:
            StateManager::getInstance().changeState(ManualMenu);
            break;
        case 3:
            StateManager::getInstance().changeState(History);
            break;
        case 4:
            StateManager::getInstance().changeState(Diagnostic);
            break;
        case 5:
            LogService::getInstance().log(MANUAL_RESTART);

            asm volatile ("  jmp 0");
            break;
    }
}