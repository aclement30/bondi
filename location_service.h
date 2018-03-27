#include <ArduinoSTL.h>
#include <RFID.h>

#include "rail_point.h"
#include "route.h"

#ifndef LOCATION_SERVICE_H
#define LOCATION_SERVICE_H

class LocationService {
    public:
        vector<RailPoint> & railPoints;
        vector<Route> & routes;
        RailPoint * activeRailPointPtr = NULL;
        Route * currentRoutePtr = NULL;

        LocationService(vector<RailPoint> &railPointsRef, vector<Route> &routesRef);

        void setup();
        void refreshActivePoint();
        void followRoute(int routeId);
        void unfollowRoute();
        bool isFollowingRoute();
        bool isDocked();
        bool readRfidPoint(char * uid);

    private:
        RFID rfid;
        char lastRfidUid[20] = {};

        void completeRoute();
};

#endif
