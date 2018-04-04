#include "controller.h"
#include "conveyor_motor.h"
#include "diagnostic_service.h"
#include "rail_motor.h"

#ifndef DIAGNOSTIC_CONTROLLER_H
#define DIAGNOSTIC_CONTROLLER_H

using namespace std;

class DiagnosticController: public Controller {
    public:
        DiagnosticController();
        ~DiagnosticController();
        
        void handle();
        void escape();
        void stopDiagnostic();
        void completeDiagnostic();

    private:
        DiagnosticService * diagnosticPtr = NULL;

        void displayNavMenu();
        void displayConfirmationScreen();
        void displayCompletionScreen();
};

#endif