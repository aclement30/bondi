#include "constants.h"
#include "display_service.h"
#include "keypad_service.h"
#include "state_manager.h"
#include "safety_service.h"

SafetyService::SafetyService() {
    pinMode(SAFETY_SENSOR_FRONT, INPUT);
    pinMode(SAFETY_SENSOR_BACK, INPUT);
}

void SafetyService::checkSafetyState() {
    bool safetyMode = StateManager::getInstance().isSafetyMode();

    if (!safetyMode && isSafetyBarPressed()) {
        Serial.println("Barre de sécurité enclenchée");

        // Shutdown everything immediately
        StateManager::getInstance().safetyStop();

        displaySafetyStopWarning();
    }
}

// PRIVATE

void SafetyService::displaySafetyStopWarning() {
    DisplayService::getInstance().clearScreen();
    DisplayService::getInstance().printTitle("(!) ARRET SECURITE");
    DisplayService::getInstance().addBorder();
    DisplayService::getInstance().printCenter("Continuer [F1]", 2);

    bool canContinue = KeypadService::getInstance().waitForConfirmation();

    if (canContinue) {
        StateManager::getInstance().disengageSafetyMode();
    } else {
        StateManager::getInstance().changeState(MainMenu);
    }
}

bool SafetyService::isSafetyBarPressed() {
    MovingDirection movingDirection = StateManager::getInstance().getMovingDirection();

    return (movingDirection == MOVING_FORWARD && digitalRead(SAFETY_SENSOR_FRONT) == HIGH) 
        || (movingDirection == MOVING_BACKWARD && digitalRead(SAFETY_SENSOR_BACK) == HIGH);
}