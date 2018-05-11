#include <ArduinoSTL.h>
//#include <avr/wdt.h>

#include "constants.h"
#include "string.h"
#include "config.h"

// Motors
#include "conveyor_motor.h"
#include "rail_motor.h"

// Services
#include "log_service.h"
#include "state_manager.h"
#include "keypad_service.h"
#include "location_service.h"
#include "route_service.h"
#include "safety_service.h"

void displayStartupScreen() {
    const static char startupString[] PROGMEM = "Demarrage en cours";
    DisplayService::getInstance().clearScreen();
    DisplayService::getInstance().printCenter(getString(startupString), 3);
}

void setup() {
    Serial.begin(9600);   // open serial over USB
    Serial.println(F("Démarrage en cours"));

    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);

    LogService::getInstance().log(STARTUP);

    // Init SPI bus
    SPI.begin();

    // Setup RFID reader
    LocationService::getInstance().setup();

    //wdt_enable(WDTO_1S);
  
    displayStartupScreen();

    // Load config
    Config config = loadConfiguration();
    AppConfig::getInstance().railPoints = config.railPoints;
    AppConfig::getInstance().routes = config.routes;
    AppConfig::getInstance().meals = config.meals;
    AppConfig::getInstance().configVersion = config.version;

    Serial.println(F("Configuration initiale"));

    StateManager::getInstance().changeState(MainMenu);
    KeypadService::getInstance().listenForEscape();

    // Initialize rail motor at stop position
    RailMotor::getInstance().stop();

    // Initialize conveyors motor at stop position
    FrontConveyor::getInstance().stop();
    BackConveyor::getInstance().stop();
}

void loop() {
    //wdt_reset();
  
    // Serial.println(F("* loop"));

    if (StateManager::getInstance().isSafetyMode()) {
        StateManager::getInstance().getCurrentController()->safetyStop();
    }

    if (KeypadService::getInstance().isEscapeKeyPressed()) {
        // Stop motor
        RailMotor::getInstance().stop();

        StateManager::getInstance().getCurrentController()->escape();
        
        KeypadService::getInstance().resetEscapeKey();
    } else {
        StateManager::getInstance().getCurrentController()->handle();
    }
}
