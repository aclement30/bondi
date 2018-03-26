#include "diagnostic_service.h"
#include "display_service.h"
#include "keypad_service.h"
#include "location_service.h"
#include "navigation_menu.h"
#include "route.h"
#include "string.h"
#include "route_mapping_diagnostic_service.h"

using namespace std;

RouteMappingDiagnosticService::RouteMappingDiagnosticService(LocationService &locationServiceRef) : 
    locationService(locationServiceRef),
    routes(locationServiceRef.routes)
{}

void RouteMappingDiagnosticService::startDiagnostic() {
    completed = false;
    
    if (!locationService.isDocked()) {
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

    displayDiagnosticScreen();

    Route &currentRoute = routes.at(currentRouteIndex);
    displayCurrentRoute(currentRoute.id);
    locationService.followRoute(currentRoute.id);
}

void RouteMappingDiagnosticService::continueDiagnostic() {
    if (locationService.activeRailPointPtr != NULL) {
        displayCurrentPoint(locationService.activeRailPointPtr->name);
    }

    // Let feeder continue on its current route
    if (locationService.isFollowingRoute()) {
        return;
    }

    currentRouteIndex += 1;

    if (currentRouteIndex < routes.size()) {
        Route &currentRoute = routes.at(currentRouteIndex);
        displayCurrentRoute(currentRoute.id);
        locationService.followRoute(currentRoute.id);
    } else {
        Serial.println(F("* complete diagnostic"));
        completed = true;
        currentRouteIndex = 0;
    }
}

void RouteMappingDiagnosticService::abortDiagnostic() {
    locationService.unfollowRoute();
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