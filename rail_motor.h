#include "constants.h"
#include "state_manager.h"

#ifndef RAIL_MOTOR_H
#define RAIL_MOTOR_H

class RailMotor {
    public:
        static RailMotor & getInstance();

        // void loop();
        void moveForward();
        void moveBackward();
        void stop();
        void inverseMovingDirection();

    private:
        RailMotor();
        void setLight(LightColor lightColor, bool blinking);
        void operator = (RailMotor const&); // Don't implement
};

#endif
