#include "constants.h"
#include "conveyor_motor.h"
#include "display_service.h"
#include "keypad_service.h"
#include "log_service.h"
#include "rail_motor.h"
#include "state_manager.h"
#include "string.h"
#include "safety_service.h"

SafetyService & SafetyService::getInstance() {
    static SafetyService instance; // Guaranteed to be destroyed.
                                    // Instantiated on first use.
    return instance;
}

void SafetyService::listenForTrigger() {
    attachInterrupt(digitalPinToInterrupt(3), SafetyService::safetyBarInterrupt, FALLING);
}

void SafetyService::stopListeningForTrigger() {
    detachInterrupt(digitalPinToInterrupt(3));
}

void SafetyService::displaySafetyStopWarning() {
    const static char title[] PROGMEM = "(!) ARRET SECURITE";
    const static char continueBtn[] PROGMEM = "Continuer [#]";

    DisplayService::getInstance().clearScreen();
    DisplayService::getInstance().printTitle(getString(title));
    DisplayService::getInstance().addBorder();
    DisplayService::getInstance().printCenter(getString(continueBtn), 2);
}

void SafetyService::displayAutoDisengagement() {
    const static char title[] PROGMEM = "(!) ARRET SECURITE";
    const static char message[] PROGMEM = "Desactivation auto";

    DisplayService::getInstance().clearScreen();
    DisplayService::getInstance().printTitle(getString(title));
    DisplayService::getInstance().addBorder();
    DisplayService::getInstance().printCenter(getString(message), 2);
}

void SafetyService::displayMovingWarning() {
    const static char title[] PROGMEM = "(!) ATTENTION";
    const static char message1[] PROGMEM = "Le robot va se";
    const static char message2[] PROGMEM = "mettre en mouvement";

    DisplayService::getInstance().clearScreen();
    DisplayService::getInstance().printTitle(getString(title));
    DisplayService::getInstance().addBorder();
    DisplayService::getInstance().printCenter(getString(message1), 2);
    DisplayService::getInstance().printCenter(getString(message2), 3);

    tone(SPEAKER, 500, 2000);
    KeypadService::getInstance().waitForActivity(2000);
    pinMode(SPEAKER, INPUT);
}

void SafetyService::safetyBarInterrupt() {
    bool safetyMode = StateManager::getInstance().isSafetyMode();

    if (!safetyMode) {
        // Serial.println(F("Barre de sécurité enclenchée"));
        LogService::getInstance().log(SAFETY_BAR_PRESSED);
        
        // Shutdown everything immediately
        RailMotor::getInstance().stop();
        FrontConveyor::getInstance().stop();
        BackConveyor::getInstance().stop();

        StateManager::getInstance().safetyStop();
    }
}