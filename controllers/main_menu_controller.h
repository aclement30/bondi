#include "./controller.h"
#include "../display_service.h"
#include "../navigation_menu.h"
#include "../state_manager.h"

#ifndef CONTROLLERS_MAINMENUCONTROLLER_H
#define CONTROLLERS_MAINMENUCONTROLLER_H

using namespace std;

class StateManager;

class MainMenuController: public Controller {
    public:
        MainMenuController() {}

        void handle() {
            displayNavMenu();
        }

    private:
        void displayNavMenu() {
            vector<string> menuOptions = {
                "Mode AUTO.",
                "Mode MANUEL",
                "Historique",
                "Diagnostic",
                "Redemarrage"
            };

            NavigationMenu menu = NavigationMenu("MENU PRINCIPAL", menuOptions, "");
            menu.show();
            int selectedOption = menu.waitForSelection();

            switch(selectedOption) {
                case 1:
                    StateManager::getInstance().changeState(Automatic);
                    break;
                case 2:
                    StateManager::getInstance().changeState(Manual);
                    break;
                case 3:
                    // TODO: Switch for History mode
                    break;
                case 4:
                    StateManager::getInstance().changeState(Diagnostic);
                    break;
                case 5:
                    //asm volatile ("  jmp 0");
                    break;
            }
        }
};

#endif