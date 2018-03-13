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
            routes(locationServiceRef.routes),
            routeIterator(locationServiceRef.routes.begin())
        {}

        void startDiagnostic() {
            displayDiagnosticScreen();

            if (!locationService.isDocked()) {
                vector<string> errorMessage = {
                    "Le robot doit etre",
                    "positionne au dock."
                };
                DisplayService::getInstance().showErrorScreen(errorMessage, string("OK"));
                NavigationMenu::waitForConfirmation();
                
                cancelled = true;
                return;
            }

            Route &currentRoute = routes.front();
            displayCurrentRoute(currentRoute.id);
            locationService.followRoute(routes.front());
        }

        void continueDiagnostic() {
            displayDiagnosticScreen();

            // Let feeder continue on its current route
            if (locationService.isFollowingRoute()) {
                return;
            }

            advance(routeIterator, 1);

            if (routeIterator != routes.end()) {
                Route currentRoute = *routeIterator;
                displayCurrentRoute(currentRoute.id);
                locationService.followRoute(currentRoute);
            } else {
                completed = true;
            }
        }

        string getTitle() {
            return "DIAGNO. LISTE ROUTES";
        }

        void displayCurrentRoute(int routeId) {
            char lineText[20];
            sprintf(lineText, "Route: %d", routeId);
            DisplayService::getInstance().print(lineText, 2);
        }

        void displayCurrentPoint(string &name) {
            DisplayService::getInstance().print("Point: " + name, 3);
        }

    private:
        LocationService &locationService;
        vector<Route> &routes;
        Route *routeIterator;

        void displayDiagnosticScreen() {
            DisplayService::getInstance().clearScreen();
            DisplayService::getInstance().printTitle(getTitle());
            DisplayService::getInstance().addBorder();
        }
};

#endif