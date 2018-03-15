#include "diagnostic_service.h"
#include "../conveyor_motor.h"
#include "../display_service.h"
#include "../navigation_menu.h"

#ifndef DIAGNOSTIC_CONVEYOR_H
#define DIAGNOSTIC_CONVEYOR_H

using namespace std;

class ConveyorDiagnosticService: public DiagnosticService {
    public:
        ConveyorDiagnosticService(
            ConveyorMotor &conveyorRef,
            const char * name
        ) :
            conveyorMotor(conveyorRef),
            conveyorName(name)
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
                NavigationMenu::waitForConfirmation();
                
                cancelled = true;
                return;
            }

            displayDiagnosticScreen();
            conveyorMotor.stop();
            displaySide(CONVEYOR_IDLE);

            Serial.println("* start diagnostic");
        }

        void continueDiagnostic() {
            currentStep += 1;

            switch(currentStep) {
                case 1:
                    delay(1000);
                    displaySide(CONVEYOR_SIDE_LEFT);
                    displaySpeed(100);
                    conveyorMotor.start(CONVEYOR_SIDE_LEFT, 100);
                break;
                case 2:
                    delay(2000);
                    displaySide(CONVEYOR_SIDE_LEFT);
                    displaySpeed(200);
                    conveyorMotor.start(CONVEYOR_SIDE_LEFT, 200);
                break;
                case 3:
                    delay(2000);
                    displaySide(CONVEYOR_SIDE_LEFT);
                    displaySpeed(300);
                    conveyorMotor.start(CONVEYOR_SIDE_LEFT, 300);
                break;
                case 4:
                    delay(2000);
                    displaySide(CONVEYOR_SIDE_LEFT);
                    displaySpeed(400);
                    conveyorMotor.start(CONVEYOR_SIDE_LEFT, 400);
                break;
                case 5:
                    delay(2000);
                    displaySide(CONVEYOR_IDLE);
                    conveyorMotor.stop();
                break;
                case 6:
                    delay(1000);
                    displaySide(CONVEYOR_SIDE_RIGHT);
                    displaySpeed(100);
                    conveyorMotor.start(CONVEYOR_SIDE_RIGHT, 100);
                break;
                case 7:
                    delay(2000);
                    displaySide(CONVEYOR_SIDE_RIGHT);
                    displaySpeed(200);
                    conveyorMotor.start(CONVEYOR_SIDE_RIGHT, 200);
                break;
                case 8:
                    delay(2000);
                    displaySide(CONVEYOR_SIDE_RIGHT);
                    displaySpeed(300);
                    conveyorMotor.start(CONVEYOR_SIDE_RIGHT, 300);
                break;
                case 9:
                    delay(2000);
                    displaySide(CONVEYOR_SIDE_RIGHT);
                    displaySpeed(400);
                    conveyorMotor.start(CONVEYOR_SIDE_RIGHT, 400);
                break;
                case 10:
                    delay(2000);
                    displaySide(CONVEYOR_IDLE);
                    conveyorMotor.stop();
                break;
                default:
                    completed = true;
                    currentStep = 0;
                    break;
            }
        }

        string getTitle() {
            char title[21] = "TEST ";
            return strcpy(title, conveyorName);
        }

        void displaySide(ConveyorSide side) {
            if (side == CONVEYOR_SIDE_RIGHT) {
                DisplayService::getInstance().printCenter(">>> DROITE >>>", 2);
            } else if (side == CONVEYOR_SIDE_LEFT) {
                DisplayService::getInstance().printCenter("<<< GAUCHE <<<", 2);
            } else {
                DisplayService::getInstance().printCenter("<ARRET>", 2);
                DisplayService::getInstance().print("", 3);
            }
        }

        void displaySpeed(int speed) {
            char speedText[20];
            sprintf(speedText, "%d rpm", speed);
            DisplayService::getInstance().printCenter(string(speedText), 3);
        }

    private:
        ConveyorMotor &conveyorMotor;
        const char * conveyorName;
        int currentStep = 0;

        void displayDiagnosticScreen() {
            DisplayService::getInstance().clearScreen();
            DisplayService::getInstance().printTitle(getTitle());
            DisplayService::getInstance().addBorder();
        }
};

#endif