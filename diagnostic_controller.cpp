#include "diagnostic_service.h"
#include "conveyor_diagnostic_service.h"
#include "rail_motor_diagnostic_service.h"
#include "route_mapping_diagnostic_service.h"
#include "display_service.h"
#include "keypad_service.h"
#include "navigation_menu.h"
#include "state_manager.h"
#include "diagnostic_controller.h"

using namespace std;

DiagnosticController::DiagnosticController(
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

void DiagnosticController::handle() {
    if (diagnosticPtr == NULL) {
        displayNavMenu();
    } else if (diagnosticPtr->isCompleted()) {    
        completeDiagnostic();
    } else if (diagnosticPtr->isCancelled()) {    
        stopDiagnostic();
    } else {
        diagnosticPtr->continueDiagnostic();
    }
}

void DiagnosticController::escape() {
    if (diagnosticPtr == NULL) {
        // Immediately go back to main menu
        StateManager::getInstance().changeState(MainMenu);
        return;
    }
    
    diagnosticPtr->abortDiagnostic();
    stopDiagnostic();

    // Will return to diagnostic menu on next loop
}

void DiagnosticController::stopDiagnostic() {
    delete diagnosticPtr;
    diagnosticPtr = NULL;
}

void DiagnosticController::completeDiagnostic() {
    displayCompletionScreen();
}

// PRIVATE

void DiagnosticController::displayNavMenu() {
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

void DiagnosticController::displayConfirmationScreen() {
    string diagnosticTitle = diagnosticPtr->getTitle();

    DisplayService::getInstance().clearScreen();
    DisplayService::getInstance().printTitle(diagnosticTitle);
    DisplayService::getInstance().addBorder();
    DisplayService::getInstance().printCenter("Commencer [F1]", 2);

    bool canStart = KeypadService::getInstance().waitForConfirmation();

    if (canStart) {
        diagnosticPtr->startDiagnostic();
    } else {
        stopDiagnostic();
    }
}

void DiagnosticController::displayCompletionScreen() {
    string diagnosticTitle = diagnosticPtr->getTitle();

    DisplayService::getInstance().clearScreen();
    DisplayService::getInstance().printTitle(diagnosticTitle);
    DisplayService::getInstance().addBorder();
    DisplayService::getInstance().printCenter("Recommencer [F1]", 2);
    DisplayService::getInstance().printCenter("Quitter [F4]", 3);

    bool restart = KeypadService::getInstance().waitForConfirmation();

    if (restart) {
        diagnosticPtr->startDiagnostic();
    } else {
        stopDiagnostic();
    }
}