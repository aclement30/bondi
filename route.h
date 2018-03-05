#include "rail_point.h"

#ifndef ROUTE_H
#define ROUTE_H

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

Route getRouteById(std::vector<Route> routes, int routeId) {
    std::vector<Route>::iterator iterator = std::find_if (routes.begin(), routes.end(), [&](const Route & route) {
        return route.id == routeId;
    });

    return *iterator;
}

#endif