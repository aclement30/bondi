#include "constants.h"
#include "state_manager.h"

#ifndef RAILMOTOR_H
#define RAILMOTOR_H

class RailMotor: public DirectionAware {
    public:
        PinConfig out1;
        PinConfig out2;

        RailMotor(
            PinConfig motorOut1, 
            PinConfig motorOut2
        ) : 
            out1(motorOut1),
            out2(motorOut2)
        {
            pinMode(out1, OUTPUT);
            pinMode(out2, OUTPUT);
            // Set to LOW so no power is flowing through the output
            digitalWrite(out1, LOW);
            digitalWrite(out2, LOW);

            StateManager::getInstance().subscribeToDirection(this);
        }

        void didChangeDirection(MovingDirection direction) {
            if (direction == MOVING_FORWARD) {
                Serial.println("Moteur principal: rotation avant");
                moveForward();
            } else if (direction == MOVING_BACKWARD) {
                Serial.println("Moteur principal: rotation arrière");
                moveBackward();
            } else {
                Serial.println("Moteur principal: arrêt");
                stop();
            }
        }

        void moveForward() {
            digitalWrite(out1, HIGH);
            digitalWrite(out2, LOW);
        }

        void moveBackward() {
            digitalWrite(out1, LOW);
            digitalWrite(out2, HIGH);
        }

        void stop() {
            digitalWrite(out1, HIGH);
            digitalWrite(out2, HIGH);
        }

        // void inverseMovingDirection() {
        //     stop();

        //     if (previousState == MOVING_FORWARD) {
        //         moveBackward();
        //     } else if (previousState == MOVING_BACKWARD) {
        //         moveForward();
        //     }
        // }
};

#endif