#include "display_service.h"
#include "navigation_menu.h"
#include "state_manager.h"
#include "main_menu_controller.h"

using namespace std;

void MainMenuController::handle() {
    displayNavMenu();
}

// Do nothing
void MainMenuController::escape() {}

// PRIVATE

void MainMenuController::displayNavMenu() {
    vector<string> menuOptions = {
        "Mode AUTO.",
        "Mode MANUEL",
        "Historique",
        "Diagnostic",
        "Redemarrage"
    };

    NavigationMenu menu;
    menu.escapable = false;
    menu.build("MENU PRINCIPAL", menuOptions);
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
            //asm volatile ("  jmp 0");
            break;
    }
}