/*  Soil Mositure Basic Example
    This sketch was written by SparkFun Electronics
    Joel Bartlett 
    August 31, 2015

    Basic skecth to print out soil moisture values to the Serial Monitor 

    Released under the MIT License(http://opensource.org/licenses/MIT)
*/

//#include <Arduino.h>
//#include <ArduinoSTL.h>

#ifdef __EMSCRIPTEN__
#include "simulator.h"
#endif

#include "config.h"
#include "conveyor_motor.h"
#include "feeder.h"
#include "meal.h"
#include "rail_point.h"
#include "rail_motor.h"
#include "route.h"

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

Feeder feeder = Feeder(
    mainMotor, 
    conveyorFront, 
    conveyorBack, 
    GREEN_LIGHT, 
    RED_LIGHT,
    SAFETY_SENSOR_FRONT,
    SAFETY_SENSOR_BACK
);

//// POINTS
RailPoint dockPoint(1, "DOCK");
RailPoint groupe1Debut(2, "Groupe1/Debut");
RailPoint groupe1Fin(3, "Groupe1/Fin");

RailPoint& currentRailPoint = dockPoint;

//// ROUTES
Route trajet1(MOVING_FORWARD, dockPoint, dockPoint);

// //// MEALS
MealSequence sequence1("G1", groupe1Debut, groupe1Fin, 500, 600);

std::vector<MealSequence> repasMatin1Sequences = {
    sequence1
};

Meal repasMatin1("Repas matin - GE", 7, 0, trajet1, repasMatin1Sequences, 1);

bool isPowerON() {
    return digitalRead(POWER_BUTTON) == LOW;
}

RailPoint& getCurrentRailPoint() {
    // Read data from RFID reader
    return groupe1Debut;
}

void distributeMeal(Meal meal) {
    // Move feeder in corresponding direction
    if (feeder.state == STATE_IDLE) {
        if (meal.route.initialDirection == MOVING_FORWARD) {
            feeder.moveForward();
        } else {
            feeder.moveBackward();
        }
    }

    const MealSequence* sequencePtr = meal.getMealSequenceAt(currentRailPoint);
    if (sequencePtr) {
        MealSequence sequence = MealSequence(* sequencePtr);

        int feedingSide = ((feeder.getMovingDirection() == MOVING_FORWARD) ? CONVEYOR_SIDE_RIGHT : CONVEYOR_SIDE_LEFT);
        if (sequence.feed1Flow > 0) {
            feeder.conveyorFront.start(feedingSide, sequence.feed1Flow);
        }
        if (sequence.feed2Flow > 0) {
            feeder.conveyorBack.start(feedingSide, sequence.feed2Flow);
        }
    } else {
        // Make sure all feed conveyors are stopped
        feeder.stopFeeding();
    }
}

void setup() {
    Serial.begin(9600);   // open serial over USB

    for (int n = 0; n < INPUTS_COUNT; n++) {
        pinMode(INPUTS[n], INPUT);
    }

    Serial.print("Setup completed");
}

void loop() {
    // Stop right here if power is OFF
    if (!isPowerON()) {
        delay(1000);
        return;
    }

    feeder.checkSafetyState();

    // Stop here if emergency button or safety bar is pressed
    if (feeder.state == STATE_SAFETY_STOP) {
        delay(1000);
        return;
    }

    // Check current rail point
    //currentRailPoint = getCurrentRailPoint();

    distributeMeal(repasMatin1);

    feeder.checkMovingDirectionState();

    //Serial.print("Start loop");
    
    //get soil moisture value from the function below and print it
    //Serial.println(readSoil());

    //This 1 second timefrme is used so you can test the sensor and see it change in real-time.
    delay(1000);
}

#ifdef __EMSCRIPTEN__

#include "simulator.cpp"

int main() {
    emscripten_set_main_loop(loop, 60, 1);
}

#endif