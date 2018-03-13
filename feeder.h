// #include "constants.h"
// #include "conveyor_motor.h"
// #include "location_service.h"
// #include "rail_motor.h"
// #include "rail_point.h"
// #include "route.h"

// #ifndef FEEDER_H
// #define FEEDER_H

// class Feeder {
//     public:
//         FeederState state = FEEDER_IDLE;
//         // ConveyorMotor conveyorFront;
//         // ConveyorMotor conveyorBack;
        
//         Feeder(
//             RailMotor motor,
//             // ConveyorMotor feederConveyorFront,
//             // ConveyorMotor feederConveyorBack,
//             // PinConfig feederGreenLight,
//             // PinConfig feederRedLight,
//             // PinConfig feederSafetySensorFront,
//             // PinConfig feederSafetySensorBack,
//             LocationService feederLocationService
//         ) : 
//             mainMotor(motor),
//             // conveyorFront(feederConveyorFront),
//             // conveyorBack(feederConveyorBack),
//             // greenLight(feederGreenLight),
//             // redLight(feederRedLight)
//             // safetySensorFront(feederSafetySensorFront),
//             // safetySensorBack(feederSafetySensorBack),
//         {
//             locationService.subscribe(this);
//         }

//         void setup() {
//             pinMode(GREEN_LIGHT, OUTPUT);
//             pinMode(RED_LIGHT, OUTPUT);
//             // pinMode(safetySensorFront, INPUT);
//             // pinMode(safetySensorBack, INPUT);
//             // Set to LOW so no power is flowing through the output
//             digitalWrite(GREEN_LIGHT, LOW);
//             digitalWrite(RED_LIGHT, LOW);

//             mainMotor.setup();
//             // conveyorFront.setup();
//             // conveyorBack.setup();

//             Serial.println("Configuration du robot");
//         }

//         // void didChangeDirection(MovingDirection direction) {
//         //     if (direction == MOVING_FORWARD) {
//         //         Serial.println("Moteur principal: rotation avant");
//         //         mainMotor.moveForward();
//         //     } else if (direction == MOVING_BACKWARD) {
//         //         Serial.println("Moteur principal: rotation arrière");
//         //         mainMotor.moveBackward();
//         //     } else {
//         //         Serial.println("Moteur principal: arrêt");
//         //         mainMotor.stop();
//         //     }
//         // }

//         // void moveForward() {
//         //     mainMotor.moveForward();
//         //     changeState(FEEDER_MOVING);
//         //     Serial.println("Moteur principal: rotation avant");
//         // }

//         // void moveBackward() {
//         //     mainMotor.moveBackward();
//         //     changeState(FEEDER_MOVING);
//         //     Serial.println("Moteur principal: rotation arrière");
//         // }

//         // void stop() {
//         //     mainMotor.stop();
//         //     changeState(FEEDER_MOVING);
//         //     Serial.println("Moteur principal: arrêt");
//         // }

//         // void followRoute(Route route) {
//         //     currentRoutePtr = &route;

//         //     if (isDocked()) {
//         //         if (route.initialDirection == MOVING_FORWARD) {
//         //             moveForward();
//         //         } else {
//         //             moveBackward();
//         //         }
//         //     } else {
//         //         Serial.println("ERREUR: le robot doit être arrêté au dock avant de commencer la route !");
//         //     }
//         // }

//         void didUpdateLocation(RailPoint railPoint) {
//         //     if (railPoint.isReverse()) {
//         //         stopFeeding();
//         //         mainMotor.inverseMovingDirection();
//         //     }

//         //     // Ignore rail points in opposite direction
//         //     if (mainMotor.state == MOVING_FORWARD && railPoint.id % 2 != 0 
//         //      || mainMotor.state == MOVING_BACKWARD && railPoint.id % 2 == 0) {
//         //         return;
//         //     }

//         //     if (currentRoutePtr != NULL) {
//         //         Route currentRoute = Route(* currentRoutePtr);
//         //         RailPoint lastPoint = RailPoint(* lastPointPtr);

//         //         if (railPoint.id == currentRoute.endPoint.id && railPoint.id != lastPoint.id) {
//         //             completeRoute();
//         //         }
//         //     }

//         //     lastPointPtr = &railPoint;

//         //     notifyLocationObservers(railPoint);
//         }

//         // bool isSafetyBarPressed() {
//         //     return (mainMotor.state == MOVING_FORWARD && digitalRead(safetySensorFront) == HIGH) 
//         //         || (mainMotor.state == MOVING_BACKWARD && digitalRead(safetySensorBack) == HIGH);
//         // }

//         // bool isDocked() {
//         //     RailPoint activePoint = locationService.getActiveRailPoint();
//         //     return state == FEEDER_IDLE && activePoint.isDock();
//         // }

//         // bool hasCurrentRoute() {
//         //     return currentRoutePtr != NULL;
//         // }

//         // void checkSafetyState() {
//         //     if (state != FEEDER_SAFETY_STOP && isSafetyBarPressed()) {
//         //         Serial.println("Barre de sécurité enclenchée");

//         //         // Shutdown everything immediately
//         //         safetyStop();
//         //     } else if (state == FEEDER_SAFETY_STOP && !isSafetyBarPressed()) {
//         //         // Note: we should wait for manual reactivation or a timeout before reactivating automatically
//         //         // TODO: Reactivate robot
//         //     }
//         // }

//         // void safetyStop() {
//         //     mainMotor.stop();
//         //     stopFeeding();
            
//         //     changeState(FEEDER_SAFETY_STOP);

//         //     Serial.println("(!) Arrêt d'urgence");
//         // }

//         // void emergencyStop() {
//         //     mainMotor.stop();
//         //     stopFeeding();
            
//         //     changeState(FEEDER_SAFETY_STOP);

//         //     Serial.println("(!) Arrêt d'urgence");
//         // }

//         // int getMovingDirection() {
//         //     return mainMotor.state;
//         // }

//         // void stopFeeding() {
//         //     // Shutdown conveyors motor
//         //     conveyorFront.stop();
//         //     conveyorBack.stop();

//         //     Serial.println("Convoyeurs: arrêt");
//         // }

//         void setLight(LightColor lightColor, bool blinking) {
//             if (lightColor == GREEN) {
//                 digitalWrite(GREEN_LIGHT, HIGH);
//                 digitalWrite(RED_LIGHT, LOW);
                
//                 Serial.println("Lumière VERTE allumée");
//             } else {
//                 digitalWrite(GREEN_LIGHT, LOW);
//                 digitalWrite(RED_LIGHT, HIGH);

//                 Serial.println("Lumière ROUGE allumée");
//             }
//         }

//     private:
//         RailMotor mainMotor;
//         // PinConfig greenLight;
//         // PinConfig redLight;
//         // PinConfig safetySensorFront;
//         // PinConfig safetySensorBack;
//         //FeederState previousState = FEEDER_IDLE;
//         // Route *currentRoutePtr = NULL;
//         // RailPoint *lastPointPtr = NULL;

//         // void changeState(FeederState newState) {
//         //     previousState = state;
//         //     state = newState;

//         //     if (state == FEEDER_MOVING || state == FEEDER_REFILLING) {
//         //         setLight(GREEN, false);
//         //     } else if (state == FEEDER_SAFETY_STOP) {
//         //         setLight(RED, true);
//         //     }
//         // }
// };

// #endif