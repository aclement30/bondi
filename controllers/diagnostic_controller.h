#include "./controller.h"
#include "../diagnostic/diagnostic_service.h"
#include "../diagnostic/conveyor.h"
#include "../diagnostic/rail_motor.h"
#include "../diagnostic/route_mapping.h"
#include "../display_service.h"
#include "../navigation_menu.h"
#include "../state_manager.h"

#ifndef CONTROLLERS_DIAGNOSTICCONTROLLER_H
#define CONTROLLERS_DIAGNOSTICCONTROLLER_H

using namespace std;

class LocationService;
class StateManager;

class DiagnosticController: public Controller {
    public:
        DiagnosticController(
            LocationService & locationServiceRef,
            RailMotor & railMotorRef,
            ConveyorMotor & conveyorFrontRef,
            ConveyorMotor & conveyorBackRef
        ) : 
            locationService(locationServiceRef),
            railMotor(railMotorRef),
            conveyorFront(conveyorFrontRef),
            conveyorBack(conveyorBackRef)
        {}

        void handle() {
            // Serial.println("* DiagnosticController->handle");
            // delay(250);

            if (diagnosticPtr == NULL) {
                Serial.println("* no diagnostic");
                delay(250);

                displayNavMenu();
            } else if (diagnosticPtr->isCompleted()) {    
                Serial.println("* diagnostic completed");
                delay(250);

                completeDiagnostic();
            } else if (diagnosticPtr->isCancelled()) {    
                Serial.println("** diagnostic cancelled");
                delay(250);

                stopDiagnostic();
            } else {
                Serial.println("*** continue diagnostic");
                delay(250);

                diagnosticPtr->continueDiagnostic();
            }
        }

        void escape() {
            if (diagnosticPtr == NULL) {
                // Immediately go back to main menu
                StateManager::getInstance().changeState(MainMenu);
                return;
            }
            
            diagnosticPtr->abortDiagnostic();
            stopDiagnostic();

            // Will return to diagnostic menu on next loop
        }

        void stopDiagnostic() {
            Serial.println("* clear diagnostic service");

            delete diagnosticPtr;
            diagnosticPtr = NULL;
        }

        void completeDiagnostic() {
            displayCompletionScreen();
        }

    private:
        DiagnosticService *diagnosticPtr = NULL;
        LocationService &locationService;
        RailMotor &railMotor;
        ConveyorMotor &conveyorFront;
        ConveyorMotor &conveyorBack;

        void displayNavMenu() {
             vector<string> menuOptions = {
                "Liste routes",
                "Test mot. rail",
                "Test conv. av",
                "Test conv. arr"
            };

            NavigationMenu menu;
            menu.build("DIAGNOSTIC", menuOptions);
            menu.show();
            
            int selectedOption = menu.waitForSelection();

            switch(selectedOption) {
                case 1:
                    diagnosticPtr = new RouteMappingDiagnosticService(locationService);
                    displayConfirmationScreen();
                    break;
                case 2:
                    diagnosticPtr = new RailMotorDiagnosticService(railMotor);
                    displayConfirmationScreen();
                    break;
                case 3:
                    diagnosticPtr = new ConveyorDiagnosticService(conveyorFront, "CONVOYEUR AVANT");
                    displayConfirmationScreen();
                    break;
                case 4:
                    diagnosticPtr = new ConveyorDiagnosticService(conveyorBack, "CONVOYEUR ARRIERE");
                    displayConfirmationScreen();
                    break;
                case -1:
                    StateManager::getInstance().changeState(MainMenu);
                    break;
            }
        }

        void displayConfirmationScreen() {
            string diagnosticTitle = diagnosticPtr->getTitle();

            DisplayService::getInstance().clearScreen();
            DisplayService::getInstance().printTitle(diagnosticTitle);
            DisplayService::getInstance().addBorder();
            DisplayService::getInstance().printCenter("Commencer [F1]", 2);

            bool canStart = DisplayService::waitForConfirmation();

            if (canStart) {
                diagnosticPtr->startDiagnostic();
            } else {
                stopDiagnostic();
            }
        }

        void displayCompletionScreen() {
            string diagnosticTitle = diagnosticPtr->getTitle();

            DisplayService::getInstance().clearScreen();
            DisplayService::getInstance().printTitle(diagnosticTitle);
            DisplayService::getInstance().addBorder();
            DisplayService::getInstance().printCenter("Recommencer [F1]", 2);
            DisplayService::getInstance().printCenter("Quitter [F4]", 3);

            bool restart = DisplayService::waitForConfirmation();

            if (restart) {
                diagnosticPtr->startDiagnostic();
            } else {
                stopDiagnostic();
            }
        }
};

#endif