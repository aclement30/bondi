#include "conveyor_motor.h"
#include "display_service.h"
#include "keypad_service.h"
#include "log_service.h"
#include "rail_motor.h"
#include "state_manager.h"
#include "string.h"
#include "manual_control_controller.h"

ManualControlController::ManualControlController() {
    LogService::getInstance().log(MANUAL_CONTROL_START);
}

void ManualControlController::handle() {
    MovingDirection movingDirection = StateManager::getInstance().getMovingDirection();
    bool conveyorsMoving = FrontConveyor::getInstance().getSpeed() != 0 || BackConveyor::getInstance().getSpeed() != 0;

    displayManualControlScreen(movingDirection == MOVING_IDLE && !conveyorsMoving);
}

void ManualControlController::escape() {
    // Stop all motors
    RailMotor::getInstance().stop();
    FrontConveyor::getInstance().stop();
    BackConveyor::getInstance().stop();

    LogService::getInstance().log(MANUAL_CONTROL_END);

    // Go back to main menu
    StateManager::getInstance().changeState(ManualMenu);
    return;
}

// PRIVATE

void ManualControlController::displayManualControlScreen(bool showInvite) {
    const static char title[] PROGMEM = "CONTROLE MANUEL";

    DisplayService::getInstance().clearScreen();
    DisplayService::getInstance().printTitle(getString(title));
    DisplayService::getInstance().addBorder();

    if (showInvite) {
        displayInvite();
    } else {
        MovingDirection movingDirection = StateManager::getInstance().getMovingDirection();
        displayMovingDirection(movingDirection);

        bool frontConveyorMoving = FrontConveyor::getInstance().getSpeed() != 0;
        bool backConveyorMoving = BackConveyor::getInstance().getSpeed() != 0;
        displayConveyorsState(frontConveyorMoving, backConveyorMoving);
    }

    int selection = KeypadService::getInstance().waitForSelection();
    handleCommand(selection);
}

void ManualControlController::handleCommand(int key) {
    MovingDirection movingDirection = StateManager::getInstance().getMovingDirection();

    switch(key) {
        case 1:
            if (movingDirection == MOVING_FORWARD) {
                RailMotor::getInstance().stop();
                displayMovingDirection(MOVING_IDLE);
            } else if (movingDirection == MOVING_BACKWARD) {
                RailMotor::getInstance().inverseMovingDirection();
                displayMovingDirection(MOVING_FORWARD);
            } else {
                RailMotor::getInstance().moveForward();
                displayMovingDirection(MOVING_FORWARD);
            }
            break;
        case 2:
            if (movingDirection == MOVING_BACKWARD) {
                RailMotor::getInstance().stop();
                displayMovingDirection(MOVING_IDLE);
            } else if (movingDirection == MOVING_FORWARD) {
                RailMotor::getInstance().inverseMovingDirection();
                displayMovingDirection(MOVING_BACKWARD);
            } else {
                RailMotor::getInstance().moveBackward();
                displayMovingDirection(MOVING_BACKWARD);
            }
            break;
        case 7: {
            bool frontConveyorMoving = FrontConveyor::getInstance().getSpeed() != 0;
            bool backConveyorMoving = BackConveyor::getInstance().getSpeed() != 0;

            if (frontConveyorMoving) {
                FrontConveyor::getInstance().stop();
                frontConveyorMoving = false;
            } else {
                FrontConveyor::getInstance().start(CONVEYOR_SIDE_RIGHT, 100);
                frontConveyorMoving = true;
            }

            displayConveyorsState(frontConveyorMoving, backConveyorMoving);
            break;
        }
        case 8: {
            bool frontConveyorMoving = FrontConveyor::getInstance().getSpeed() != 0;
            bool backConveyorMoving = BackConveyor::getInstance().getSpeed() != 0;

            if (backConveyorMoving) {
                BackConveyor::getInstance().stop();
                backConveyorMoving = false;
            } else {
                BackConveyor::getInstance().start(CONVEYOR_SIDE_RIGHT, 100);
                backConveyorMoving = true;
            }

            displayConveyorsState(frontConveyorMoving, backConveyorMoving);
            break;
        }
        default:
            Serial.print("Key: ");
            Serial.println(key);
            if (key == KeypadService::EscapeKey) {
                escape();
            }
            break;
    }
}

void ManualControlController::displayInvite() {
    const static char line1[] PROGMEM = "1/2: Avancer/Reculer";
    const static char line2[] PROGMEM = "7/8: Conv. av/arr.";

    DisplayService::getInstance().print(getString(line1), 2);
    DisplayService::getInstance().print(getString(line2), 3);
}

void ManualControlController::displayMovingDirection(MovingDirection direction) {
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

void ManualControlController::displayConveyorsState(bool frontConveyorMoving, bool backConveyorMoving) {
    const static char moving[] PROGMEM = ">>>";
    const static char stopped[] PROGMEM = "[-]";

    char frontConveyorState[4];
    if (frontConveyorMoving) {
        strcpy(frontConveyorState, getString(moving));
    } else {
        strcpy(frontConveyorState, getString(stopped));
    }
    
    char backConveyorState[4];
    if (backConveyorMoving) {
        strcpy(backConveyorState, getString(moving));
    } else {
        strcpy(backConveyorState, getString(stopped));
    }
    
    char line[21];
    sprintf(line, "C.AV %s | C.ARR %s", frontConveyorState, backConveyorState);
    
    DisplayService::getInstance().printCenter(line, 3);
}