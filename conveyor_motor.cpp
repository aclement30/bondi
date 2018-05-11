#include <Arduino.h>
#include "constants.h"
#include "conveyor_motor.h"
#include "state_manager.h"

ConveyorMotor::ConveyorMotor(PinConfig motorPwm, PinConfig motorReverse) {
    pwm = motorPwm;
    reverse = motorReverse;
    feedingSide = CONVEYOR_IDLE;
    speed = 0;
}

void ConveyorMotor::setup() {
    pinMode(pwm, OUTPUT);
    pinMode(reverse, OUTPUT);
    // Set to LOW so no power is flowing through the output
    analogWrite(pwm, 0);
    digitalWrite(reverse, LOW);
}

void ConveyorMotor::start(ConveyorSide side, int motorSpeed) {
    if (StateManager::getInstance().isSafetyMode()) {
        return;
    }
    
    if (side == CONVEYOR_SIDE_LEFT) {
        digitalWrite(reverse, HIGH);
    } else {
        digitalWrite(reverse, LOW);
    }

    if (motorSpeed > 100) {
        motorSpeed = 100;
    }

    analogWrite(pwm, map(motorSpeed, 0, 100, 0, 255));

    feedingSide = side;
    speed = motorSpeed;
}

void ConveyorMotor::stop() {
    analogWrite(pwm, 0);
    digitalWrite(reverse, LOW);

    feedingSide = CONVEYOR_IDLE;
    speed = 0;
}

int ConveyorMotor::getSpeed() {
    return speed;
}

// FRONT CONVEYOR

FrontConveyor & FrontConveyor::getInstance() {
    static FrontConveyor instance; // Guaranteed to be destroyed.
                                    // Instantiated on first use.
    return instance;
}

FrontConveyor::FrontConveyor() : ConveyorMotor(CONVEYOR_MOTOR_FRONT_PWM, CONVEYOR_MOTOR_FRONT_REVERSE) {}

// BACK CONVEYOR

BackConveyor & BackConveyor::getInstance() {
    static BackConveyor instance;   // Guaranteed to be destroyed.
                                    // Instantiated on first use.
    return instance;
}

BackConveyor::BackConveyor() : ConveyorMotor(CONVEYOR_MOTOR_BACK_PWM, CONVEYOR_MOTOR_BACK_REVERSE) {}