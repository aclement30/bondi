#ifndef FEEDER_H
#define FEEDER_H

//#include "conveyor_motor.h"
#include "rail_motor.h"

// STATES
const int STATE_IDLE = 0;
const int STATE_MOVING = 1;
const int STATE_REFILLING = 2;
const int STATE_SAFETY_STOP = 3;

// LIGHT COLORS
const int LIGHT_GREEN = 1;
const int LIGHT_RED = 2;

class Feeder {
    public:
        int state = STATE_IDLE;
        ConveyorMotor conveyorFront;
        ConveyorMotor conveyorBack;

        Feeder(
            RailMotor motor,
            ConveyorMotor feederConveyorFront,
            ConveyorMotor feederConveyorBack,
            int feederGreenLight,
            int feederRedLight
        ) : 
            mainMotor(motor),
            conveyorFront(feederConveyorFront),
            conveyorBack(feederConveyorBack),
            greenLight(feederGreenLight),
            redLight(feederRedLight)
        {
            pinMode(greenLight, OUTPUT);
            pinMode(redLight, OUTPUT);
            // Set to LOW so no power is flowing through the output
            digitalWrite(greenLight, LOW);
            digitalWrite(redLight, LOW);
        }

        void moveForward() {
            mainMotor.moveForward();
            changeState(STATE_MOVING);
        }

        void moveBackward() {
            mainMotor.moveBackward();
            changeState(STATE_MOVING);
        }

        void stop() {
            mainMotor.stop();
            changeState(STATE_IDLE);
        }

        void safetyStop() {
            mainMotor.stop();

            // Shutdown all motors
            conveyorFront.stop();
            conveyorBack.stop();
            
            changeState(STATE_SAFETY_STOP);
        }

        void emergencyStop() {
            mainMotor.stop();

            // Shutdown all motors
            conveyorFront.stop();
            conveyorBack.stop();
            
            changeState(STATE_SAFETY_STOP);
        }

        int getMovingDirection() {
            return mainMotor.state;
        }

        void setLight(int lightColor, bool blinking) {
            if (lightColor == LIGHT_GREEN) {
                digitalWrite(greenLight, LOW);
                digitalWrite(redLight, HIGH);
            } else {
                digitalWrite(greenLight, HIGH);
                digitalWrite(redLight, LOW);
            }
        }

    private:
        RailMotor mainMotor;
        const int greenLight;
        const int redLight;
        int previousState = STATE_IDLE;

        void changeState(int newState) {
            previousState = state;
            state = newState;

            if (state == STATE_MOVING || state == STATE_REFILLING) {
                setLight(LIGHT_GREEN, false);
            } else if (state == STATE_SAFETY_STOP) {
                setLight(LIGHT_RED, true);
            }
        }
};

#endif