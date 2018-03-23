#include <ArduinoSTL.h>

#ifndef DIAGNOSTIC_SERVICE_H
#define DIAGNOSTIC_SERVICE_H

using namespace std;

class DiagnosticService {
    public:
        virtual ~DiagnosticService() = default;

        virtual void startDiagnostic() = 0;
        virtual void continueDiagnostic() = 0;
        virtual void abortDiagnostic() = 0;
        virtual string getTitle() = 0;

        bool isCompleted();
        bool isCancelled();

    protected:
        bool completed = false;
        bool cancelled = false;
};

#endif