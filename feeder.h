#include "config.h"
#include "conveyor_motor.h"
#include "rail_motor.h"

#ifndef FEEDER_H
#define FEEDER_H

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
            int feederRedLight,
            int feederSafetySensorFront,
            int feederSafetySensorBack
        ) : 
            mainMotor(motor),
            conveyorFront(feederConveyorFront),
            conveyorBack(feederConveyorBack),
            greenLight(feederGreenLight),
            redLight(feederRedLight),
            safetySensorFront(feederSafetySensorFront),
            safetySensorBack(feederSafetySensorBack)
        {
            pinMode(greenLight, OUTPUT);
            pinMode(redLight, OUTPUT);
            pinMode(safetySensorFront, INPUT);
            pinMode(safetySensorBack, INPUT);
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

        void checkMovingDirectionState() {
            if (1 == 1) {
                stopFeeding();
                mainMotor.inverseMovingDirection();
            }
        }

        bool isSafetyBarPressed() {
            return (mainMotor.state == MOVING_FORWARD && digitalRead(safetySensorFront) == LOW) 
                || (mainMotor.state == MOVING_BACKWARD && digitalRead(safetySensorBack) == LOW);
        }

        void checkSafetyState() {
            if (state != STATE_SAFETY_STOP && isSafetyBarPressed()) {
                // Shutdown everything immediately
                safetyStop();
            } else if (state == STATE_SAFETY_STOP && !isSafetyBarPressed()) {
                // Note: we should wait for manual reactivation or a timeout before reactivating automatically
                // TODO: Reactivate robot
            }
        }

        void safetyStop() {
            mainMotor.stop();
            stopFeeding();
            
            changeState(STATE_SAFETY_STOP);
        }

        void emergencyStop() {
            mainMotor.stop();
            stopFeeding();
            
            changeState(STATE_SAFETY_STOP);
        }

        int getMovingDirection() {
            return mainMotor.state;
        }

        void stopFeeding() {
            // Shutdown conveyors motor
            conveyorFront.stop();
            conveyorBack.stop();
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
        const int safetySensorFront;
        const int safetySensorBack;
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