#include "constants.h"
#include "display_service.h"
#include "keypad_service.h"
#include "state_manager.h"
#include "string.h"
#include "safety_service.h"

SafetyService::SafetyService(
    MealService & mealServiceRef
) : 
    mealService(mealServiceRef)
{
    pinMode(SAFETY_SENSOR_FRONT, INPUT);
    pinMode(SAFETY_SENSOR_BACK, INPUT);
}

void SafetyService::checkSafetyState() {
    bool safetyMode = StateManager::getInstance().isSafetyMode();

    if (!safetyMode && isSafetyBarPressed()) {
        Serial.println(F("Barre de sécurité enclenchée"));

        // Shutdown everything immediately
        StateManager::getInstance().safetyStop();

        displaySafetyStopWarning();
    }
}

// PRIVATE

void SafetyService::displaySafetyStopWarning() {
    const static char title[] PROGMEM = "(!) ARRET SECURITE";
    const static char continueBtn[] PROGMEM = "Continuer [#]";

    DisplayService::getInstance().clearScreen();
    DisplayService::getInstance().printTitle(getString(title));
    DisplayService::getInstance().addBorder();
    DisplayService::getInstance().printCenter(getString(continueBtn), 2);

    bool canContinue = KeypadService::getInstance().waitForConfirmation();

    if (canContinue) {
        StateManager::getInstance().disengageSafetyMode();
    } else {
        if (mealService.hasCurrentMeal()) {
            mealService.abortDistribution();
        }

        StateManager::getInstance().changeState(MainMenu);
    }
}

bool SafetyService::isSafetyBarPressed() {
    MovingDirection movingDirection = StateManager::getInstance().getMovingDirection();

    return (movingDirection == MOVING_FORWARD && digitalRead(SAFETY_SENSOR_FRONT) == HIGH) 
        || (movingDirection == MOVING_BACKWARD && digitalRead(SAFETY_SENSOR_BACK) == HIGH);
}