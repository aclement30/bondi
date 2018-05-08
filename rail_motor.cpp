#include "constants.h"
#include "display_service.h"
#include "keypad_service.h"
#include "state_manager.h"
#include "string.h"
#include "rail_motor.h"

RailMotor & RailMotor::getInstance() {
    static RailMotor instance;  // Guaranteed to be destroyed.
                                // Instantiated on first use.
    return instance;
}

void RailMotor::moveForward() {
    if (StateManager::getInstance().getMovingDirection() == MOVING_FORWARD || StateManager::getInstance().isSafetyMode()) {
        return;
    }

    digitalWrite(MAIN_MOTOR_OUT1, HIGH);
    digitalWrite(MAIN_MOTOR_OUT2, LOW);

    Serial.println(F("Moteur principal: rotation avant"));
    setLight(GREEN, false);
    StateManager::getInstance().changeMovingDirection(MOVING_FORWARD);
}

void RailMotor::moveBackward() {
    if (StateManager::getInstance().getMovingDirection() == MOVING_BACKWARD || StateManager::getInstance().isSafetyMode()) {
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
    
    Serial.println(F("Moteur principal: inversion direction"));

    stop();

    delay(1000);

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

    pinMode(GREEN_FORWARD_LIGHT, OUTPUT);
    pinMode(RED_LIGHT, OUTPUT);
    pinMode(GREEN_BACKWARD_LIGHT, OUTPUT);

    // Set to LOW so no power is flowing through the output
    digitalWrite(MAIN_MOTOR_OUT1, LOW);
    digitalWrite(MAIN_MOTOR_OUT2, LOW);

    digitalWrite(GREEN_FORWARD_LIGHT, LOW);
    digitalWrite(RED_LIGHT, LOW);
    digitalWrite(GREEN_BACKWARD_LIGHT, LOW);
}

void RailMotor::setLight(LightColor lightColor, bool blinking) {
    digitalWrite(GREEN_FORWARD_LIGHT, LOW);
    digitalWrite(RED_LIGHT, LOW);
    digitalWrite(GREEN_BACKWARD_LIGHT, LOW);

    if (lightColor == GREEN) {
        digitalWrite(GREEN_FORWARD_LIGHT, HIGH);
    } else if (lightColor == GREEN_BACKWARD) {
        digitalWrite(GREEN_BACKWARD_LIGHT, HIGH);
    } else {
        digitalWrite(RED_LIGHT, HIGH);
    }
}
