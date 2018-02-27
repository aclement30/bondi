#ifndef CONVEYORMOTOR_H
#define CONVEYORMOTOR_H

const int DIRECTION_IDLE = 0;
const int DIRECTION_LEFT = 1;
const int DIRECTION_RIGHT = 2;

class ConveyorMotor {
    public:
        const int pwm;
        const int reverse;

        int movingDirection = DIRECTION_IDLE;
        int speed = 0;

        ConveyorMotor(
            int motorPwm, 
            int motorReverse
        ) : 
            pwm(motorPwm),
            reverse(motorReverse)
        {
            pinMode(pwm, OUTPUT);
            pinMode(reverse, OUTPUT);
            // Set to LOW so no power is flowing through the output
            digitalWrite(pwm, LOW);
            digitalWrite(reverse, LOW);
        }

        void move(int direction, int motorSpeed) {
            if (direction == DIRECTION_LEFT) {
                digitalWrite(reverse, HIGH);
            } else {
                digitalWrite(reverse, LOW);
            }

            analogWrite(pwm, motorSpeed);

            movingDirection = direction;
            speed = motorSpeed;
        }

        void stop() {
            analogWrite(pwm, 0);
            digitalWrite(reverse, LOW);

            movingDirection = DIRECTION_IDLE;
            speed = 0;
        }
};

#endif