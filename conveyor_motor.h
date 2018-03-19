#include "constants.h"

#ifndef CONVEYORMOTOR_H
#define CONVEYORMOTOR_H

class ConveyorMotor {
    public:
        PinConfig pwm;
        PinConfig reverse;

        ConveyorSide feedingSide = CONVEYOR_IDLE;
        int speed = 0;

        ConveyorMotor(
            PinConfig motorPwm, 
            PinConfig motorReverse
        ) : 
            pwm(motorPwm),
            reverse(motorReverse)
        {}

        void setup() {
            pinMode(pwm, OUTPUT);
            pinMode(reverse, OUTPUT);
            // Set to LOW so no power is flowing through the output
            digitalWrite(pwm, LOW);
            digitalWrite(reverse, LOW);

            Serial.println("Configuration du convoyeur");
        }

        void start(ConveyorSide side, int motorSpeed) {
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

        void stop() {
            analogWrite(pwm, 0);
            digitalWrite(reverse, LOW);

            feedingSide = CONVEYOR_IDLE;
            speed = 0;
        }
};

#endif