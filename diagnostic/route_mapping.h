#include "diagnostic_service.h"
#include "../display_service.h"
#include "../navigation_menu.h"
#include "../location_service.h"
#include "../route.h"

#ifndef DIAGNOSTIC_ROUTEMAPPING_H
#define DIAGNOSTIC_ROUTEMAPPING_H

using namespace std;

class RouteMappingDiagnosticService: public DiagnosticService {
    public:
        RouteMappingDiagnosticService(
            LocationService &locationServiceRef
        ) : 
            locationService(locationServiceRef),
            routes(locationServiceRef.routes)
        {}

        void startDiagnostic() {
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

        void continueDiagnostic() {
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

        void abortDiagnostic() {
            locationService.unfollowRoute();
        }

        string getTitle() {
            return "DIAGNO. LISTE ROUTES";
        }

        void displayCurrentRoute(int routeId) {
            char lineText[20];
            sprintf(lineText, "Route: %d", routeId);
            DisplayService::getInstance().print(lineText, 2);
        }

        void displayCurrentPoint(const char *name) {
            char message[20] = "Point: ";
            DisplayService::getInstance().print(strcat(message, name), 3);
        }

    private:
        LocationService &locationService;
        vector<Route> &routes;
        int currentRouteIndex = 0;

        void displayDiagnosticScreen() {
            DisplayService::getInstance().clearScreen();
            DisplayService::getInstance().printTitle(getTitle());
            DisplayService::getInstance().addBorder();
        }
};

#endif