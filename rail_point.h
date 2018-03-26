#include <ArduinoSTL.h>
#include "constants.h"

#ifndef RAIL_POINT_H
#define RAIL_POINT_H

using namespace std;

// TODO: Transform into struct

struct RailPoint {
    int id;
    const char * name;
    const char * rfidUid;

    RailPoint(
        int railPointId, 
        const char * pointName,
        const char * pointRfidUid
    );

    bool isDock();
    bool isReverse();
};

// TODO: Refactor these functions as static methods in RailPoint class
RailPoint getRailPointById(vector<RailPoint> railPoints, int pointId);
RailPoint getDockPoint(vector<RailPoint> railPoints);
int getRailPointIndexFromRfid(vector<RailPoint> railPoints, const char *rfidUid, MovingDirection direction);

#endif
