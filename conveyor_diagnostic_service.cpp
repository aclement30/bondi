#include "diagnostic_service.h"
#include "conveyor_motor.h"
#include "display_service.h"
#include "keypad_service.h"
#include "navigation_menu.h"
#include "state_manager.h"
#include "conveyor_diagnostic_service.h"

ConveyorDiagnosticService::ConveyorDiagnosticService(
    ConveyorMotor &conveyorRef,
    const char * name
) :
    conveyorMotor(conveyorRef),
    conveyorName(name)
{}

void ConveyorDiagnosticService::startDiagnostic() {
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
    conveyorMotor.stop();
    displaySide(CONVEYOR_IDLE);

    Serial.println("* start diagnostic");
}

void ConveyorDiagnosticService::continueDiagnostic() {
    currentStep += 1;

    switch(currentStep) {
        case 1:
            KeypadService::getInstance().waitForActivity(1000);
            displaySide(CONVEYOR_SIDE_LEFT);
            displaySpeed(25);
            conveyorMotor.start(CONVEYOR_SIDE_LEFT, 25);
        break;
        case 2:
            KeypadService::getInstance().waitForActivity(2000);
            displaySide(CONVEYOR_SIDE_LEFT);
            displaySpeed(50);
            conveyorMotor.start(CONVEYOR_SIDE_LEFT, 50);
        break;
        case 3:
            KeypadService::getInstance().waitForActivity(2000);
            displaySide(CONVEYOR_SIDE_LEFT);
            displaySpeed(75);
            conveyorMotor.start(CONVEYOR_SIDE_LEFT, 75);
        break;
        case 4:
            KeypadService::getInstance().waitForActivity(2000);
            displaySide(CONVEYOR_SIDE_LEFT);
            displaySpeed(100);
            conveyorMotor.start(CONVEYOR_SIDE_LEFT, 100);
        break;
        case 5:
            KeypadService::getInstance().waitForActivity(2000);
            displaySide(CONVEYOR_IDLE);
            conveyorMotor.stop();
        break;
        case 6:
            KeypadService::getInstance().waitForActivity(1000);
            displaySide(CONVEYOR_SIDE_RIGHT);
            displaySpeed(25);
            conveyorMotor.start(CONVEYOR_SIDE_RIGHT, 25);
        break;
        case 7:
            KeypadService::getInstance().waitForActivity(2000);
            displaySide(CONVEYOR_SIDE_RIGHT);
            displaySpeed(50);
            conveyorMotor.start(CONVEYOR_SIDE_RIGHT, 50);
        break;
        case 8:
            KeypadService::getInstance().waitForActivity(2000);
            displaySide(CONVEYOR_SIDE_RIGHT);
            displaySpeed(75);
            conveyorMotor.start(CONVEYOR_SIDE_RIGHT, 75);
        break;
        case 9:
            KeypadService::getInstance().waitForActivity(2000);
            displaySide(CONVEYOR_SIDE_RIGHT);
            displaySpeed(100);
            conveyorMotor.start(CONVEYOR_SIDE_RIGHT, 100);
        break;
        case 10:
            KeypadService::getInstance().waitForActivity(2000);
            displaySide(CONVEYOR_IDLE);
            conveyorMotor.stop();
        break;
        default:
            completed = true;
            currentStep = 0;
            break;
    }
}

void ConveyorDiagnosticService::abortDiagnostic() {
    conveyorMotor.stop();
    displaySide(CONVEYOR_IDLE);
}

string ConveyorDiagnosticService::getTitle() {
    char title[21] = "TEST ";
    return strcpy(title, conveyorName);
}

void ConveyorDiagnosticService::displaySide(ConveyorSide side) {
    if (side == CONVEYOR_SIDE_RIGHT) {
        DisplayService::getInstance().printCenter(">>> DROITE >>>", 2);
    } else if (side == CONVEYOR_SIDE_LEFT) {
        DisplayService::getInstance().printCenter("<<< GAUCHE <<<", 2);
    } else {
        DisplayService::getInstance().printCenter("<ARRET>", 2);
        DisplayService::getInstance().print("", 3);
    }
}

void ConveyorDiagnosticService::displaySpeed(int speed) {
    char speedText[20];
    sprintf(speedText, "%d %%", speed);
    DisplayService::getInstance().printCenter(string(speedText), 3);
}

// PRIVATE

void ConveyorDiagnosticService::displayDiagnosticScreen() {
    DisplayService::getInstance().clearScreen();
    DisplayService::getInstance().printTitle(getTitle());
    DisplayService::getInstance().addBorder();
}