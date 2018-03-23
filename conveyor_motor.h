#include <Arduino.h>
#include "constants.h"

#ifndef CONVEYORMOTOR_H
#define CONVEYORMOTOR_H

class ConveyorMotor {
    public:
        PinConfig pwm;
        PinConfig reverse;
        ConveyorSide feedingSide;
        int speed;

        ConveyorMotor(PinConfig motorPwm, PinConfig motorReverse);

        void setup();
        void start(ConveyorSide side, int motorSpeed);
        void stop();
};

#endif