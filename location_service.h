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

class LocationService {
    public:
        vector<RailPoint> & railPoints;
        vector<Route> & routes;
        RailPoint * activeRailPointPtr = NULL;
        Route * currentRoutePtr = NULL;

        LocationService(
            vector<RailPoint> &railPointsRef,
            vector<Route> &routesRef
        ) : 
            rfid(RFID(RFID_RSA_PIN, RFID_RST_PIN)),
            railPoints(railPointsRef),
            routes(routesRef)
        {
            // Init SPI bus
            SPI.begin();
            rfid.init();
        }

        void refreshActivePoint() {
            string uid = readRfidPoint();
            
            // If UID is empty, it means o new RFID tag has been scanned, so we keep the active one
            if (uid.length() == 0 || uid.compare(lastRfidUid) == 0) {
                return;
            }

            MovingDirection movingDirection = StateManager::getInstance().getMovingDirection();
            
            // New rail point detected: match RFID uid with corresponding rail point
            int pointIndex = getRailPointIndexFromRfid(railPoints, uid.c_str(), movingDirection);
            if (pointIndex == -1) {
                return;
            }

            if (activeRailPointPtr != NULL) {
                delete activeRailPointPtr;
                activeRailPointPtr = NULL;
            }

            activeRailPointPtr = new RailPoint(railPoints.at(pointIndex));

            lastRfidUid = uid;
            
            // Serial.print("New Point: ");
            // Serial.print(activeRailPointPtr->name);
            // Serial.print(" - ");
            // Serial.println(activeRailPointPtr->pointId);
            
            if (currentRoutePtr != NULL && activeRailPointPtr->pointId == currentRoutePtr->endPoint.pointId) {
                completeRoute();
            }
            
            char message[] = "* active point: ";
            Serial.println(strcat(message, activeRailPointPtr->name));
        }

        void followRoute(int routeId) {
            if (currentRoutePtr != NULL) {
                completeRoute();
            }
            
            currentRoutePtr = new Route(routes.at(getRouteIndexById(routes, routeId)));

            if (isDocked()) {
                if (currentRoutePtr->initialDirection == MOVING_FORWARD) {
                    StateManager::getInstance().changeMovingDirection(MOVING_FORWARD);
                } else {
                    StateManager::getInstance().changeMovingDirection(MOVING_BACKWARD);
                }
            } else {
                Serial.println("ERREUR: le robot doit être arrêté au dock avant de commencer la route !");
            }
        }

        bool isFollowingRoute() {
            return currentRoutePtr != NULL;
        }
        
        bool isDocked() {
            MovingDirection movingDirection = StateManager::getInstance().getMovingDirection();
            return movingDirection == MOVING_IDLE && activeRailPointPtr != NULL && activeRailPointPtr->isDock();
        }

    private:
        RFID rfid;
        string lastRfidUid = "";
        // vector <class LocationAware *> observers;

        string readRfidPoint() {
            Serial.println("* detecting RFID point");

            if (rfid.isCard()) {  
                Serial.println("* RFID card detected");
                if (rfid.readCardSerial()) {
                    Serial.println("* reading RFID card serial");
                    char uid[20];
                    stringstream charUid;

                    for(int n = 0; n < 5; n++) {
                        charUid << rfid.serNum[n] << ".";
                    }

                    sprintf(uid, "%d.%d.%d.%d.%d", rfid.serNum[0], rfid.serNum[1], rfid.serNum[2], rfid.serNum[3], rfid.serNum[4]);

                    return uid;
                }

                rfid.halt();
            }

            return "";
        }

        void completeRoute() {
            if (currentRoutePtr != NULL) {
                delete currentRoutePtr;
                currentRoutePtr = NULL;
            }

            StateManager::getInstance().stop();
        }
};

#endif
