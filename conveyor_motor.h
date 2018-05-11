#include <Arduino.h>
#include "constants.h"

#ifndef CONVEYOR_MOTOR_H
#define CONVEYOR_MOTOR_H

class ConveyorMotor {
    public:
        PinConfig pwm;
        PinConfig reverse;
        ConveyorSide feedingSide;

        ConveyorMotor(PinConfig motorPwm, PinConfig motorReverse);

        void setup();
        void start(ConveyorSide side, int motorSpeed);
        void stop();
        int getSpeed();
    
    private:
        int speed;
};

class FrontConveyor: public ConveyorMotor {
    public:
        static FrontConveyor & getInstance();

    private:
        FrontConveyor();

        void operator = (FrontConveyor const&); // Don't implement
};

class BackConveyor: public ConveyorMotor {
    public:
        static BackConveyor & getInstance();

    private:
        BackConveyor();

        void operator = (BackConveyor const&); // Don't implement
};

#endif