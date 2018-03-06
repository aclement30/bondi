#include "rail_point.h"
#include "rfid_reader.h"
#include "route.h"

#ifndef LOCATIONSERVICE_H
#define LOCATIONSERVICE_H

class LocationService {
    public:
        RfidReader rfidReader;
        std::vector<RailPoint> railPoints;
        std::vector<Route> routes;

        RailPoint *activeRailPointPtr = NULL;

        LocationService(
            RfidReader feederRfidReader,
            std::vector<RailPoint> feederRailPoints,
            std::vector<Route> feederRoutes
        ) : 
            rfidReader(feederRfidReader),
            railPoints(feederRailPoints),
            routes(feederRoutes)
        {}

        RailPoint getActiveRailPoint() {
            return RailPoint(* activeRailPointPtr);
        }

        void refreshActivePoint() {
            const char *uid = rfidReader.read();

            if (strlen(uid) != 0) {
                // New rail point detected: match RFID uid with corresponding rail point
                RailPoint railPoint = getRailPointByRfidUId(railPoints, uid);
                activeRailPointPtr = &railPoint;
            }

            // If UID is empty, no new RFID tag has been scanned, so we keep the active one
        }
};

#endif