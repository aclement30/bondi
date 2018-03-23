#include "diagnostic_service.h"
#include "location_service.h"
#include "route.h"

#ifndef ROUTE_MAPPING_DIAGNOSTIC_SERVICE_H
#define ROUTE_MAPPING_DIAGNOSTIC_SERVICE_H

using namespace std;

class RouteMappingDiagnosticService: public DiagnosticService {
    public:
        RouteMappingDiagnosticService(LocationService & locationServiceRef);

        void startDiagnostic();
        void continueDiagnostic();
        void abortDiagnostic();
        string getTitle();
        void displayCurrentRoute(int routeId);
        void displayCurrentPoint(const char *name);

    private:
        LocationService & locationService;
        vector<Route> & routes;
        int currentRouteIndex = 0;

        void displayDiagnosticScreen();
};

#endif