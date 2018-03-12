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
#include "controllers/controller.h"
#include "conveyor_motor.h"
// #include "diagnostic/route_mapping.h"
#include "feeder.h"
#include "location_service.h"
// #include "meal.h"
// #include "meal_service.h"
// #include "rail_point.h"
#include "rail_motor.h"
#include "rfid_reader.h"
// #include "route.h"
#include "state_manager.h"

// MOTORS

const RailMotor mainMotor = RailMotor(
    MAIN_MOTOR_OUT1,
    MAIN_MOTOR_OUT2
);

const ConveyorMotor conveyorFront = ConveyorMotor(
    CONVEYOR_MOTOR_FRONT_PWM,
    CONVEYOR_MOTOR_FRONT_REVERSE
);
const ConveyorMotor conveyorBack = ConveyorMotor(
    CONVEYOR_MOTOR_BACK_PWM,
    CONVEYOR_MOTOR_BACK_REVERSE
);

// RFID
RfidReader rfidReader = RfidReader(RFID_RSA_PIN, RFID_RST_PIN);

// Load config
Config config = loadStaticConfiguration();

LocationService locationService = LocationService(rfidReader, config.railPoints, config.routes);

Feeder feeder = Feeder(
    mainMotor,
    conveyorFront, 
    conveyorBack, 
    GREEN_LIGHT, 
    RED_LIGHT,
    SAFETY_SENSOR_FRONT,
    SAFETY_SENSOR_BACK,
    locationService
);

// MealService mealService = MealService(config.meals, feeder);

Controller *currentControllerPtr = NULL;

bool isPowerON() {
    return digitalRead(POWER_BUTTON) == LOW;
}

void setup() {
    Serial.begin(9600);   // open serial over USB
    Serial.println("Démarrage en cours");
    
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

    feeder.setup();
    rfidReader.setup();

    Serial.println("Configuration init.");

    StateManager::getInstance().changeState(MainMenu);
}

void loop() {
    // Stop right here if power is OFF
    // if (!isPowerON()) {
    //     delay(1000);
    //     return;
    // }

    feeder.checkSafetyState();

    // Stop here if emergency button or safety bar is pressed
    if (feeder.state == FEEDER_SAFETY_STOP) {
        delay(1000);
        return;
    }

    locationService.refreshActivePoint();

    Controller &currentController = StateManager::getInstance().getController();
    currentController.handle();
}

#ifdef __EMSCRIPTEN__

#include "simulator/simulator.cpp"

int main() {
    emscripten_set_main_loop(loop, 60, 1);
}

#endif
