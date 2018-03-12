#include "diagnostic_service.h"
#include "../display_service.h"
#include "../feeder.h"
#include "../navigation_menu.h"
#include "../route.h"

#ifndef DIAGNOSTIC_ROUTEMAPPING_H
#define DIAGNOSTIC_ROUTEMAPPING_H

using namespace std;

class RouteMappingDiagnosticService: public DiagnosticService {
    public:
        RouteMappingDiagnosticService(
            Feeder &diagnosticFeeder,
            vector<Route> &diagnosticRoutes
        ) : 
            feeder(diagnosticFeeder),
            routes(diagnosticRoutes),
            routeIterator(diagnosticRoutes.begin())
        {}

        void startDiagnostic() {
            displayDiagnosticScreen();

            if (!feeder.isDocked()) {
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
            feeder.followRoute(routes.front());
        }

        void continueDiagnostic() {
            displayDiagnosticScreen();

            // Let feeder continue on its current route
            if (feeder.hasCurrentRoute()) {
                return;
            }

            advance(routeIterator, 1);

            if (routeIterator != routes.end()) {
                Route currentRoute = *routeIterator;
                displayCurrentRoute(currentRoute.id);
                feeder.followRoute(currentRoute);
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
        Feeder &feeder;
        vector<Route> &routes;
        Route *routeIterator;

        void displayDiagnosticScreen() {
            DisplayService::getInstance().clearScreen();
            DisplayService::getInstance().printTitle(getTitle());
            DisplayService::getInstance().addBorder();
        }
};

#endif