#include "diagnostic_service.h"
#include "display_service.h"
#include "keypad_service.h"
#include "navigation_menu.h"
#include "rail_motor.h"
#include "string.h"
#include "rail_motor_diagnostic_service.h"

using namespace std;

RailMotorDiagnosticService::RailMotorDiagnosticService() {}

void RailMotorDiagnosticService::startDiagnostic() {
    completed = false;

    MovingDirection movingDirection = StateManager::getInstance().getMovingDirection();
    if (movingDirection != MOVING_IDLE) {
        const static char errorMsg1[] PROGMEM = "Le robot doit etre";
        const static char errorMsg2[] PROGMEM = "en arret complet.";
        const static char okBtn[] PROGMEM = "OK";

        vector<string> errorMessage = {
            getString(errorMsg1),
            getString(errorMsg2)
        };
        DisplayService::getInstance().showErrorScreen(errorMessage, getString(okBtn));
        KeypadService::getInstance().waitForConfirmation();
        
        cancelled = true;
        return;
    }

    displayDiagnosticScreen();
    displayDirection(MOVING_IDLE);

    Serial.println(F("* start diagnostic"));
}

void RailMotorDiagnosticService::continueDiagnostic() {
    currentStep += 1;

    switch(currentStep) {
        case 1:
            KeypadService::getInstance().waitForActivity(1000);
            displayDirection(MOVING_FORWARD);
            RailMotor::getInstance().moveForward();
        break;
        case 2:
            KeypadService::getInstance().waitForActivity(4000);
            displayDirection(MOVING_IDLE);
            RailMotor::getInstance().stop();
        break;
        case 3:
            KeypadService::getInstance().waitForActivity(1000);
            displayDirection(MOVING_BACKWARD);
            RailMotor::getInstance().moveBackward();
        break;
        case 4:
            KeypadService::getInstance().waitForActivity(4000);
            displayDirection(MOVING_IDLE);
            RailMotor::getInstance().stop();
        break;
        default:
            completed = true;
            currentStep = 0;
            break;
    }
}

void RailMotorDiagnosticService::abortDiagnostic() {
    RailMotor::getInstance().stop();
    displayDirection(MOVING_IDLE);
}

string RailMotorDiagnosticService::getTitle() {
    const static char title[] PROGMEM = "TEST MOTEUR RAIL";
    return getString(title);
}

void RailMotorDiagnosticService::displayDirection(MovingDirection direction) {
    const static char frontDirection[] PROGMEM = ">>> AVANT >>>";
    const static char backDirection[] PROGMEM = "<<< ARRIERE <<<";
    const static char stopped[] PROGMEM = "<ARRET>";

    if (direction == MOVING_FORWARD) {
        DisplayService::getInstance().printCenter(getString(frontDirection), 2);
    } else if (direction == MOVING_BACKWARD) {
        DisplayService::getInstance().printCenter(getString(backDirection), 2);
    } else {
        DisplayService::getInstance().printCenter(getString(stopped), 2);
    }
}

// PRIVATE

void RailMotorDiagnosticService::displayDiagnosticScreen() {
    DisplayService::getInstance().clearScreen();
    DisplayService::getInstance().printTitle(getTitle());
    DisplayService::getInstance().addBorder();
}