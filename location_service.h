#include "rail_point.h"
#include "rfid_reader.h"
#include "route.h"

#ifndef LOCATIONSERVICE_H
#define LOCATIONSERVICE_H

using namespace std;

class LocationAware {
    public:
        virtual void didUpdateLocation(RailPoint railPoint) = 0;
};

class LocationService {
    public:
        RfidReader rfidReader;
        vector<RailPoint> railPoints;
        vector<Route> routes;

        RailPoint *activeRailPointPtr = NULL;

        LocationService(
            RfidReader feederRfidReader,
            vector<RailPoint> feederRailPoints,
            vector<Route> feederRoutes
        ) : 
            rfidReader(feederRfidReader),
            railPoints(feederRailPoints),
            routes(feederRoutes)
        {
            RailPoint railPoint = getDockPoint(railPoints);
            activeRailPointPtr = &railPoint;
        }

        RailPoint getActiveRailPoint() {
            return RailPoint(* activeRailPointPtr);
        }

        void refreshActivePoint() {
            const char *uid = rfidReader.read();

            if (strlen(uid) != 0) {
                // New rail point detected: match RFID uid with corresponding rail point
                RailPoint railPoint = getRailPointByRfidUId(railPoints, uid);
                activeRailPointPtr = &railPoint;

                char message[] = "Point actif: ";
                Serial.println(strcat(message, railPoint.name));

                notifyObservers(railPoint);
            }

            // If UID is empty, no new RFID tag has been scanned, so we keep the active one
        }

        void notifyObservers(RailPoint railPoint) {
            for (int i = 0; i < observers.size(); i++) {
                observers[i]->didUpdateLocation(railPoint);
            }
        }

        void subscribe(LocationAware *observer) {
            observers.push_back(observer);
        }

    private:
        vector <class LocationAware *> observers;
};

#endif