#include "constants.h"
#include "state_manager.h"
#include "rail_motor.h"

RailMotor::RailMotor() {
    pinMode(MAIN_MOTOR_OUT1, OUTPUT);
    pinMode(MAIN_MOTOR_OUT2, OUTPUT);

    pinMode(GREEN_LIGHT, OUTPUT);
    pinMode(RED_LIGHT, OUTPUT);
    pinMode(GREEN_BACKWARD_LIGHT, OUTPUT);

    // Set to LOW so no power is flowing through the output
    digitalWrite(MAIN_MOTOR_OUT1, LOW);
    digitalWrite(MAIN_MOTOR_OUT2, LOW);

    digitalWrite(GREEN_LIGHT, LOW);
    digitalWrite(RED_LIGHT, LOW);
    digitalWrite(GREEN_BACKWARD_LIGHT, LOW);

    currentDirection = MOVING_IDLE;
}

void RailMotor::loop() {
    MovingDirection newDirection = StateManager::getInstance().getMovingDirection();
    if (currentDirection == newDirection) return;

    // First stop the motor
    if (currentDirection != MOVING_IDLE) {
        Serial.println(F("Moteur principal: arrêt"));
        stop();
    }

    // Then, change the motor direction
    if (newDirection == MOVING_FORWARD) {
        Serial.println(F("Moteur principal: rotation avant"));
        moveForward();
    } else if (newDirection == MOVING_BACKWARD) {
        Serial.println(F("Moteur principal: rotation arrière"));
        moveBackward();
    }

    currentDirection = newDirection;
}

void RailMotor::moveForward() {
    digitalWrite(MAIN_MOTOR_OUT1, HIGH);
    digitalWrite(MAIN_MOTOR_OUT2, LOW);

    setLight(GREEN, false);
}

void RailMotor::moveBackward() {
    digitalWrite(MAIN_MOTOR_OUT1, LOW);
    digitalWrite(MAIN_MOTOR_OUT2, HIGH);

    setLight(GREEN_BACKWARD, false);
}

void RailMotor::stop() {
    digitalWrite(MAIN_MOTOR_OUT1, HIGH);
    digitalWrite(MAIN_MOTOR_OUT2, HIGH);

    setLight(RED, false);
}

// void inverseMovingDirection() {
//     stop();

//     if (previousState == MOVING_FORWARD) {
//         moveBackward();
//     } else if (previousState == MOVING_BACKWARD) {
//         moveForward();
//     }
// }

// PRIVATE

void RailMotor::setLight(LightColor lightColor, bool blinking) {
    if (lightColor == GREEN) {
        digitalWrite(GREEN_LIGHT, HIGH);
        digitalWrite(RED_LIGHT, LOW);
        digitalWrite(GREEN_BACKWARD_LIGHT, LOW);

        Serial.println(F("Lumière VERTE allumée"));
    } else if (lightColor == GREEN_BACKWARD) {
        digitalWrite(GREEN_LIGHT, LOW);
        digitalWrite(RED_LIGHT, LOW);
        digitalWrite(GREEN_BACKWARD_LIGHT, HIGH);

        Serial.println(F("Lumière VERTE #2 allumée"));
    } else {
        digitalWrite(GREEN_LIGHT, LOW);
        digitalWrite(RED_LIGHT, HIGH);
        digitalWrite(GREEN_BACKWARD_LIGHT, LOW);

        Serial.println(F("Lumière ROUGE allumée"));
    }
}
