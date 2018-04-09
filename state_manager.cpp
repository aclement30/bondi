#include "constants.h"
#include "controller.h"
#include "automatic_controller.h"
#include "diagnostic_controller.h"
#include "history_controller.h"
#include "log_service.h"
#include "main_menu_controller.h"
#include "manual_control_controller.h"
#include "manual_meal_distribution_controller.h"
#include "manual_menu_controller.h"
#include "off_controller.h"
#include "state_manager.h"

using namespace std;

MachineState StateManager::getState() {
    return currentState;
}

MachineState StateManager::getPreviousState() {
    return previousState;
}

void StateManager::changeState(MachineState newState) {
    if (currentControllerPtr != NULL) {
        if (currentState == newState) {
            return;
        }

        delete currentControllerPtr;
        currentControllerPtr = NULL;
    }

    previousState = currentState;
    currentState = newState;
    
    switch(currentState) {
        case Off: {
            currentControllerPtr = new OffController();
            Serial.println(F("MODE: ARRET"));
            break;
        }
        case MainMenu: {
            currentControllerPtr = new MainMenuController();
            Serial.println(F("MENU PRINCIPAL"));
            break;
        }
        case Automatic: {
            currentControllerPtr = new AutomaticController();
            Serial.println(F("MODE: AUTO"));
            break;
        }
        case ManualMenu: {
            currentControllerPtr = new ManualMenuController();
            Serial.println(F("MODE: MANUEL"));
            break;
        }
        case ManualMealDistribution: {
            currentControllerPtr = new ManualMealDistributionController();
            Serial.println(F("DISTRIBUTION MANUELLE"));
            break;
        }
        case ManualControl: {
            currentControllerPtr = new ManualControlController();
            Serial.println(F("CONTROLE MANUEL"));
            break;
        }
        case History: {
            currentControllerPtr = new HistoryController();
            Serial.println(F("HISTORIQUE"));
            break;
        }
        case Diagnostic: {
            currentControllerPtr = new DiagnosticController();
            Serial.println(F("MODE: DIAGNOSTIC"));
            break;
        }
    }
}

Controller * StateManager::getCurrentController() {
    return currentControllerPtr;
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
    LogService::getInstance().log(SAFETY_MODE_ENGAGED);

    stop();
}

void StateManager::disengageSafetyMode() {
    safetyModeActivated = false;
    LogService::getInstance().log(SAFETY_MODE_DISENGAGED);

    currentControllerPtr->resume();
}

// PRIVATE

StateManager::StateManager() {
    currentState = Off;
    previousState = Off;
    movingDirection = MOVING_IDLE;
    safetyModeActivated = false;
}