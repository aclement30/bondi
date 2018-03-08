#include "diagnostic_service.h"
#include "../feeder.h"
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
            //observer = observer;

            if (!feeder.isDocked()) {
                Serial.println("ERREUR: le robot doit être arrêté au dock avant de commencer le diagnostic !");
                return;
            }

            feeder.followRoute(routes.front());
        }

        void continueDiagnostic() {
            // Let feeder continue on its current route
            if (feeder.hasCurrentRoute()) {
                return;
            }

            advance(routeIterator, 1);

            if (routeIterator != routes.end()) {
                Route currentRoute = Route(*routeIterator);
                feeder.followRoute(currentRoute);
            } else {
                completed = true;
            }
        }

    private:
        Feeder &feeder;
        vector<Route> &routes;
        Route *routeIterator;
};

#endif