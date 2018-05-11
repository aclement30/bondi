#include "constants.h"
#include "controller.h"
#include "automatic_controller.h"
#include "diagnostic_controller.h"
#include "history_controller.h"
#include "keypad_service.h"
#include "log_service.h"
#include "main_menu_controller.h"
#include "manual_control_controller.h"
#include "manual_meal_distribution_controller.h"
#include "manual_menu_controller.h"
#include "off_controller.h"
#include "settings_menu_controller.h"
#include "time_setting_controller.h"
#include "state_manager.h"
#include "safety_service.h"

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
            // Serial.println(F("MODE: ARRET"));
            break;
        }
        case MainMenu: {
            currentControllerPtr = new MainMenuController();
            // Serial.println(F("MENU PRINCIPAL"));
            break;
        }
        case Automatic: {
            currentControllerPtr = new AutomaticController();
            // Serial.println(F("MODE: AUTO"));
            break;
        }
        case ManualMenu: {
            currentControllerPtr = new ManualMenuController();
            // Serial.println(F("MODE: MANUEL"));
            break;
        }
        case ManualMealDistribution: {
            currentControllerPtr = new ManualMealDistributionController();
            // Serial.println(F("DISTRIBUTION MANUELLE"));
            break;
        }
        case ManualControl: {
            currentControllerPtr = new ManualControlController();
            // Serial.println(F("CONTROLE MANUEL"));
            break;
        }
        case History: {
            currentControllerPtr = new HistoryController();
            // Serial.println(F("HISTORIQUE"));
            break;
        }
        case Diagnostic: {
            currentControllerPtr = new DiagnosticController();
            // Serial.println(F("MODE: DIAGNOSTIC"));
            break;
        }
        case SettingsMenu: {
            currentControllerPtr = new SettingsMenuController();
            // Serial.println(F("CONFIGURATION"));
            break;
        }
        case TimeSetting: {
            currentControllerPtr = new TimeSettingController();
            // Serial.println(F("PARAM. DATE & HEURE"));
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
    // Serial.println(F("* StateManager::safetyStopp"));
    // delay(250);

    safetyModeActivated = true;
    LogService::getInstance().log(SAFETY_MODE_ENGAGED);

    stop();
}

void StateManager::disengageSafetyMode(bool manualAction) {
    safetyModeActivated = false;

    if (manualAction) {
        LogService::getInstance().log(SAFETY_MODE_DISENGAGED);
    } else {
        LogService::getInstance().log(SAFETY_MODE_RETRY);

        SafetyService::getInstance().displayAutoDisengagement();
        KeypadService::getInstance().waitForActivity(2000);
    }

    // Display short warning notice before moving
    SafetyService::getInstance().displayMovingWarning();

    currentControllerPtr->resume();
}

// PRIVATE

StateManager::StateManager() {
    currentState = Off;
    previousState = Off;
    movingDirection = MOVING_IDLE;
    safetyModeActivated = false;
}