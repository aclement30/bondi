#include "diagnostic_service.h"
#include "conveyor_diagnostic_service.h"
#include "rail_motor_diagnostic_service.h"
#include "route_mapping_diagnostic_service.h"
#include "rfid_reader_diagnostic_service.h"
#include "display_service.h"
#include "keypad_service.h"
#include "navigation_menu.h"
#include "state_manager.h"
#include "string.h"
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
    const static char title[] PROGMEM = "DIAGNOSTIC";
    const static char menuItem1[] PROGMEM = "Liste routes";
    const static char menuItem2[] PROGMEM = "Test mot. rail";
    const static char menuItem3[] PROGMEM = "Test conv. av";
    const static char menuItem4[] PROGMEM = "Test conv. arr";
    const static char menuItem5[] PROGMEM = "Test lect. RFID";

    vector<string> menuOptions = {
        getString(menuItem1),
        getString(menuItem2),
        getString(menuItem3),
        getString(menuItem4),
        getString(menuItem5)
    };

    NavigationMenu menu;
    menu.build(getString(title), menuOptions);
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
        case 3: {
            const static char conveyorName[] PROGMEM = "CONVOYEUR AVANT";
            diagnosticPtr = new ConveyorDiagnosticService(conveyorFront, getString(conveyorName));
            displayConfirmationScreen();
            break;
        }
        case 4: {
            const static char conveyorName[] PROGMEM = "CONVOYEUR ARRIERE";
            diagnosticPtr = new ConveyorDiagnosticService(conveyorBack, getString(conveyorName));
            displayConfirmationScreen();
            break;
        }
        case 5:
            diagnosticPtr = new RfidReaderDiagnosticService(locationService);
            displayConfirmationScreen();
            break;
        case -1:
            StateManager::getInstance().changeState(MainMenu);
            break;
    }
}

void DiagnosticController::displayConfirmationScreen() {
    string diagnosticTitle = diagnosticPtr->getTitle();
    const static char startButton[] PROGMEM = "Commencer [#]";

    DisplayService::getInstance().clearScreen();
    DisplayService::getInstance().printTitle(diagnosticTitle);
    DisplayService::getInstance().addBorder();
    DisplayService::getInstance().printCenter(getString(startButton), 2);

    bool canStart = KeypadService::getInstance().waitForConfirmation();

    if (canStart) {
        diagnosticPtr->startDiagnostic();
    } else {
        stopDiagnostic();
    }
}

void DiagnosticController::displayCompletionScreen() {
    string diagnosticTitle = diagnosticPtr->getTitle();
    const static char restartButton[] PROGMEM = "Recommencer [#]";
    const static char exitButton[] PROGMEM = "Quitter [*]";

    DisplayService::getInstance().clearScreen();
    DisplayService::getInstance().printTitle(diagnosticTitle);
    DisplayService::getInstance().addBorder();
    DisplayService::getInstance().printCenter(getString(restartButton), 2);
    DisplayService::getInstance().printCenter(getString(exitButton), 3);

    bool restart = KeypadService::getInstance().waitForConfirmation();

    if (restart) {
        diagnosticPtr->startDiagnostic();
    } else {
        stopDiagnostic();
    }
}