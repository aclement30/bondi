#ifndef __EMSCRIPTEN__
#include <ArduinoSTL.h>
#endif

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
    std::vector<RailPoint>::iterator iterator = std::find_if (railPoints.begin(), railPoints.end(), [&](const RailPoint & point) {
        return point.id == pointId;
    });

    return *iterator;
}

RailPoint getRailPointByRfidUId(std::vector<RailPoint> railPoints, const char *rfidUid) {
    std::vector<RailPoint>::iterator iterator = std::find_if (railPoints.begin(), railPoints.end(), [&](const RailPoint & point) {
        return point.rfidUid == rfidUid;
    });

    return *iterator;
}

#endif