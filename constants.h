#include <Arduino.h>

#ifndef CONSTANTS_H
#define CONSTANTS_H

enum PinConfig {
    // INPUTS
    POWER_BUTTON = A0,
    SAFETY_SENSOR_FRONT = A5,
    SAFETY_SENSOR_BACK = A6,
    SD_CARD_CS = 4,
    RFID_RSA_PIN = 10,
    RFID_RST_PIN = 9,

    // OUTPUTS
    GREEN_LIGHT = 22,
    RED_LIGHT = 24,
    GREEN_BACKWARD_LIGHT = 26,

    // RAIL MOTOR
    MAIN_MOTOR_OUT1 = A9,
    MAIN_MOTOR_OUT2 = A10,

    // CONVEYOR MOTORS
    CONVEYOR_MOTOR_FRONT_PWM = 4,
    CONVEYOR_MOTOR_FRONT_REVERSE = 46,
    CONVEYOR_MOTOR_BACK_PWM = A13,
    CONVEYOR_MOTOR_BACK_REVERSE = A12,
};

enum MachineState {
    Off,
    MainMenu,
    Automatic,
    ManualMenu,
    ManualMealDistribution,
    ManualControl,
    History,
    Diagnostic
};

enum MovingDirection {
    MOVING_IDLE,
    MOVING_FORWARD,
    MOVING_BACKWARD
};

enum FeederState {
    FEEDER_IDLE,
    FEEDER_MOVING,
    FEEDER_REFILLING,
    FEEDER_SAFETY_STOP
};

enum ConveyorSide {
    CONVEYOR_IDLE,
    CONVEYOR_SIDE_LEFT,
    CONVEYOR_SIDE_RIGHT
};

enum LightColor {
    GREEN,
    RED,
    GREEN_BACKWARD
};

const PinConfig INPUTS[1] = {
    POWER_BUTTON,
};
const int INPUTS_COUNT = 1;

#endif
