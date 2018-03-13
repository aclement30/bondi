#include "constants.h"
#include "state_manager.h"

#ifndef RAILMOTOR_H
#define RAILMOTOR_H

class RailMotor {
    public:
        RailMotor() {
            pinMode(MAIN_MOTOR_OUT1, OUTPUT);
            pinMode(MAIN_MOTOR_OUT2, OUTPUT);

            // Set to LOW so no power is flowing through the output
            digitalWrite(MAIN_MOTOR_OUT1, LOW);
            digitalWrite(MAIN_MOTOR_OUT2, LOW);
        }

        void loop() {
            MovingDirection newDirection = StateManager::getInstance().getMovingDirection();
            if (currentDirection == newDirection) return;

            // First stop the motor
            if (currentDirection != MOVING_IDLE) {
                Serial.println("Moteur principal: arrêt");
                stop();
            }

            // Then, change the motor direction
            if (newDirection == MOVING_FORWARD) {
                Serial.println("Moteur principal: rotation avant");
                moveForward();
            } else if (newDirection == MOVING_BACKWARD) {
                Serial.println("Moteur principal: rotation arrière");
                moveBackward();
            }
        }

        void moveForward() {
            digitalWrite(MAIN_MOTOR_OUT1, HIGH);
            digitalWrite(MAIN_MOTOR_OUT2, LOW);
        }

        void moveBackward() {
            digitalWrite(MAIN_MOTOR_OUT1, LOW);
            digitalWrite(MAIN_MOTOR_OUT2, HIGH);
        }

        void stop() {
            digitalWrite(MAIN_MOTOR_OUT1, HIGH);
            digitalWrite(MAIN_MOTOR_OUT2, HIGH);
        }

        // void inverseMovingDirection() {
        //     stop();

        //     if (previousState == MOVING_FORWARD) {
        //         moveBackward();
        //     } else if (previousState == MOVING_BACKWARD) {
        //         moveForward();
        //     }
        // }
    private:
        MovingDirection currentDirection = MOVING_IDLE;
};

#endif