#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <emscripten.h>

#include "config.h"

void pinMode(uint8_t pinNumber, uint8_t mode);
void digitalWrite(uint8_t pinNumber, uint8_t value) {
    switch(pinNumber) {
    case MAIN_MOTOR_OUT2:
        const int movingDirection = feeder.getMovingDirection();
        if (movingDirection == MOVING_FORWARD) {
            EM_ASM({
                window.simulator.zone.run(() => {
                    window.simulator.service.moveMainMotor('forward');
                });
            });
        } else if (movingDirection == MOVING_BACKWARD) {
            EM_ASM({
                window.simulator.zone.run(() => {
                    window.simulator.service.moveMainMotor('backward');
                });
            });
        } else {
            EM_ASM({
                window.simulator.zone.run(() => {
                    window.simulator.service.stopMainMotor();
                });
            });
        }
        break;
    case GREEN_LIGHT:
        if (value == HIGH) {
            EM_ASM({
                window.simulator.zone.run(() => {
                    window.simulator.service.turnOnLight('green');
                });
            });
        } else {
            EM_ASM({
                window.simulator.zone.run(() => {
                    window.simulator.service.turnOffLight('green');
                });
            });
        }
        break;
    case RED_LIGHT:
        if (value == HIGH) {
            EM_ASM({
                window.simulator.zone.run(() => {
                    window.simulator.service.turnOnLight('red');
                });
            });
        } else {
            EM_ASM({
                window.simulator.zone.run(() => {
                    window.simulator.service.turnOffLight('red');
                });
            });
        }
        break;
    }
}

int digitalRead(uint8_t pinNumber) {
    switch(pinNumber) {
    case POWER_BUTTON:
        return EM_ASM_INT({
            return +window.simulator.service.getPowerStatus();
        });
        break; 
    case SAFETY_SENSOR_FRONT:
        return EM_ASM_INT({
            return +window.simulator.service.getSafetySensorStatus('front');
        });
        break;
    case SAFETY_SENSOR_BACK:
        return EM_ASM_INT({
            return +window.simulator.service.getSafetySensorStatus('back');
        });
        break;
    default:
        return LOW;
    }
}

void analogWrite(uint8_t pinNumber, uint8_t value) {}

void delay(unsigned long microseconds) {
    sleep(microseconds);
}

class Serial_ {
public:
	Serial_() {};
	void begin(unsigned long);
	void end(void);
    
    // size_t print(const __FlashStringHelper *);
    size_t print(const std::string &);
    size_t print(const char[]);
    size_t print(char);
    // size_t print(unsigned char, int = DEC);
    // size_t print(int, int = DEC);
    // size_t print(unsigned int, int = DEC);
    // size_t print(long, int = DEC);
    // size_t print(unsigned long, int = DEC);
    size_t print(double, int = 2);
    // size_t print(const Printable&);

    // size_t println(const __FlashStringHelper *);
    size_t println(const std::string &s);
    size_t println(const char[]);
    size_t println(char);
    // size_t println(unsigned char, int = DEC);
    // size_t println(int, int = DEC);
    // size_t println(unsigned int, int = DEC);
    // size_t println(long, int = DEC);
    // size_t println(unsigned long, int = DEC);
    size_t println(double, int = 2);
    // size_t println(const Printable&);
    size_t println(void);
};

Serial_ Serial = Serial_();

