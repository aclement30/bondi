#include "diagnostic_service.h"
#include "../display_service.h"
#include "../keypad_service.h"
#include "../navigation_menu.h"
#include "../rail_motor.h"

#ifndef DIAGNOSTIC_RAILMOTOR_H
#define DIAGNOSTIC_RAILMOTOR_H

using namespace std;

class RailMotorDiagnosticService: public DiagnosticService {
    public:
        RailMotorDiagnosticService(
            RailMotor &motorRef
        ) :
            railMotor(motorRef)
        {}

        void startDiagnostic() {
            completed = false;

            MovingDirection movingDirection = StateManager::getInstance().getMovingDirection();
            if (movingDirection != MOVING_IDLE) {
                vector<string> errorMessage = {
                    "Le robot doit etre",
                    "en arret complet."
                };
                DisplayService::getInstance().showErrorScreen(errorMessage, string("OK"));
                KeypadService::getInstance().waitForConfirmation();
                
                cancelled = true;
                return;
            }

            displayDiagnosticScreen();
            displayDirection(MOVING_IDLE);

            Serial.println("* start diagnostic");
        }

        void continueDiagnostic() {
            currentStep += 1;

            switch(currentStep) {
                case 1:
                    KeypadService::getInstance().waitForActivity(1000);
                    displayDirection(MOVING_FORWARD);
                    railMotor.moveForward();
                break;
                case 2:
                    KeypadService::getInstance().waitForActivity(4000);
                    displayDirection(MOVING_IDLE);
                    railMotor.stop();
                break;
                case 3:
                    KeypadService::getInstance().waitForActivity(1000);
                    displayDirection(MOVING_BACKWARD);
                    railMotor.moveBackward();
                break;
                case 4:
                    KeypadService::getInstance().waitForActivity(4000);
                    displayDirection(MOVING_IDLE);
                    railMotor.stop();
                break;
                default:
                    completed = true;
                    currentStep = 0;
                    break;
            }
        }

        void abortDiagnostic() {
            railMotor.stop();
            displayDirection(MOVING_IDLE);
        }

        string getTitle() {
            return "TEST MOTEUR RAIL";
        }

        void displayDirection(MovingDirection direction) {
            if (direction == MOVING_FORWARD) {
                DisplayService::getInstance().printCenter(">>> AVANT >>>", 2);
            } else if (direction == MOVING_BACKWARD) {
                DisplayService::getInstance().printCenter("<<< ARRIERE <<<", 2);
            } else {
                DisplayService::getInstance().printCenter("<ARRET>", 2);
            }
        }

    private:
        RailMotor &railMotor;
        int currentStep = 0;

        void displayDiagnosticScreen() {
            DisplayService::getInstance().clearScreen();
            DisplayService::getInstance().printTitle(getTitle());
            DisplayService::getInstance().addBorder();
        }
};

#endif