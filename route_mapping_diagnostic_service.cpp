#include "diagnostic_service.h"
#include "display_service.h"
#include "keypad_service.h"
#include "location_service.h"
#include "navigation_menu.h"
#include "route.h"
#include "route_mapping_diagnostic_service.h"

using namespace std;

RouteMappingDiagnosticService::RouteMappingDiagnosticService(LocationService &locationServiceRef) : 
    locationService(locationServiceRef),
    routes(locationServiceRef.routes)
{}

void RouteMappingDiagnosticService::startDiagnostic() {
    completed = false;
    
    if (!locationService.isDocked()) {
        vector<string> errorMessage = {
            "Le robot doit etre",
            "positionne au dock."
        };
        DisplayService::getInstance().showErrorScreen(errorMessage, string("OK"));
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
        Serial.println("* complete diagnostic");
        completed = true;
        currentRouteIndex = 0;
    }
}

void RouteMappingDiagnosticService::abortDiagnostic() {
    locationService.unfollowRoute();
}

string RouteMappingDiagnosticService::getTitle() {
    return "DIAGNO. LISTE ROUTES";
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