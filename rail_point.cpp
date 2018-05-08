#include <Arduino.h>
#include <ArduinoSTL.h>
#include "constants.h"
#include "rail_point.h"

using namespace std;

RailPoint::RailPoint(
    int railPointId, 
    const char * pointName,
    const char * pointRfidUid
) {
    id = railPointId;
    name = pointName;
    rfidUid = pointRfidUid;
}

bool RailPoint::isDock() {
    return id == 1000;
}

bool RailPoint::isReverse() {
    return id >= 100 && id < 200;
}

MovingDirection RailPoint::getMovingDirection() {
    if (isDock()) {
        return MOVING_IDLE;
    }

    if (isReverse()) {
        return id % 2 == 0 ? MOVING_BACKWARD : MOVING_FORWARD;
    }

    return id % 2 == 0 ? MOVING_FORWARD : MOVING_BACKWARD;
}

RailPoint getRailPointById(vector<RailPoint> railPoints, int pointId) {
    vector<RailPoint>::iterator iterator = find_if (railPoints.begin(), railPoints.end(), [&](RailPoint &point) {
        return point.id == pointId;
    });

    return *iterator;
}

RailPoint getDockPoint(vector<RailPoint> railPoints) {
    vector<RailPoint>::iterator iterator = find_if (railPoints.begin(), railPoints.end(), [&](RailPoint &point) {
        return point.isDock();
    });

    return *iterator;
}

int getRailPointIndexFromRfid(vector<RailPoint> railPoints, const char * rfidUid, MovingDirection direction) {
    vector<RailPoint>::iterator iterator = find_if (railPoints.begin(), railPoints.end(), [&](RailPoint & point) {
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

bool railPointExists(vector<RailPoint> railPoints, int railPointId) {
    vector<RailPoint>::iterator iterator = find_if (railPoints.begin(), railPoints.end(), [&](RailPoint &point) {
        return point.id == railPointId;
    });

    return iterator != railPoints.end();
}

bool dockPointExists(vector<RailPoint> railPoints) {
    vector<RailPoint>::iterator iterator = find_if (railPoints.begin(), railPoints.end(), [&](RailPoint &point) {
        return point.isDock();
    });

    return iterator != railPoints.end();
}
