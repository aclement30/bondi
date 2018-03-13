#ifndef __EMSCRIPTEN__
#include <ArduinoSTL.h>
#include <string.h>
#include <sstream>
#include <SPI.h>
#include <RFID.h>
#endif

#include "constants.h"
#include "rail_point.h"
#include "route.h"
#include "state_manager.h"

#ifndef LOCATIONSERVICE_H
#define LOCATIONSERVICE_H

using namespace std;

class LocationAware {
    public:
        virtual void didUpdateLocation(RailPoint railPoint) = 0;
};

class LocationService {
    public:
        vector<RailPoint> &railPoints;
        vector<Route> &routes;

        LocationService(
            vector<RailPoint> &railPointsRef,
            vector<Route> &routesRef
        ) : 
            rfid(RFID(RFID_RSA_PIN, RFID_RST_PIN)),
            railPoints(railPointsRef),
            routes(routesRef)
        {
            // Set dock as active rail point by default
            RailPoint railPoint = getDockPoint(railPoints);
            activeRailPointPtr = &railPoint;

            // Init SPI bus
            SPI.begin();
            rfid.init();
        }

        void refreshActivePoint() {
            string uid = readRfidPoint();

            // If UID is empty, it means o new RFID tag has been scanned, so we keep the active one
            if (uid.length() == 0) {
                return;
            }

            // New rail point detected: match RFID uid with corresponding rail point
            RailPoint railPoint = getRailPointByRfidUId(railPoints, uid.c_str());

            // Ignore rail points in opposite direction
            MovingDirection movingDirection = StateManager::getInstance().getMovingDirection();
            if (movingDirection == MOVING_FORWARD && railPoint.id % 2 != 0 
            || movingDirection == MOVING_BACKWARD && railPoint.id % 2 == 0) {
                return;
            }

            if (currentRoutePtr != NULL && railPoint.id == currentRoutePtr->endPoint.id) {
                completeRoute();
            }
            
            activeRailPointPtr = &railPoint;

            char message[] = "Point actif: ";
            Serial.println(strcat(message, railPoint.name));

            notifyObservers(railPoint);
        }

        void followRoute(Route &route) {
            currentRoutePtr = &route;

            if (isDocked()) {
                if (route.initialDirection == MOVING_FORWARD) {
                    StateManager::getInstance().changeMovingDirection(MOVING_FORWARD);
                } else {
                    StateManager::getInstance().changeMovingDirection(MOVING_BACKWARD);
                }
            } else {
                Serial.println("ERREUR: le robot doit être arrêté au dock avant de commencer la route !");
            }
        }
        
        void notifyObservers(RailPoint railPoint) {
            for (int i = 0; i < observers.size(); i++) {
                observers[i]->didUpdateLocation(railPoint);
            }
        }

        void subscribe(LocationAware *observer) {
            observers.push_back(observer);
        }

        bool isFollowingRoute() {
            return currentRoutePtr != NULL;
        }
        
        bool isDocked() {
            MovingDirection movingDirection = StateManager::getInstance().getMovingDirection();
            return movingDirection == MOVING_IDLE && activeRailPointPtr->isDock();
        }

    private:
        RFID rfid;
        RailPoint *activeRailPointPtr = NULL;
        Route *currentRoutePtr = NULL;
        vector <class LocationAware *> observers;

        string readRfidPoint() {
            if (rfid.isCard()) {  
                if (rfid.readCardSerial()) {
                    stringstream charUid;

                    for(int n = 0; n < 5; n++) {
                        charUid << rfid.serNum[n] << ".";
                    }

                    string uid = charUid.str();
                    //string message = "RFID point detected: ";
                    //Serial.println(message + uid);

                    return uid;
                }

                rfid.halt();
            }

            return "";
        }

        void completeRoute() {
            currentRoutePtr = NULL;
            StateManager::getInstance().stop();
        }
};

#endif