/*  Soil Mositure Basic Example
    This sketch was written by SparkFun Electronics
    Joel Bartlett 
    August 31, 2015

    Basic skecth to print out soil moisture values to the Serial Monitor 

    Released under the MIT License(http://opensource.org/licenses/MIT)
*/

#ifdef __EMSCRIPTEN__
#include "simulator/simulator.h"
#else
#include <SD.h>
#include <ArduinoSTL.h>
#endif

#include "config.h"
#include "constants.h"
#include "state_manager.h"
#include "controllers/controller.h"
#include "controllers/automatic_controller.h"
#include "controllers/diagnostic_controller.h"
#include "controllers/main_menu_controller.h"
#include "controllers/off_controller.h"
#include "conveyor_motor.h"
// #include "diagnostic/route_mapping.h"
// #include "feeder.h"
#include "location_service.h"
// #include "meal.h"
// #include "meal_service.h"
// #include "rail_point.h"
#include "rail_motor.h"
// #include "route.h"
#include "safety_service.h"

// MOTORS

RailMotor mainMotor = RailMotor();

const ConveyorMotor conveyorFront = ConveyorMotor(
    CONVEYOR_MOTOR_FRONT_PWM,
    CONVEYOR_MOTOR_FRONT_REVERSE
);
const ConveyorMotor conveyorBack = ConveyorMotor(
    CONVEYOR_MOTOR_BACK_PWM,
    CONVEYOR_MOTOR_BACK_REVERSE
);

// Load config
Config config = loadStaticConfiguration();

LocationService locationService = LocationService(config.railPoints, config.routes);
SafetyService safetyService = SafetyService();

// Feeder feeder = Feeder(
//     mainMotor,
//     conveyorFront, 
//     conveyorBack
// );

// MealService mealService = MealService(config.meals, feeder);

Controller *currentControllerPtr = NULL;
MachineState previousState = Off;

bool isPowerON() {
    return digitalRead(POWER_BUTTON) == LOW;
}

void createController(MachineState currentState) {
    if (currentControllerPtr != NULL) {
        if (previousState == currentState) {
          Serial.println(" (use existing controller)");
          return;
        }

        delete currentControllerPtr;
        currentControllerPtr = NULL;
    }

    previousState = currentState;

    switch(currentState) {
        case Off: {
            currentControllerPtr = new OffController();
            break;
        }
        case MainMenu: {
            currentControllerPtr = new MainMenuController();
            Serial.println(" (new MainMenuController)");
            break;
        }
        case Automatic: {
            currentControllerPtr = new AutomaticController();
            Serial.println(" (new AutomaticController)");
            break;
        }
        case Manual: {
            break;
        }
        case Diagnostic: {
            currentControllerPtr = new DiagnosticController(locationService);
            Serial.println(" (new DiagnosticController)");
            break;
        }
    }
}

void displayStartupScreen() {
    DisplayService::getInstance().clearScreen();
    DisplayService::getInstance().printCenter("Demarrage en cours", 3);
}

void setup() {
    Serial.begin(9600);   // open serial over USB
    Serial.println("Démarrage en cours");
    
    displayStartupScreen();

    for (int n = 0; n < INPUTS_COUNT; n++) {
        pinMode(INPUTS[n], INPUT);
    }

    #ifndef __EMSCRIPTEN__

    //if (!SD.begin(SD_CARD_CS)) {
    //    Serial.println("Card failed, or not present");
    //    // don't do anything more:
    //    while (1);
    //}

    #endif

    // feeder.setup();

    Serial.println("Configuration initiale");

    StateManager::getInstance().changeState(MainMenu);
}

void loop() {
    delay(250);
    
    Serial.println("* loop");
    
    // Stop right here if power is OFF
    if (!isPowerON()) {
        delay(1000);
        return;
    }
    
    // Serial.println("* check safety");
    // safetyService.checkSafetyState();
    
    Serial.println("* refresh point");
    locationService.refreshActivePoint();
    
    // Serial.println("* main motor");
    // mainMotor.loop();
    
    // Serial.print("* create controller");
    createController(StateManager::getInstance().getState());
    
    // Serial.println("* handle");

    // delay(250);

    currentControllerPtr->handle();
}

#ifdef __EMSCRIPTEN__

#include "simulator/simulator.cpp"

int main() {
    emscripten_set_main_loop(loop, 1, 1);
}

#endif
