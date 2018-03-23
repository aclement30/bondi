#include "diagnostic_service.h"
#include "rail_motor.h"

#ifndef RAIL_MOTOR_DIAGNOSTIC_SERVICE_H
#define RAIL_MOTOR_DIAGNOSTIC_SERVICE_H

using namespace std;

class RailMotorDiagnosticService: public DiagnosticService {
    public:
        RailMotorDiagnosticService(RailMotor &motorRef);

        void startDiagnostic();
        void continueDiagnostic();
        void abortDiagnostic();
        string getTitle();
        void displayDirection(MovingDirection direction);

    private:
        RailMotor &railMotor;
        int currentStep = 0;

        void displayDiagnosticScreen();
};

#endif