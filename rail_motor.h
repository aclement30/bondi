#ifndef RAILMOTOR_H
#define RAILMOTOR_H

// MOVING DIRECTIONS
const int MOVING_IDLE = 0;
const int MOVING_FORWARD = 1;
const int MOVING_BACKWARD = 2;

class RailMotor {
    public:
        const int out1;
        const int out2;

        int state = MOVING_IDLE;

        RailMotor(
            int motorOut1, 
            int motorOut2
        ) : 
            out1(motorOut1),
            out2(motorOut2)
        {
            pinMode(out1, OUTPUT);
            pinMode(out2, OUTPUT);
            // Set to LOW so no power is flowing through the output
            digitalWrite(out1, LOW);
            digitalWrite(out2, LOW);
        }

        void moveForward() {
            digitalWrite(out1, HIGH);
            digitalWrite(out2, LOW);

            changeState(MOVING_FORWARD);
        }

        void moveBackward() {
            digitalWrite(out1, LOW);
            digitalWrite(out2, HIGH);
            
            changeState(MOVING_BACKWARD);
        }

        void stop() {
            digitalWrite(out1, HIGH);
            digitalWrite(out2, HIGH);

            changeState(MOVING_IDLE);
        }

        void inverseMovingDirection() {
            stop();

            if (previousState == MOVING_FORWARD) {
                moveBackward();
            } else if (previousState == MOVING_BACKWARD) {
                moveForward();
            }
        }

    private:
        int previousState = MOVING_IDLE;

        void changeState(int newState) {
            previousState = state;
            state = newState;
        }
};

#endif