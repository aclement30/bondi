#include "diagnostic_service.h"
#include "conveyor_motor.h"

#ifndef DIAGNOSTIC_CONVEYOR_H
#define DIAGNOSTIC_CONVEYOR_H

using namespace std;

class ConveyorDiagnosticService: public DiagnosticService {
    public:
        ConveyorDiagnosticService(
            ConveyorMotor &conveyorRef,
            const char * name
        );

        void startDiagnostic();
        void continueDiagnostic();
        void abortDiagnostic();
        string getTitle();
        void displaySide(ConveyorSide side);
        void displaySpeed(int speed);

    private:
        ConveyorMotor &conveyorMotor;
        const char * conveyorName;
        int currentStep = 0;

        void displayDiagnosticScreen();
};

#endif