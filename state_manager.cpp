#include "constants.h"
#include "state_manager.h"

using namespace std;

MachineState StateManager::getState() {
    return currentState;
}

MachineState StateManager::getPreviousState() {
    return previousState;
}

void StateManager::changeState(MachineState newState) {
    if (currentState == newState) {
        return;
    }

    previousState = currentState;
    currentState = newState;
    
    switch(currentState) {
        case Off: {
            Serial.println(F("MODE: ARRET"));
            break;
        }
        case MainMenu: {
            Serial.println(F("MENU PRINCIPAL"));
            break;
        }
        case Automatic: {
            Serial.println(F("MODE: AUTO"));
            break;
        }
        case ManualMenu: {
            Serial.println(F("MODE: MANUEL"));
            break;
        }
        case ManualMealDistribution: {
            Serial.println(F("DISTRIBUTION MANUELLE"));
            break;
        }
        case ManualControl: {
            Serial.println(F("CONTROLE MANUEL"));
            break;
        }
        case Diagnostic: {
            Serial.println(F("MODE: DIAGNOSTIC"));
            break;
        }
    }
}

MovingDirection StateManager::getMovingDirection() {
    return movingDirection;
}

void StateManager::changeMovingDirection(MovingDirection newDirection) {
    if (movingDirection == newDirection) return;

    movingDirection = newDirection;
}

void StateManager::stop() {
    changeMovingDirection(MOVING_IDLE);
}

bool StateManager::isSafetyMode() {
    return safetyModeActivated;
}

void StateManager::safetyStop() {
    safetyModeActivated = true;
    stop();
}

void StateManager::disengageSafetyMode() {
    safetyModeActivated = false;
}

// PRIVATE

StateManager::StateManager() {
    currentState = Off;
    previousState = Off;
    movingDirection = MOVING_IDLE;
    safetyModeActivated = false;
}