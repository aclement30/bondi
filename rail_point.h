#ifndef __EMSCRIPTEN__
#include <ArduinoSTL.h>
#endif

#include "constants.h"

#ifndef RAILPOINT_H
#define RAILPOINT_H

class RailPoint {
    public:
        int pointId;
        const char *name;
        const char *rfidUid;

        RailPoint(
            int railPointId, 
            const char *pointName,
            const char *pointRfidUid
        ) : 
            pointId(railPointId),
            name(pointName),
            rfidUid(pointRfidUid)
        {}

        bool isDock() {
            return pointId == 1000;
        }

        bool isReverse() {
            return pointId >= 100 && pointId < 200;
        }
};

RailPoint getRailPointById(std::vector<RailPoint> railPoints, int pointId) {
    std::vector<RailPoint>::iterator iterator = std::find_if (railPoints.begin(), railPoints.end(), [&](RailPoint &point) {
        return point.pointId == pointId;
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
                    (direction == MOVING_FORWARD && point.pointId % 2 == 0) || 
                    (direction == MOVING_BACKWARD && point.pointId % 2 != 0)
                ));
    });

    if (iterator == railPoints.end()) {
        return -1;
    }
    
    return iterator - railPoints.begin();
}

#endif
