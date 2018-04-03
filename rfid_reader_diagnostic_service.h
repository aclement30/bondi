#include "diagnostic_service.h"
#include "location_service.h"

#ifndef RFID_READER_DIAGNOSTIC_SERVICE_H
#define RFID_READER_DIAGNOSTIC_SERVICE_H

using namespace std;

class RfidReaderDiagnosticService: public DiagnosticService {
    public:
        RfidReaderDiagnosticService();

        void startDiagnostic();
        void continueDiagnostic();
        void abortDiagnostic();
        string getTitle();

    private:
        char lastRfidUid[20];

        void displayDiagnosticScreen();
        void displayRailPoint(const char * name);
        void displayUid(const char * uid);
        void displayWaitingText();
};

#endif