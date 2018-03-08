#include "rail_point.h"

#ifndef ROUTE_H
#define ROUTE_H

using namespace std;

class Route {
    public:
        int id;
        int initialDirection;
        RailPoint startPoint;
        RailPoint endPoint;

        Route(
            int routeId,
            int routeInitialDirection, 
            RailPoint routeStartPoint, 
            RailPoint routeEndPoint
        ) : 
            id(routeId),
            initialDirection(routeInitialDirection),
            startPoint(routeStartPoint),
            endPoint(routeEndPoint)
        {}  
};

Route getRouteById(vector<Route> routes, int routeId) {
    vector<Route>::iterator iterator = find_if (routes.begin(), routes.end(), [&](const Route & route) {
        return route.id == routeId;
    });

    return *iterator;
}

#endif