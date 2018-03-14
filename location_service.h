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
        vector<RailPoint> &railPoints;
        vector<Route> &routes;
        RailPoint *activeRailPointPtr = NULL;
        Route *currentRoutePtr = NULL;

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
            if (uid.length() == 0 || uid == lastRfidUid) {
                Serial.print("* no new RFID uid detected: ");
                Serial.println(uid.c_str());
                return;
            }

            MovingDirection movingDirection = StateManager::getInstance().getMovingDirection();

            // New rail point detected: match RFID uid with corresponding rail point
            activeRailPointPtr = getRailPointFromRfid(railPoints, uid.c_str(), movingDirection);

            if (activeRailPointPtr == NULL) {
              Serial.print("Point RFID introuvable: ");
              Serial.println(uid.c_str());
                return;
            }
            
            if (currentRoutePtr != NULL && activeRailPointPtr->id == currentRoutePtr->endPoint.id) {
                completeRoute();
            }
            
            char message[] = "Point actif: ";
            Serial.println(strcat(message, activeRailPointPtr->name));
        }

        void followRoute(int routeId) {
            currentRoutePtr = getRouteById(routes, routeId);

            //if (isDocked()) {
                if (currentRoutePtr->initialDirection == MOVING_FORWARD) {
                    StateManager::getInstance().changeMovingDirection(MOVING_FORWARD);
                } else {
                    StateManager::getInstance().changeMovingDirection(MOVING_BACKWARD);
                }
            // } else {
            //     Serial.println("ERREUR: le robot doit être arrêté au dock avant de commencer la route !");
            // }
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
