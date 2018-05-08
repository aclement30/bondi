#include "diagnostic_service.h"
#include "display_service.h"
#include "keypad_service.h"
#include "navigation_menu.h"
#include "string.h"
#include "time_service.h"
#include "time_diagnostic_service.h"

using namespace std;

TimeDiagnosticService::TimeDiagnosticService() {}

void TimeDiagnosticService::startDiagnostic() {
    displayDiagnosticScreen();
}

void TimeDiagnosticService::continueDiagnostic() {
    displayDiagnosticScreen();
}

void TimeDiagnosticService::abortDiagnostic() {}

string TimeDiagnosticService::getTitle() {
    const static char title[] PROGMEM = "DATE & HEURE";
    return getString(title);
}

// PRIVATE

void TimeDiagnosticService::displayDiagnosticScreen() {
    char dateTime[50];
    TimeService::getInstance().getLocalDateTime(dateTime);

    DisplayService::getInstance().clearScreen();
    DisplayService::getInstance().printTitle(getTitle());
    DisplayService::getInstance().addBorder();
    DisplayService::getInstance().print(dateTime, 2);

    KeypadService::getInstance().waitForActivity(1000);
}