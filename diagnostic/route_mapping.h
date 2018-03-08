#include "diagnostic_service.h"
#include "../feeder.h"
#include "../route.h"

#ifndef DIAGNOSTIC_ROUTEMAPPING_H
#define DIAGNOSTIC_ROUTEMAPPING_H

using namespace std;

class RouteMappingDiagnosticService: public DiagnosticService {
    public:
        RouteMappingDiagnosticService(
            Feeder diagnosticFeeder,
            vector<Route> diagnosticRoutes
        ) : 
            feeder(diagnosticFeeder),
            routes(diagnosticRoutes),
            currentRoute(diagnosticRoutes.front())
        {}


    void startDiagnostic() {
        //observer = observer;

        if (!feeder.isDocked()) {
            Serial.println("Error: feeder must be stopped at dock station before starting diagnostic!");
            return;
        }

        feeder.followRoute(currentRoute);
    }

    void continueDiagnostic() {
        // Let feeder continue on its current route
        if (feeder.hasCurrentRoute()) {
            return;
        }

        int index = distance(routes.begin(), find(routes.begin(), routes.end(), [&](const Route & route) {
            return route.id == currentRoute.id;
        }));

        if (index < routes.size()) {
            currentRoute = routes.at(index + 1);
            feeder.followRoute(currentRoute);
        } else {
            Serial.println("Diagnostic completed!");
        }
    }

    // void completeDiagnostic() {
    //     observer.didCompleteDiagnostic();
    // }

    private:
        Feeder &feeder;
        vector<Route> &routes;
        Route &currentRoute;
};

#endif