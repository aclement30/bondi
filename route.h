#include "rail_point.h"

#ifndef ROUTE_H
#define ROUTE_H

class Route {
    public:
        const int initialDirection;
        const RailPoint startPoint;
        const RailPoint endPoint;

        Route(
            int routeInitialDirection, 
            RailPoint routeStartPoint, 
            RailPoint routeEndPoint
        ) : 
            initialDirection(routeInitialDirection),
            startPoint(routeStartPoint),
            endPoint(routeEndPoint)
        {}  
};

#endif