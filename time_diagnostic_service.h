#include "diagnostic_service.h"

#ifndef TIME_DIAGNOSTIC_SERVICE_H
#define TIME_DIAGNOSTIC_SERVICE_H

using namespace std;

class TimeDiagnosticService: public DiagnosticService {
    public:
        TimeDiagnosticService();

        void startDiagnostic();
        void continueDiagnostic();
        void abortDiagnostic();
        string getTitle();

    private:
        void displayDiagnosticScreen();
};

#endif