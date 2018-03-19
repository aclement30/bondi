#include "./controller.h"
#include "../display_service.h"
#include "../state_manager.h"

#ifndef CONTROLLERS_MANUALMENUCONTROLLER_H
#define CONTROLLERS_MANUALMENUCONTROLLER_H

using namespace std;

class ManualMenuController: public Controller {
    public:
        void handle() {
            displayManualMenuScreen();
        }

        void escape() {
            StateManager::getInstance().changeState(MainMenu);
            return;
        }

    private:
        void displayManualMenuScreen() {
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
};

#endif