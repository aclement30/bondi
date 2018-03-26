#include "constants.h"

#ifndef ROUTE_H
#define ROUTE_H

using namespace std;

struct Route {
    int id;
    MovingDirection initialDirection;
    int startPointId;
    int endPointId;

    Route(
        int routeId,
        MovingDirection routeInitialDirection, 
        int routeStartPointId, 
        int routeEndPointId
    );
};

int getRouteIndexById(vector<Route> routes, int routeId);

#endif