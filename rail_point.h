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
            int pointId, 
            const char *pointName,
            const char *pointRfidUid
        ) : 
            id(pointId),
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

RailPoint* getRailPointFromRfid(std::vector<RailPoint> railPoints, const char *rfidUid, MovingDirection direction) {
    std::vector<RailPoint>::iterator iterator = std::find_if (railPoints.begin(), railPoints.end(), [&](const RailPoint & point) {
        return point.rfidUid == rfidUid 
            && ((direction == MOVING_FORWARD && point.id % 2 == 0) 
            || (direction == MOVING_BACKWARD && point.id % 2 != 0));
    });

    return &*iterator;
}

#endif