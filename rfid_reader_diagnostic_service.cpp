#include "config.h"
#include "display_service.h"
#include "keypad_service.h"
#include "location_service.h"
#include "rail_point.h"
#include "string.h"
#include "rfid_reader_diagnostic_service.h"

using namespace std;

RfidReaderDiagnosticService::RfidReaderDiagnosticService() {}

void RfidReaderDiagnosticService::startDiagnostic() {
    completed = false;
    
    displayDiagnosticScreen();
}

void RfidReaderDiagnosticService::continueDiagnostic() {
    KeypadService::getInstance().waitForActivity(1000);

    char uid[20];
    bool hasRfidUid = LocationService::getInstance().readRfidUid(uid);

    if (!hasRfidUid) {
        displayWaitingText();
        strcpy(lastRfidUid, "");
        return;
    }

    // Skip if UID hasn't changed
    if (strcmp(lastRfidUid, uid) == 0) {
        return;
    }

    displayUid(uid);
    strcpy(lastRfidUid, uid);
    
    int pointIndex = getRailPointIndexFromRfid(AppConfig::getInstance().railPoints, uid, MOVING_IDLE);
    if (pointIndex == -1) {
        const static char unknownPoint[] PROGMEM = "?";
        displayRailPoint(getString(unknownPoint));
        return;
    }

    RailPoint & railPoint = AppConfig::getInstance().railPoints.at(pointIndex);
    displayRailPoint(railPoint.name);
}

void RfidReaderDiagnosticService::abortDiagnostic() {}

string RfidReaderDiagnosticService::getTitle() {
    const static char title[] PROGMEM = "TEST LECTEUR RFID";
    return getString(title);
}

// PRIVATE

void RfidReaderDiagnosticService::displayDiagnosticScreen() {
    DisplayService::getInstance().clearScreen();
    DisplayService::getInstance().printTitle(getTitle());
    DisplayService::getInstance().addBorder();
}

void RfidReaderDiagnosticService::displayRailPoint(const char * name) {
    char message[20] = "Point: ";
    DisplayService::getInstance().print(strcat(message, name), 2);
}

void RfidReaderDiagnosticService::displayUid(const char * uid) {
    DisplayService::getInstance().print(uid, 3);
}

void RfidReaderDiagnosticService::displayWaitingText() {
    const static char waitingText[] PROGMEM = "En attente";
    DisplayService::getInstance().printCenter(getString(waitingText), 2);
    DisplayService::getInstance().clearLine(3);
}