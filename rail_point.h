#ifndef __EMSCRIPTEN__
#include <ArduinoSTL.h>
#endif

#include "constants.h"

#ifndef RAILPOINT_H
#define RAILPOINT_H

class RailPoint {
    public:
        int id;
        const char *name;
        const char *rfidUid;

        RailPoint(
            int railPointId, 
            const char *pointName,
            const char *pointRfidUid
        ) : 
            id(railPointId),
            name(pointName),
            rfidUid(pointRfidUid)
        {}

        bool isDock() {
            return id == 1000;
        }

        bool isReverse() {
            return id >= 100 && id < 200;
        }
};

RailPoint getRailPointById(std::vector<RailPoint> railPoints, int pointId) {
    std::vector<RailPoint>::iterator iterator = std::find_if (railPoints.begin(), railPoints.end(), [&](RailPoint &point) {
        return point.id == pointId;
    });

    return *iterator;
}

RailPoint getDockPoint(std::vector<RailPoint> railPoints) {
    std::vector<RailPoint>::iterator iterator = std::find_if (railPoints.begin(), railPoints.end(), [&](RailPoint &point) {
        return point.isDock();
    });

    return *iterator;
}

int getRailPointIndexFromRfid(std::vector<RailPoint> railPoints, const char *rfidUid, MovingDirection direction) {
    std::vector<RailPoint>::iterator iterator = std::find_if (railPoints.begin(), railPoints.end(), [&](RailPoint & point) {
        return (strcmp(point.rfidUid, rfidUid) == 0 && 
                (
                    point.isDock() || 
                    direction == MOVING_IDLE || 
                    (direction == MOVING_FORWARD && point.id % 2 == 0) || 
                    (direction == MOVING_BACKWARD && point.id % 2 != 0)
                ));
    });

    if (iterator == railPoints.end()) {
        return -1;
    }
    
    return iterator - railPoints.begin();
}

#endif
