#include <ArduinoSTL.h>
#include "rail_point.h"
#include "route.h"

#ifndef ROUTE_SERVICE_H
#define ROUTE_SERVICE_H

using namespace std;

class RouteService {
    public:
        RailPoint * activeRailPointPtr = NULL;
        Route * currentRoutePtr = NULL;

        RouteService(int routeId);
        ~RouteService();

        void start();
        void stop();
        void refreshLocation();
        bool isCompleted();

    private:
        char lastRfidUid[20] = {};
        int checkpointsCount = 0;
};

#endif
