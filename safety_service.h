#include "constants.h"
#include "feeder.h"
#include "state_manager.h"

#ifndef SAFETYSERVICE_H
#define SAFETYSERVICE_H

using namespace std;

class SafetyService {
    public:
        SafetyService() {
            pinMode(SAFETY_SENSOR_FRONT, INPUT);
            pinMode(SAFETY_SENSOR_BACK, INPUT);
        }

        void checkSafetyState() {
            bool safetyMode = StateManager::getInstance().isSafetyMode();

            if (!safetyMode && isSafetyBarPressed()) {
                Serial.println("Barre de sécurité enclenchée");

                // Shutdown everything immediately
                StateManager::getInstance().safetyStop();

                displaySafetyStopWarning();
            }
        }

    private:        
        void displaySafetyStopWarning() {
            DisplayService::getInstance().clearScreen();
            DisplayService::getInstance().printTitle("(!) ARRET SECURITE");
            DisplayService::getInstance().addBorder();
            DisplayService::getInstance().printCenter("Continuer [F1]", 2);

            bool canContinue = NavigationMenu::waitForConfirmation();

            if (canContinue) {
                return;
            } else {
                StateManager::getInstance().changeState(MainMenu);
            }
        }

        bool isSafetyBarPressed() {
            MovingDirection movingDirection = StateManager::getInstance().getMovingDirection();

            return (movingDirection == MOVING_FORWARD && digitalRead(SAFETY_SENSOR_FRONT) == HIGH) 
                || (movingDirection == MOVING_BACKWARD && digitalRead(SAFETY_SENSOR_BACK) == HIGH);
        }
};

#endif