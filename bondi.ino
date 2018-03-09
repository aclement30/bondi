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
#include "conveyor_motor.h"
#include "diagnostic/route_mapping.h"
#include "feeder.h"
#include "location_service.h"
#include "meal.h"
#include "meal_service.h"
#include "rail_point.h"
#include "rail_motor.h"
#include "rfid_reader.h"
#include "route.h"
#include "state_manager.h"

// INPUTS
const int INPUTS[2] = {
    POWER_BUTTON,
    RFID_READER
    // SAFETY_SENSOR_FRONT,
    // SAFETY_SENSOR_BACK,
};
const int INPUTS_COUNT = 2;

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

Config config = loadStaticConfiguration();

RfidReader rfidReader = RfidReader(RFID_RSA_PIN, RFID_RST_PIN);

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

MealService mealService = MealService(config.meals, feeder);

DiagnosticService *diagnosticPtr = NULL;

StateManager stateManager = StateManager();

bool isPowerON() {
    return digitalRead(POWER_BUTTON) == LOW;
}

void setup() {
    Serial.begin(9600);   // open serial over USB
    Serial.println("Configuration initiale...");
    
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

    Serial.println("Configuration initiale terminée");

    stateManager.changeState(Idle);

    // TEMPORARY
    stateManager.changeState(Diagnostic);
}

void loop() {
    // Stop right here if power is OFF
    // if (!isPowerON()) {
    //     delay(1000);
    //     return;
    // }

    feeder.checkSafetyState();

    // Stop here if emergency button or safety bar is pressed
    if (feeder.state == STATE_SAFETY_STOP) {
        delay(1000);
        return;
    }

    locationService.refreshActivePoint();

    MachineState currentState = stateManager.getState();
    switch(currentState) {
        case Automatic:
            mealService.refreshCurrentMeal();

            if (mealService.hasCurrentMeal()) {
                Meal currentMeal = mealService.getCurrentMeal();
                mealService.distributeMeal(currentMeal);

                char message[] = "Distribution du repas: ";
                Serial.println(strcat(message, currentMeal.name));
            } else {
                Serial.println("Attente...");
                delay(1000);
            }
            break;
        case Manual:
            delay(1000);
            break;
        case Diagnostic:
            if (diagnosticPtr == NULL) {
                RouteMappingDiagnosticService routeMappingDiagnostic = RouteMappingDiagnosticService(feeder, locationService.routes);
                diagnosticPtr = &routeMappingDiagnostic;

                Serial.println("Début du diagnostic: Routes");
                routeMappingDiagnostic.startDiagnostic();
                delay(1000);
            } else {
                if (diagnosticPtr->isCompleted()) {
                    Serial.println("Fin du diagnostic");
                    diagnosticPtr = NULL;
                    stateManager.changeState(Manual);
                } else {
                    diagnosticPtr->continueDiagnostic();
                    delay(1000);
                }
            }
        default:
            break;
    }
}

#ifdef __EMSCRIPTEN__

#include "simulator/simulator.cpp"

int main() {
    emscripten_set_main_loop(loop, 60, 1);
}

#endif
