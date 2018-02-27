/*  Soil Mositure Basic Example
    This sketch was written by SparkFun Electronics
    Joel Bartlett 
    August 31, 2015

    Basic skecth to print out soil moisture values to the Serial Monitor 

    Released under the MIT License(http://opensource.org/licenses/MIT)
*/

#include <Arduino.h>
#include <ArduinoSTL.h>

#include "feeder.h"
#include "meal.h"
#include "rail_point.h"
#include "rail_motor.h"
#include "trip.h"

//#include "conveyor_motor.h"

// INPUTS
const int POWER_BUTTON = A0;

const int RFID_READER = A3;
const int SAFETY_SENSOR_FRONT = A5;
const int SAFETY_SENSOR_BACK = A6;

const int INPUTS[4] = {
    POWER_BUTTON,
    RFID_READER,
    SAFETY_SENSOR_FRONT,
    SAFETY_SENSOR_BACK,
};
const int INPUTS_COUNT = 4;

// OUTPUTS
const int GREEN_LIGHT = A7;
const int RED_LIGHT = A8;

const int MAIN_MOTOR_OUT1 = A9;
const int MAIN_MOTOR_OUT2 = A10;

const RailMotor mainMotor = RailMotor(
    MAIN_MOTOR_OUT1,
    MAIN_MOTOR_OUT2
);

const int CONVEYOR_MOTOR_FRONT_PWM = A11;
const int CONVEYOR_MOTOR_FRONT_REVERSE = A12;
const int CONVEYOR_MOTOR_BACK_PWM = A11;
const int CONVEYOR_MOTOR_BACK_REVERSE = A12;

const ConveyorMotor conveyorFront = ConveyorMotor(
    CONVEYOR_MOTOR_FRONT_PWM,
    CONVEYOR_MOTOR_FRONT_REVERSE
);
const ConveyorMotor conveyorBack = ConveyorMotor(
    CONVEYOR_MOTOR_BACK_PWM,
    CONVEYOR_MOTOR_BACK_REVERSE
);

Feeder feeder = Feeder(mainMotor, conveyorFront, conveyorBack, GREEN_LIGHT, RED_LIGHT);

const int OUTPUTS[0] = {
    // GREEN_LIGHT,
    // RED_LIGHT,
    // MAIN_MOTOR_OUT1,
    // MAIN_MOTOR_OUT2,
    // CONVEYOR_MOTOR_FRONT_PWM,
    // CONVEYOR_MOTOR_FRONT_REVERSE,
    // CONVEYOR_MOTOR_BACK_PWM,
    // CONVEYOR_MOTOR_BACK_REVERSE,
};
const int OUTPUTS_COUNT = 0;

const int CONVEYORS[4] = {
    CONVEYOR_MOTOR_FRONT_PWM,
    CONVEYOR_MOTOR_FRONT_REVERSE,
    CONVEYOR_MOTOR_BACK_PWM,
    CONVEYOR_MOTOR_BACK_REVERSE,
};

//// POINTS
RailPoint dockPoint(1, "DOCK");
RailPoint groupe1Debut(2, "Groupe1/Debut");
RailPoint groupe1Fin(3, "Groupe1/Fin");

RailPoint& currentRailPoint = dockPoint;

//// TRIPS
Trip trajet1(MOVING_FORWARD, dockPoint, dockPoint);

// //// MEALS
MealSequence sequence1("G1", groupe1Debut, groupe1Fin, 500, 600);

std::vector<MealSequence> repasMatin1Sequences = {
    sequence1
};

Meal repasMatin1(7, 0, trajet1, repasMatin1Sequences, 1);

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

    checkSafetyState();

    // Check current rail point
    //currentRailPoint = getCurrentRailPoint();

    distributeMeal(repasMatin1);

    // Stop here if emergency button or safety bar is pressed
    if (feeder.state == STATE_SAFETY_STOP) {
        delay(1000);
        return;
    }

    Serial.print("Start loop");
    
    //get soil moisture value from the function below and print it
    //Serial.println(readSoil());

    //This 1 second timefrme is used so you can test the sensor and see it change in real-time.
    delay(1000);
}

bool isPowerON() {
    return digitalRead(POWER_BUTTON) == LOW;
}

// bool isEmergencyPressed() {
//     return digitalRead(EMERGENCY_BUTTON) == LOW;
// }

bool isSafetyBarPressed() {
    return (feeder.getMovingDirection() == MOVING_FORWARD && digitalRead(SAFETY_SENSOR_FRONT) == LOW) 
        || (feeder.getMovingDirection() == MOVING_BACKWARD && digitalRead(SAFETY_SENSOR_BACK) == LOW);
}

void checkSafetyState() {
    if (feeder.state != STATE_SAFETY_STOP && isSafetyBarPressed()) {
        // Shutdown everything immediately
        feeder.safetyStop();
    } else if (feeder.state == STATE_SAFETY_STOP && !isSafetyBarPressed()) {
        // Note: we should wait for manual reactivation or a timeout before reactivating automatically
        // TODO: Reactivate robot
    }
}

RailPoint& getCurrentRailPoint() {
    // Read data from RFID reader
    return groupe1Debut;
}

void distributeMeal(Meal meal) {
    // Move feeder in corresponding direction
    if (feeder.state == STATE_IDLE) {
        if (meal.trip.initialDirection == MOVING_FORWARD) {
            feeder.moveForward();
        } else {
            feeder.moveBackward();
        }
    }

    const MealSequence* sequencePtr = meal.getMealSequenceAt(currentRailPoint);
    if (sequencePtr) {
        MealSequence sequence = MealSequence(* sequencePtr);
    } else {
        // Make sure all feed conveyors are stopped
        feeder.conveyorFront.stop();
        feeder.conveyorBack.stop();
    }
}
