#include "rail_point.h"

#ifndef TRIP_H
#define TRIP_H

class Trip {
    public:
        const int initialDirection;
        const RailPoint startPoint;
        const RailPoint endPoint;

        Trip(
            int tripInitialDirection, 
            RailPoint tripStartPoint, 
            RailPoint tripEndPoint
        ) : 
            initialDirection(tripInitialDirection),
            startPoint(tripStartPoint),
            endPoint(tripEndPoint)
        {}  
};

#endif