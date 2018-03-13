#include "./controller.h"
#include "../diagnostic/diagnostic_service.h"
#include "../diagnostic/route_mapping.h"
#include "../display_service.h"
#include "../feeder.h"
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
            LocationService &locationServiceRef
        ) : 
            locationService(locationServiceRef)
        {}

        void handle() {
            if (diagnosticPtr == NULL) {
                showNavMenu();
            } else if (diagnosticPtr->isCompleted()) {                    
                completeDiagnostic();
            } else {
                diagnosticPtr->continueDiagnostic();
                delay(1000);
            }
        }

        void stopDiagnostic() {
            diagnosticPtr = NULL;
        }

        void completeDiagnostic() {
            showDiagnosticCompleted();
        }

    private:
        DiagnosticService *diagnosticPtr = NULL;
        LocationService &locationService;
        
        void showNavMenu() {
            vector<string> menuOptions = {
                "Liste routes",
                "Test mot. rail",
                "Test conv. av",
                "Test conv. arr"
            };

            NavigationMenu menu = NavigationMenu("DIAGNOSTIC", menuOptions);
            menu.show();
            int selectedOption = menu.waitForSelection();

            switch(selectedOption) {
                case 1: {
                    RouteMappingDiagnosticService routeMappingDiagnostic = RouteMappingDiagnosticService(locationService);
                    diagnosticPtr = &routeMappingDiagnostic;

                    showDiagnosticConfirmation();
                    break;
                }
                // case 2:
                //     DisplayScreens::diagnosticRailMotor();
                //     break;
                // case 3:
                //     DisplayScreens::diagnosticConveyorMotor();
                //     break;
                // case 4:
                //     DisplayScreens::diagnosticConveyorMotor();
                //     break;
                case -1:
                    StateManager::getInstance().changeState(MainMenu);
                    break;
            }
        }

        void showDiagnosticConfirmation() {
            string diagnosticTitle = diagnosticPtr->getTitle();

            DisplayService::getInstance().clearScreen();
            DisplayService::getInstance().printTitle(diagnosticTitle);
            DisplayService::getInstance().addBorder();
            DisplayService::getInstance().printCenter("Commencer [F1]", 2);

            bool canStart = NavigationMenu::waitForConfirmation();

            if (canStart) {
                diagnosticPtr->startDiagnostic();
                delay(1000);
            } else {
                stopDiagnostic();
            }
        }

        void showDiagnosticCompleted() {
            string diagnosticTitle = diagnosticPtr->getTitle();

            DisplayService::getInstance().clearScreen();
            DisplayService::getInstance().printTitle(diagnosticTitle);
            DisplayService::getInstance().addBorder();
            DisplayService::getInstance().printCenter("Recommencer [F1]", 2);
            DisplayService::getInstance().printCenter("Quitter [F4]", 3);

            bool restart = NavigationMenu::waitForConfirmation();

            if (restart) {
                diagnosticPtr->startDiagnostic();
            } else {
                stopDiagnostic();
            }
        }
};

#endif