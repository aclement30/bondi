#ifndef DIAGNOSTICSERVICE_H
#define DIAGNOSTICSERVICE_H

using namespace std;

class DiagnosticService {
    public:
        virtual ~DiagnosticService() = default;

        virtual void startDiagnostic() = 0;
        virtual void continueDiagnostic() = 0;
        virtual void abortDiagnostic() = 0;
        virtual string getTitle() = 0;

        bool isCompleted() {
            return completed;
        }

        bool isCancelled() {
            return cancelled;
        }

    protected:
        bool completed = false;
        bool cancelled = false;
};

#endif