#include "./controller.h"
#include "../feeder.h"
#include "../state_manager.h"

#ifndef CONTROLLERS_SAFETYCONTROLLER_H
#define CONTROLLERS_SAFETYCONTROLLER_H

using namespace std;

class SafetyController: public Controller {
    public:
        SafetyController(
            Feeder &feederRef
        ) : 
            feeder(feederRef)
        {}

        void handle() {
            displaySafetyStopWarning();
        }

    private:
        Feeder &feeder;
        
        void displaySafetyStopWarning() {
            DisplayService::getInstance().clearScreen();
            DisplayService::getInstance().printTitle("(!) ARRET SECURITE");
            DisplayService::getInstance().addBorder();
            DisplayService::getInstance().printCenter("Continuer [F1]", 2);

            bool canContinue = NavigationMenu::waitForConfirmation();

            if (canContinue) {
                StateManager::getInstance().changeState(MainMenu);
            } else {
                StateManager::getInstance().changeState(MainMenu);
            }
        }
};

#endif