#include "constants.h"
#include "state_manager.h"

#ifndef RAIL_MOTOR_H
#define RAIL_MOTOR_H

class RailMotor {
    public:
        RailMotor();

        void loop();
        void moveForward();
        void moveBackward();
        void stop();
        // void inverseMovingDirection();

    private:
        MovingDirection currentDirection;

        void setLight(LightColor lightColor, bool blinking);
};

#endif
