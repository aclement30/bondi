#include "constants.h"
#include "conveyor_motor.h"
#include "location_service.h"
#include "rail_motor.h"
#include "rail_point.h"
#include "route.h"

#ifndef FEEDER_H
#define FEEDER_H

class Feeder: public LocationAware {
    public:
        int state = STATE_IDLE;
        ConveyorMotor conveyorFront;
        ConveyorMotor conveyorBack;
        LocationService locationService;
        
        Feeder(
            RailMotor motor,
            ConveyorMotor feederConveyorFront,
            ConveyorMotor feederConveyorBack,
            int feederGreenLight,
            int feederRedLight,
            int feederSafetySensorFront,
            int feederSafetySensorBack,
            LocationService feederLocationService
        ) : 
            mainMotor(motor),
            conveyorFront(feederConveyorFront),
            conveyorBack(feederConveyorBack),
            greenLight(feederGreenLight),
            redLight(feederRedLight),
            safetySensorFront(feederSafetySensorFront),
            safetySensorBack(feederSafetySensorBack),
            locationService(feederLocationService)
        {
            locationService.subscribe(this);
        }

        void setup() {
            pinMode(greenLight, OUTPUT);
            pinMode(redLight, OUTPUT);
            pinMode(safetySensorFront, INPUT);
            pinMode(safetySensorBack, INPUT);
            // Set to LOW so no power is flowing through the output
            digitalWrite(greenLight, LOW);
            digitalWrite(redLight, LOW);

            mainMotor.setup();
            conveyorFront.setup();
            conveyorBack.setup();

            Serial.println("Feeder setup completed");
        }

        void moveForward() {
            mainMotor.moveForward();
            changeState(STATE_MOVING);
            Serial.println("Main motor: move forward");
        }

        void moveBackward() {
            mainMotor.moveBackward();
            changeState(STATE_MOVING);
            Serial.println("Main motor: move backward");
        }

        void stop() {
            mainMotor.stop();
            changeState(STATE_IDLE);
            Serial.println("Main motor: stop");
        }

        void followRoute(Route route) {
            currentRoutePtr = &route;

            if (isDocked()) {
                if (route.initialDirection == MOVING_FORWARD) {
                    moveForward();
                } else {
                    moveBackward();
                }
            } else {
                Serial.println("Error: cannot follow route starting away from docking station");
            }
        }

        void didUpdateLocation(RailPoint railPoint) {
            if (railPoint.isReverse()) {
                stopFeeding();
                mainMotor.inverseMovingDirection();
            }

            if (currentRoutePtr != NULL) {
                Route currentRoute = Route(* currentRoutePtr);
                RailPoint lastPoint = RailPoint(* lastPointPtr);

                if (railPoint.id == currentRoute.endPoint.id && railPoint.id != lastPoint.id) {
                    completeRoute();
                }
            }

            lastPointPtr = &railPoint;
        }

        bool isSafetyBarPressed() {
            return (mainMotor.state == MOVING_FORWARD && digitalRead(safetySensorFront) == HIGH) 
                || (mainMotor.state == MOVING_BACKWARD && digitalRead(safetySensorBack) == HIGH);
        }

        bool isDocked() {
            RailPoint activePoint = locationService.getActiveRailPoint();
            return state == STATE_IDLE && activePoint.isDock();
        }

        bool hasCurrentRoute() {
            return currentRoutePtr != NULL;
        }

        void checkSafetyState() {
            if (state != STATE_SAFETY_STOP && isSafetyBarPressed()) {
                Serial.println("Safety bar pressed");

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

            Serial.println("(!) Safety stop");
        }

        void emergencyStop() {
            mainMotor.stop();
            stopFeeding();
            
            changeState(STATE_SAFETY_STOP);

            Serial.println("(!) Emergency stop");
        }

        int getMovingDirection() {
            return mainMotor.state;
        }

        void stopFeeding() {
            // Shutdown conveyors motor
            conveyorFront.stop();
            conveyorBack.stop();

            Serial.println("Stop all conveyors");
        }

        void setLight(int lightColor, bool blinking) {
            if (lightColor == LIGHT_GREEN) {
                digitalWrite(greenLight, HIGH);
                digitalWrite(redLight, LOW);
                
                Serial.println("Turn on GREEN light");
            } else {
                digitalWrite(greenLight, LOW);
                digitalWrite(redLight, HIGH);

                Serial.println("Turn on RED light");
            }
        }

    private:
        RailMotor mainMotor;
        const int greenLight;
        const int redLight;
        const int safetySensorFront;
        const int safetySensorBack;
        int previousState = STATE_IDLE;
        Route *currentRoutePtr = NULL;
        RailPoint *lastPointPtr = NULL;

        void changeState(int newState) {
            previousState = state;
            state = newState;

            if (state == STATE_MOVING || state == STATE_REFILLING || state == STATE_DIAGNOSTIC) {
                setLight(LIGHT_GREEN, false);
            } else if (state == STATE_SAFETY_STOP) {
                setLight(LIGHT_RED, true);
            }
        }

        void completeRoute() {
            currentRoutePtr = NULL;
            stop();
        }
};

#endif