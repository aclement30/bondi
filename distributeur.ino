/*  Soil Mositure Basic Example
    This sketch was written by SparkFun Electronics
    Joel Bartlett 
    August 31, 2015

    Basic skecth to print out soil moisture values to the Serial Monitor 

    Released under the MIT License(http://opensource.org/licenses/MIT)
*/

#include "meal.h"
#include "rail_point.h"
#include "trip.h"

// STATES
const int STATE_IDLE = 0;
const int STATE_MOVING = 1;
const int STATE_REFILLING = 2;
const int STATE_SAFETY_STOP = 3;
const int STATE_EMERGENCY_STOP = 4;

int currentState = STATE_IDLE;
int previousState = STATE_IDLE;

// MOVING DIRECTIONS
const int MOVING_FORWARD = 0;
const int MOVING_BACKWARD = 1;

int movingDirection = MOVING_FORWARD;

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

const int CONVEYOR_MOTOR_FRONT_PWM = A11;
const int CONVEYOR_MOTOR_FRONT_REVERSE = A12;

const int CONVEYOR_MOTOR_BACK_PWM = A11;
const int CONVEYOR_MOTOR_BACK_REVERSE = A12;

const int OUTPUTS[8] = {
    GREEN_LIGHT,
    RED_LIGHT,
    MAIN_MOTOR_OUT1,
    MAIN_MOTOR_OUT2,
    CONVEYOR_MOTOR_FRONT_PWM,
    CONVEYOR_MOTOR_FRONT_REVERSE,
    CONVEYOR_MOTOR_BACK_PWM,
    CONVEYOR_MOTOR_BACK_REVERSE,
};
const int OUTPUTS_COUNT = 8;

const int CONVEYORS[4] = {
    CONVEYOR_MOTOR_FRONT_PWM,
    CONVEYOR_MOTOR_FRONT_REVERSE,
    CONVEYOR_MOTOR_BACK_PWM,
    CONVEYOR_MOTOR_BACK_REVERSE,
};

int conveyorFrontSpeed = 0;
int conveyorBackSpeed = 0;

//// POINTS
RailPoint dockPoint(1, "DOCK");
RailPoint groupe1Debut(2, "Groupe1/Debut");
RailPoint groupe1Fin(3, "Groupe1/Fin");

RailPoint currentRailPoint = dockPoint;

//// TRIPS
Trip trajet1(MOVING_FORWARD, &dockPoint, &dockPoint);

// //// MEALS
MealSequence sequence1("G1", &groupe1Debut, &groupe1Fin, 500, 600);

MealSequence *repasMatin1Sequences[] = {
    &sequence1
};

Meal repasMatin1(7, 0, &trajet1, repasMatin1Sequences, 1);

void setup() {
    Serial.begin(9600);   // open serial over USB

    for (int n = 0; n < INPUTS_COUNT; n++) {
        pinMode(INPUTS[n], INPUT);
    }

    for (int n = 0; n < OUTPUTS_COUNT; n++) {
        pinMode(OUTPUTS[n], OUTPUT);
        // Set to LOW so no power is flowing through the output
        digitalWrite(OUTPUTS[n], LOW);
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
    currentRailPoint = getCurrentRailPoint();

    distributeMeal(repasMatin1);

    // Stop here if emergency button or safety bar is pressed
    if (currentState == STATE_EMERGENCY_STOP || currentState == STATE_SAFETY_STOP) {
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
    return (movingDirection == MOVING_FORWARD && digitalRead(SAFETY_SENSOR_FRONT) == LOW) 
        || (movingDirection == MOVING_BACKWARD && digitalRead(SAFETY_SENSOR_BACK) == LOW);
}

void emergencyStop() {
    stopMoving();

    // Shutdown all motors
    for (int n = 0; n < 4; n++) {
        digitalWrite(CONVEYORS[n], LOW);
    }

    // Turn off green light
    digitalWrite(GREEN_LIGHT, LOW);

    // Turn on red light
    digitalWrite(RED_LIGHT, HIGH);
}

void moveForward() {
    digitalWrite(MAIN_MOTOR_OUT1, HIGH);
    digitalWrite(MAIN_MOTOR_OUT2, LOW);

    movingDirection = MOVING_FORWARD;
    currentState = STATE_MOVING;
}

void moveBackward() {
    digitalWrite(MAIN_MOTOR_OUT1, LOW);
    digitalWrite(MAIN_MOTOR_OUT2, HIGH);
    
    movingDirection = MOVING_BACKWARD;
    currentState = STATE_MOVING;
}

void stopMoving() {
    digitalWrite(MAIN_MOTOR_OUT1, HIGH);
    digitalWrite(MAIN_MOTOR_OUT2, HIGH);

    currentState = STATE_IDLE;
}

void inverseMovingDirection() {
    stopMoving();

    if (movingDirection == MOVING_FORWARD) {
        moveBackward();
    } else {
        moveForward();
    }
}

void checkSafetyState() {
    if (currentState != STATE_SAFETY_STOP && isSafetyBarPressed()) {
        currentState = STATE_SAFETY_STOP;

        // Shutdown everything immediately
        emergencyStop();
    } else if (currentState == STATE_SAFETY_STOP && !isSafetyBarPressed()) {
        // Note: we should wait for manual reactivation or a timeout before reactivating automatically
        // TODO: Reactivate robot
    }
}

RailPoint getCurrentRailPoint() {
    // Read data from RFID reader
    return groupe1Debut;
}

MealSequence getCurrentMealSequence(Meal meal) {
    for(int n = 0; n < meal.sequencesCount; n++) {
        MealSequence *sequence = meal.sequences[n];
        if (sequence->startPoint->id == currentRailPoint.id) {
            return sequence;
        }
    }
}

void distributeMeal(Meal meal) {
    // Move feeder in corresponding direction
    if (currentState == STATE_IDLE) {
        if (meal.trip->initialDirection == MOVING_FORWARD) {
            moveForward();
        } else {
            moveBackward();
        }
    }

    MealSequence currentSequence = getCurrentMealSequence(meal);
}
