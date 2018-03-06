#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "rail_point.h"

void pinMode(uint8_t pinNumber, uint8_t mode) {}
void digitalWrite(uint8_t pinNumber, uint8_t value) {
    switch(pinNumber) {
    case MAIN_MOTOR_OUT2: {
        const int movingDirection = feeder.getMovingDirection();
        if (movingDirection == MOVING_FORWARD) {
            EM_ASM({
                console.log('moveMainMotor(forward)');
                window.simulator && window.simulator.zone.run(() => {
                    window.simulator.service.moveMainMotor('forward');
                });
            });
        } else if (movingDirection == MOVING_BACKWARD) {
            EM_ASM({
                console.log('moveMainMotor(backward)');
                window.simulator && window.simulator.zone.run(() => {
                    window.simulator.service.moveMainMotor('backward');
                });
            });
        } else {
            EM_ASM({
                console.log('stopMainMotor()');
                window.simulator && window.simulator.zone.run(() => {
                    window.simulator.service.stopMainMotor();
                });
            });
        }
        break;
    }
    case GREEN_LIGHT:
        if (value == HIGH) {
            EM_ASM({
                console.log('turnOnLight(green)');
                window.simulator && window.simulator.zone.run(() => {
                    window.simulator.service.turnOnLight('green');
                });
            });
        } else {
            EM_ASM({
                console.log('turnOffLight(green)');
                window.simulator && window.simulator.zone.run(() => {
                    window.simulator.service.turnOffLight('green');
                });
            });
        }
        break;
    case RED_LIGHT:
        if (value == HIGH) {
            EM_ASM({
                console.log('turnOnLight(red)');
                window.simulator && window.simulator.zone.run(() => {
                    window.simulator.service.turnOnLight('red');
                });
            });
        } else {
            EM_ASM({
                console.log('turnOffLight(red)');
                window.simulator && window.simulator.zone.run(() => {
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
            console.log('getPowerStatus()');
            return window.simulator && +window.simulator.service.getPowerStatus();
        });
        break; 
    case SAFETY_SENSOR_FRONT:
        return EM_ASM_INT({
            console.log('getSafetySensorStatus(front)');
            return window.simulator && +window.simulator.service.getSafetySensorStatus('front');
        });
        break;
    case SAFETY_SENSOR_BACK:
        return EM_ASM_INT({
            console.log('getSafetySensorStatus(back)');
            return window.simulator && +window.simulator.service.getSafetySensorStatus('back');
        });
        break;
    default:
        return LOW;
    }
}

void analogWrite(uint8_t pinNumber, uint8_t value) {
    switch(pinNumber) {
        case CONVEYOR_MOTOR_FRONT_PWM: {
            const char *feedingSide = (conveyorFront.feedingSide == CONVEYOR_SIDE_LEFT) ? "left" : "right";
            if (value == 0) {
                EM_ASM({
                    console.log('startConveyor(front)');
                    window.simulator && window.simulator.zone.run(() => {
                        window.simulator.service.startConveyor('front', $0, $1);
                    });
                }, value, feedingSide);
            } else {
                EM_ASM({
                    console.log('stopConveyor(front)');
                    window.simulator && window.simulator.zone.run(() => {
                        window.simulator.service.stopConveyor('front');
                    });
                });
            }
            break;
        }
        case CONVEYOR_MOTOR_BACK_PWM: {
            const char *feedingSide = (conveyorBack.feedingSide == CONVEYOR_SIDE_LEFT) ? "left" : "right";
            if (value == 0) {
                EM_ASM({
                    console.log('startConveyor(back)');
                    window.simulator && window.simulator.zone.run(() => {
                        window.simulator.service.startConveyor('back', $0, $1);
                    });
                }, value, feedingSide);
            } else {
                EM_ASM({
                    console.log('stopConveyor(back)');
                    window.simulator && window.simulator.zone.run(() => {
                        window.simulator.service.stopConveyor('back');
                    });
                });
            }
            break;
        }
    }
}

void delay(unsigned long microseconds) {
    sleep(microseconds);
}