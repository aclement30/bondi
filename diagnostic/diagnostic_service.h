//#include "conveyor_motor.h"
//#include "feeder.h"
//#include "rail_motor.h"
//#include "route.h"

#ifndef DIAGNOSTICSERVICE_H
#define DIAGNOSTICSERVICE_H

class DiagnosticAware {
    public:
        virtual void didCompleteDiagnostic() = 0;
};

using namespace std;

class DiagnosticService {
    public:
        virtual void startDiagnostic() = 0;
        virtual void continueDiagnostic() = 0;

        // DiagnosticService(
        //     RfidReader feederRfidReader,
        //     vector<RailPoint> feederRailPoints,
        //     vector<Route> feederRoutes
        // ) : 
        //     rfidReader(feederRfidReader),
        //     railPoints(feederRailPoints),
        //     routes(feederRoutes)
        // {}
    
        // void testConveyorMotor(ConveyorMotor motor) {
        //     motor.stop();
        //     delay(1000);

        //     Serial.println("Starting conveyor: LEFT | 100");
        //     motor.start(CONVEYOR_SIDE_LEFT, 100);

        //     delay(2000);

        //     Serial.println("Testing conveyor: LEFT | 200");
        //     motor.start(CONVEYOR_SIDE_LEFT, 200);

        //     delay(2000);

        //     Serial.println("Testing conveyor: LEFT | 300");
        //     motor.start(CONVEYOR_SIDE_LEFT, 300);

        //     delay(2000);

        //     Serial.println("Testing conveyor: LEFT | 400");
        //     motor.start(CONVEYOR_SIDE_LEFT, 400);

        //     delay(2000);

        //     Serial.println("Stopping conveyor");
        //     motor.stop();

        //     delay(1000);

        //     Serial.println("Starting conveyor: RIGHT | 100");
        //     motor.start(CONVEYOR_SIDE_RIGHT, 100);

        //     delay(2000);

        //     Serial.println("Testing conveyor: RIGHT | 200");
        //     motor.start(CONVEYOR_SIDE_RIGHT, 200);

        //     delay(2000);

        //     Serial.println("Testing conveyor: RIGHT | 300");
        //     motor.start(CONVEYOR_SIDE_RIGHT, 300);

        //     delay(2000);

        //     Serial.println("Testing conveyor: RIGHT | 400");
        //     motor.start(CONVEYOR_SIDE_RIGHT, 400);

        //     delay(2000);

        //     Serial.println("Stopping conveyor");
        //     motor.stop();
        // }

        // void testRailMotor(RailMotor motor) {
        //     motor.stop();
        //     delay(1000);

        //     Serial.println("Starting rail motor: FORWARD");
        //     motor.moveForward();

        //     delay(4000);

        //     Serial.println("Stopping motor");
        //     motor.stop();

        //     delay(1000);

        //     Serial.println("Starting rail motor: BACKWARD");
        //     motor.moveBackward();

        //     delay(4000);

        //     Serial.println("Stopping motor");
        //     motor.stop();
        // }

    // private:
    //     DiagnosticMode _diagnosticMode = None;
    //     Route *currentMappedRoutePtr = NULL;

        bool isCompleted() {
            return completed;
        }

    protected:
        //DiagnosticAware &observer;
        bool completed = false;
};

#endif