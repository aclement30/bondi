#include "constants.h"
#include "conveyor_motor.h"
#include "location_service.h"
#include "rail_motor.h"
#include "rail_point.h"
#include "route.h"

#ifndef FEEDER_H
#define FEEDER_H

enum FeederState {
    FEEDER_IDLE,
    FEEDER_MOVING,
    FEEDER_REFILLING,
    FEEDER_SAFETY_STOP
};

class Feeder: public LocationAware {
    public:
        FeederState state = FEEDER_IDLE;
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

            Serial.println("Configuration du robot complétée");
        }

        void moveForward() {
            mainMotor.moveForward();
            changeState(FEEDER_MOVING);
            Serial.println("Moteur principal: rotation avant");
        }

        void moveBackward() {
            mainMotor.moveBackward();
            changeState(FEEDER_MOVING);
            Serial.println("Moteur principal: rotation arrière");
        }

        void stop() {
            mainMotor.stop();
            changeState(FEEDER_MOVING);
            Serial.println("Moteur principal: arrêt");
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
                Serial.println("ERREUR: le robot doit être arrêté au dock avant de commencer la route !");
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
            return state == FEEDER_IDLE && activePoint.isDock();
        }

        bool hasCurrentRoute() {
            return currentRoutePtr != NULL;
        }

        void checkSafetyState() {
            if (state != FEEDER_SAFETY_STOP && isSafetyBarPressed()) {
                Serial.println("Barre de sécurité enclenchée");

                // Shutdown everything immediately
                safetyStop();
            } else if (state == FEEDER_SAFETY_STOP && !isSafetyBarPressed()) {
                // Note: we should wait for manual reactivation or a timeout before reactivating automatically
                // TODO: Reactivate robot
            }
        }

        void safetyStop() {
            mainMotor.stop();
            stopFeeding();
            
            changeState(FEEDER_SAFETY_STOP);

            Serial.println("(!) Arrêt d'urgence");
        }

        void emergencyStop() {
            mainMotor.stop();
            stopFeeding();
            
            changeState(FEEDER_SAFETY_STOP);

            Serial.println("(!) Arrêt d'urgence");
        }

        int getMovingDirection() {
            return mainMotor.state;
        }

        void stopFeeding() {
            // Shutdown conveyors motor
            conveyorFront.stop();
            conveyorBack.stop();

            Serial.println("Convoyeurs: arrêt");
        }

        void setLight(int lightColor, bool blinking) {
            if (lightColor == LIGHT_GREEN) {
                digitalWrite(greenLight, HIGH);
                digitalWrite(redLight, LOW);
                
                Serial.println("Lumière VERTE allumée");
            } else {
                digitalWrite(greenLight, LOW);
                digitalWrite(redLight, HIGH);

                Serial.println("Lumière ROUGE allumée");
            }
        }

    private:
        RailMotor mainMotor;
        const int greenLight;
        const int redLight;
        const int safetySensorFront;
        const int safetySensorBack;
        FeederState previousState = FEEDER_IDLE;
        Route *currentRoutePtr = NULL;
        RailPoint *lastPointPtr = NULL;

        void changeState(FeederState newState) {
            previousState = state;
            state = newState;

            if (state == FEEDER_MOVING || state == FEEDER_REFILLING) {
                setLight(LIGHT_GREEN, false);
            } else if (state == FEEDER_SAFETY_STOP) {
                setLight(LIGHT_RED, true);
            }
        }

        void completeRoute() {
            currentRoutePtr = NULL;
            stop();
        }
};

#endif