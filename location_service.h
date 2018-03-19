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

            if (currentRoutePtr != NULL && activeRailPointPtr->id == currentRoutePtr->endPoint.id) {
                completeRoute();
            }
            
            char message[] = "* active point: ";
            Serial.println(strcat(message, activeRailPointPtr->name));
        }

        void followRoute(int routeId) {
            Serial.println("* clear existing route");
            delay(250);

            if (currentRoutePtr != NULL) {
                completeRoute();
            }
            
            Serial.println("* find selected route");
            delay(250);
            
            currentRoutePtr = new Route(routes.at(getRouteIndexById(routes, routeId)));
            
            Serial.println("* change moving direction");
            delay(250);
            
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

        void unfollowRoute() {
            completeRoute();
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

            Serial.println("* complete route");

            StateManager::getInstance().stop();
        }
};

#endif
