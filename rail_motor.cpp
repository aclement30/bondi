#include "constants.h"
#include "state_manager.h"
#include "rail_motor.h"

RailMotor & RailMotor::getInstance() {
    static RailMotor instance;  // Guaranteed to be destroyed.
                                // Instantiated on first use.
    return instance;
}

// void RailMotor::loop() {
//     MovingDirection newDirection = StateManager::getInstance().getMovingDirection();
//     if (currentDirection == newDirection) return;

//     // First stop the motor
//     if (currentDirection != MOVING_IDLE) {
//         Serial.println(F("Moteur principal: arrêt"));
//         stop();
//     }

//     // Then, change the motor direction
//     if (newDirection == MOVING_FORWARD) {
//         Serial.println(F("Moteur principal: rotation avant"));
//         moveForward();
//     } else if (newDirection == MOVING_BACKWARD) {
//         Serial.println(F("Moteur principal: rotation arrière"));
//         moveBackward();
//     }

//     currentDirection = newDirection;
// }

void RailMotor::moveForward() {
    if (StateManager::getInstance().getMovingDirection() == MOVING_FORWARD) {
        return;
    }

    digitalWrite(MAIN_MOTOR_OUT1, HIGH);
    digitalWrite(MAIN_MOTOR_OUT2, LOW);

    Serial.println(F("Moteur principal: rotation avant"));
    setLight(GREEN, false);
    StateManager::getInstance().changeMovingDirection(MOVING_FORWARD);
}

void RailMotor::moveBackward() {
    if (StateManager::getInstance().getMovingDirection() == MOVING_BACKWARD) {
        return;
    }

    digitalWrite(MAIN_MOTOR_OUT1, LOW);
    digitalWrite(MAIN_MOTOR_OUT2, HIGH);

    Serial.println(F("Moteur principal: rotation arrière"));
    setLight(GREEN_BACKWARD, false);
    StateManager::getInstance().changeMovingDirection(MOVING_BACKWARD);
}

void RailMotor::stop() {
    if (StateManager::getInstance().getMovingDirection() == MOVING_IDLE) {
        return;
    }

    digitalWrite(MAIN_MOTOR_OUT1, HIGH);
    digitalWrite(MAIN_MOTOR_OUT2, HIGH);

    Serial.println(F("Moteur principal: arrêt"));
    setLight(RED, false);
    StateManager::getInstance().stop();
}

void RailMotor::inverseMovingDirection() {
    MovingDirection previousState = StateManager::getInstance().getMovingDirection();
    
    stop();

    if (previousState == MOVING_FORWARD) {
        moveBackward();
    } else if (previousState == MOVING_BACKWARD) {
        moveForward();
    }
}

// PRIVATE

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
}

void RailMotor::setLight(LightColor lightColor, bool blinking) {
    digitalWrite(GREEN_LIGHT, LOW);
    digitalWrite(RED_LIGHT, LOW);
    digitalWrite(GREEN_BACKWARD_LIGHT, LOW);

    if (lightColor == GREEN) {
        digitalWrite(GREEN_LIGHT, HIGH);
    } else if (lightColor == GREEN_BACKWARD) {
        digitalWrite(GREEN_BACKWARD_LIGHT, HIGH);
    } else {
        digitalWrite(RED_LIGHT, HIGH);
    }
}
