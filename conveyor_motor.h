#ifndef CONVEYORMOTOR_H
#define CONVEYORMOTOR_H

const int CONVEYOR_IDLE = 0;
const int CONVEYOR_SIDE_LEFT = 1;
const int CONVEYOR_SIDE_RIGHT = 2;

class ConveyorMotor {
    public:
        PinConfig pwm;
        PinConfig reverse;

        int feedingSide = CONVEYOR_IDLE;
        int speed = 0;

        ConveyorMotor(
            PinConfig motorPwm, 
            PinConfig motorReverse
        ) : 
            pwm(motorPwm),
            reverse(motorReverse)
        {}

        void setup() {
            pinMode(pwm, OUTPUT);
            pinMode(reverse, OUTPUT);
            // Set to LOW so no power is flowing through the output
            digitalWrite(pwm, LOW);
            digitalWrite(reverse, LOW);

            Serial.println("Configuration du convoyeur");
        }

        void start(int side, int motorSpeed) {
            if (side == CONVEYOR_SIDE_LEFT) {
                digitalWrite(reverse, HIGH);
            } else {
                digitalWrite(reverse, LOW);
            }

            analogWrite(pwm, motorSpeed);

            feedingSide = side;
            speed = motorSpeed;
        }

        void stop() {
            analogWrite(pwm, 0);
            digitalWrite(reverse, LOW);

            feedingSide = CONVEYOR_IDLE;
            speed = 0;
        }
};

#endif