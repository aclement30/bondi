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
    RTC_CE_PIN = 6,
    RTC_IO_PIN = 5,
    RTC_SCLK_PIN = 4,
    
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
    MANUAL_RESTART
};

enum ConfigurationError {
    CONFIG_VALID,
    CONFIG_NO_DOCK_POINT,
    CONFIG_DUPLICATE_RAIL_POINT_ID,
    CONFIG_DUPLICATE_RAIL_POINT_RFID_UID,
    CONFIG_DUPLICATE_MEAL_ID,
    CONFIG_INVALID_MEAL_START_MOMENT,
    CONFIG_DUPLICATE_MEAL_START_MOMENT,
    CONFIG_UNKNOWN_MEAL_ROUTE_ID,
    CONFIG_UNKNOWN_MEAL_START_POINT,
    CONFIG_UNKNOWN_MEAL_END_POIND
}

const char PROGMEM FILE_MEAL_SEQUENCES[] = "MEAL_SEQ.CSV";
const char PROGMEM FILE_POINTS[] = "POINTS.CSV";
const char PROGMEM FILE_MEALS[] = "MEALS.CSV";
const char PROGMEM FILE_CONFIG_VERSION[] = "VERSION.CFG";
const char PROGMEM PATH_DISTRIB_LOGS[] = "LOGS/DISTRIB";
const char PROGMEM PATH_SYSTEM_LOGS[] = "LOGS/SYSTEM";

const char PROGMEM APP_VERSION[] = "0.0.1";

#endif
