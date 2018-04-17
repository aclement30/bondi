#include <Arduino.h>

#ifndef CONSTANTS_H
#define CONSTANTS_H

enum PinConfig {
    // INPUTS
    SAFETY_SENSOR_FRONT = 37,
    SAFETY_SENSOR_BACK = 36,
    SD_CARD_CS = 7,
    RFID_RSA_PIN = 9,
    RFID_RST_PIN = 8,

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

enum FeederEvent {
    STARTUP,
    MEAL_DISTRIBUTION_START,
    MEAL_DISTRIBUTION_END,
    MANUAL_MEAL_SELECTION,
    ROUTE_START,
    ROUTE_END,
    SAFETY_BAR_PRESSED,
    SAFETY_MODE_ENGAGED,
    SAFETY_MODE_DISENGAGED,
    MANUAL_CONTROL_START,
    MANUAL_CONTROL_END,
    MANUAL_RESTART,
};

const char PROGMEM FILE_MEAL_SEQUENCES[] = "MEAL_SEQ.CSV";
const char PROGMEM FILE_POINTS[] = "POINTS.CSV";
const char PROGMEM FILE_MEALS[] = "MEALS.CSV";
const char PROGMEM FILE_LOGS[] = "LOGS/SYSTEM.LOG";

#endif
