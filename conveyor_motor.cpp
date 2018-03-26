#include <Arduino.h>
#include "constants.h"
#include "conveyor_motor.h"

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
    digitalWrite(pwm, LOW);
    digitalWrite(reverse, LOW);

    Serial.println(F("Configuration du convoyeur"));
}

void ConveyorMotor::start(ConveyorSide side, int motorSpeed) {
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