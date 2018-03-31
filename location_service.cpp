#include <ArduinoSTL.h>
#include <string.h>
#include <SPI.h>
#include <RFID.h>
#include "constants.h"
#include "rail_point.h"
#include "route.h"
#include "state_manager.h"
#include "location_service.h"

using namespace std;

LocationService::LocationService(
    vector<RailPoint> & railPointsRef,
    vector<Route> & routesRef
) : 
    rfid(RFID(RFID_RSA_PIN, RFID_RST_PIN)),
    railPoints(railPointsRef),
    routes(routesRef)
{}

void LocationService::setup() {
    rfid.init();
}

void LocationService::refreshActivePoint() {
    char uid[20];
    bool rfidPoint = readRfidPoint(uid);

    // Always reset active rail point where not following a route
    if (currentRoutePtr == NULL && activeRailPointPtr != NULL) {
        delete activeRailPointPtr;
        activeRailPointPtr = NULL;
        strcpy(lastRfidUid, "");
    }

    // If UID is empty, it means a new RFID tag has been scanned, so we keep the active one
    if (!rfidPoint || strcmp(uid, lastRfidUid) == 0) {
        return;
    }

    Serial.print(F("* new RFID uid: "));
    Serial.println(uid);
    strcpy(lastRfidUid, uid);

    MovingDirection movingDirection = StateManager::getInstance().getMovingDirection();
    
    // New rail point detected: match RFID uid with corresponding rail point
    int pointIndex = getRailPointIndexFromRfid(railPoints, uid, movingDirection);
    if (pointIndex == -1) {
        return;
    }

    if (activeRailPointPtr != NULL) {
        delete activeRailPointPtr;
        activeRailPointPtr = NULL;
    }

    activeRailPointPtr = new RailPoint(railPoints.at(pointIndex));

    if (currentRoutePtr != NULL && activeRailPointPtr->id == currentRoutePtr->endPointId) {
        completeRoute();
    }
    
    char message[] = "* active point: ";
    Serial.println(strcat(message, activeRailPointPtr->name));
}

void LocationService::followRoute(int routeId) {
    // Serial.println("* clear existing route");
    // delay(250);

    if (currentRoutePtr != NULL) {
        completeRoute();
    }
    
    // Serial.println("* find selected route");
    // delay(250);
    
    currentRoutePtr = new Route(routes.at(getRouteIndexById(routes, routeId)));
    
    // Serial.println("* change moving direction");
    // delay(250);
    
    if (isDocked()) {
        if (currentRoutePtr->initialDirection == MOVING_FORWARD) {
            StateManager::getInstance().changeMovingDirection(MOVING_FORWARD);
        } else {
            StateManager::getInstance().changeMovingDirection(MOVING_BACKWARD);
        }
    } else {
        Serial.println(F("ERREUR: le robot doit être arrêté au dock avant de commencer la route !"));
    }
}

void LocationService::unfollowRoute() {
    completeRoute();
}

bool LocationService::isFollowingRoute() {
    return currentRoutePtr != NULL;
}

bool LocationService::isDocked() {
    MovingDirection movingDirection = StateManager::getInstance().getMovingDirection();
    return movingDirection == MOVING_IDLE && activeRailPointPtr != NULL && activeRailPointPtr->isDock();
}

bool LocationService::readRfidPoint(char * uid) {
    Serial.println(F("* detecting RFID point"));

    if (rfid.isCard()) {  
        Serial.println(F("* RFID card detected"));
        if (rfid.readCardSerial()) {
            Serial.println(F("* reading RFID card serial"));
            sprintf(uid, "%d.%d.%d.%d.%d", rfid.serNum[0], rfid.serNum[1], rfid.serNum[2], rfid.serNum[3], rfid.serNum[4]);
            
            return true;
        }

        rfid.halt();
    }

    return false;
}

// PRIVATE

void LocationService::completeRoute() {
    if (currentRoutePtr != NULL) {
        delete currentRoutePtr;
        currentRoutePtr = NULL;
    }

    Serial.println(F("* complete route"));

    StateManager::getInstance().stop();
}