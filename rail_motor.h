#include "constants.h"
#include "state_manager.h"

#ifndef RAILMOTOR_H
#define RAILMOTOR_H

class RailMotor {
    public:
        RailMotor() {
            pinMode(MAIN_MOTOR_OUT1, OUTPUT);
            pinMode(MAIN_MOTOR_OUT2, OUTPUT);

            pinMode(GREEN_LIGHT, OUTPUT);
            pinMode(RED_LIGHT, OUTPUT);
            pinMode(GREEN_BACKWARD_LIGHT, OUTPUT);

            // Set to LOW so no power is flowing through the output
            digitalWrite(MAIN_MOTOR_OUT1, LOW);
            digitalWrite(MAIN_MOTOR_OUT2, LOW);

            digitalWrite(GREEN_LIGHT, LOW);
            digitalWrite(RED_LIGHT, LOW);
            digitalWrite(GREEN_BACKWARD_LIGHT, LOW);
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

            currentDirection = newDirection;
        }

        void moveForward() {
            digitalWrite(MAIN_MOTOR_OUT1, HIGH);
            digitalWrite(MAIN_MOTOR_OUT2, LOW);

            setLight(GREEN, false);
        }

        void moveBackward() {
            digitalWrite(MAIN_MOTOR_OUT1, LOW);
            digitalWrite(MAIN_MOTOR_OUT2, HIGH);

            setLight(GREEN_BACKWARD, false);
        }

        void stop() {
            digitalWrite(MAIN_MOTOR_OUT1, HIGH);
            digitalWrite(MAIN_MOTOR_OUT2, HIGH);

            setLight(RED, false);
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

         void setLight(LightColor lightColor, bool blinking) {
            if (lightColor == GREEN) {
                digitalWrite(GREEN_LIGHT, HIGH);
                digitalWrite(RED_LIGHT, LOW);
                digitalWrite(GREEN_BACKWARD_LIGHT, LOW);

                Serial.println("Lumière VERTE allumée");
            } else if (lightColor == GREEN_BACKWARD) {
                digitalWrite(GREEN_LIGHT, LOW);
                digitalWrite(RED_LIGHT, LOW);
                digitalWrite(GREEN_BACKWARD_LIGHT, HIGH);

                Serial.println("Lumière VERTE #2 allumée");
            } else {
                digitalWrite(GREEN_LIGHT, LOW);
                digitalWrite(RED_LIGHT, HIGH);
                digitalWrite(GREEN_BACKWARD_LIGHT, LOW);

                Serial.println("Lumière ROUGE allumée");
            }
        }
};

#endif
