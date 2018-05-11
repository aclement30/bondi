#include "config.h"
#include "diagnostic_service.h"
#include "display_service.h"
#include "keypad_service.h"
#include "location_service.h"
#include "navigation_menu.h"
#include "route.h"
#include "route_service.h"
#include "safety_service.h"
#include "string.h"
#include "route_mapping_diagnostic_service.h"

using namespace std;

RouteMappingDiagnosticService::RouteMappingDiagnosticService() {}

RouteMappingDiagnosticService::~RouteMappingDiagnosticService() {
    if (routeServicePtr != NULL) {
        delete routeServicePtr;
    }
}

void RouteMappingDiagnosticService::startDiagnostic() {
    completed = false;
    
    if (!LocationService::getInstance().isDocked()) {
        const static char errorMsg1[] PROGMEM = "Le robot doit etre";
        const static char errorMsg2[] PROGMEM = "positionne au dock.";
        const static char okBtn[] PROGMEM = "OK";

        vector<string> errorMessage = {
            getString(errorMsg1),
            getString(errorMsg2)
        };
        DisplayService::getInstance().showErrorScreen(errorMessage, getString(okBtn));
        KeypadService::getInstance().waitForConfirmation();
        
        cancelled = true;
        return;
    }

    // Display short warning notice before moving
    SafetyService::getInstance().displayMovingWarning();

    displayDiagnosticScreen();

    int routeId = AppConfig::getInstance().routes.at(currentRouteIndex).id;
    routeServicePtr = new RouteService(routeId);
    routeServicePtr->start();
    displayCurrentRoute(routeId);
}

void RouteMappingDiagnosticService::continueDiagnostic() {
    routeServicePtr->refreshLocation();

    if (routeServicePtr->activeRailPointPtr != NULL) {
        displayCurrentPoint(routeServicePtr->activeRailPointPtr->name);
    }

    // Let feeder continue on its current route
    if (!routeServicePtr->isCompleted()) {
        return;
    }

    currentRouteIndex += 1;

    delete routeServicePtr;
    routeServicePtr = NULL;

    if (currentRouteIndex < AppConfig::getInstance().routes.size()) {
        int routeId = AppConfig::getInstance().routes.at(currentRouteIndex).id;
        routeServicePtr = new RouteService(routeId);
        displayCurrentRoute(routeId);
        routeServicePtr->start();
    } else {
        // Serial.println(F("* complete diagnostic"));
        completed = true;
        currentRouteIndex = 0;
    }
}

void RouteMappingDiagnosticService::abortDiagnostic() {
    routeServicePtr->stop();
}

string RouteMappingDiagnosticService::getTitle() {
    const static char title[] PROGMEM = "DIAGNO. LISTE ROUTES";
    return getString(title);
}

void RouteMappingDiagnosticService::displayCurrentRoute(int routeId) {
    char lineText[20];
    sprintf(lineText, "Route: %d", routeId);
    DisplayService::getInstance().print(lineText, 2);
}

void RouteMappingDiagnosticService::displayCurrentPoint(const char *name) {
    char message[20] = "Point: ";
    DisplayService::getInstance().print(strcat(message, name), 3);
}

// PRIVATE

void RouteMappingDiagnosticService::displayDiagnosticScreen() {
    DisplayService::getInstance().clearScreen();
    DisplayService::getInstance().printTitle(getTitle());
    DisplayService::getInstance().addBorder();
}