#include <ArduinoSTL.h>
//#include <avr/wdt.h>

#include "constants.h"
#include "string.h"
#include "config.h"

// Motors
#include "conveyor_motor.h"
#include "rail_motor.h"

// Services
#include "state_manager.h"
#include "keypad_service.h"
#include "location_service.h"
#include "route_service.h"
#include "safety_service.h"

// Services
SafetyService safetyService = SafetyService();

bool isPowerON() {
    return digitalRead(POWER_BUTTON) == LOW;
}

void displayStartupScreen() {
    const static char startupString[] PROGMEM = "Demarrage en cours";
    DisplayService::getInstance().clearScreen();
    DisplayService::getInstance().printCenter(getString(startupString), 3);
}

void setup() {
    Serial.begin(9600);   // open serial over USB
    Serial.println(F("Démarrage en cours"));

    // Init SPI bus
    SPI.begin();

    //wdt_enable(WDTO_1S);
  
    displayStartupScreen();

    // Load config
    Config config = loadConfiguration();
    AppConfig::getInstance().railPoints = config.railPoints;
    AppConfig::getInstance().routes = config.routes;
    AppConfig::getInstance().meals = config.meals;

    for (int n = 0; n < INPUTS_COUNT; n++) {
        pinMode(INPUTS[n], INPUT);
    }

    LocationService::getInstance().setup();

    Serial.println(F("Configuration initiale"));

    StateManager::getInstance().changeState(MainMenu);
    KeypadService::getInstance().listenForEscape();
}

void loop() {
    //wdt_reset();
  
    Serial.println(F("* loop"));
    
    // Stop right here if power is OFF
    // if (!isPowerON()) {
    //     delay(1000);
    //     return;
    // }

    // Serial.println("* check safety");
    safetyService.checkSafetyState();
    
    // Serial.println("* main motor");
    // mainMotor.loop();
    
    Serial.println(F("* handle"));

    // delay(250);

    if (KeypadService::getInstance().isEscapeKeyPressed()) {
        // Stop motor
        RailMotor::getInstance().stop();

        StateManager::getInstance().getCurrentController()->escape();
        
        KeypadService::getInstance().resetEscapeKey();
    } else {
        StateManager::getInstance().getCurrentController()->handle();
    }
}
