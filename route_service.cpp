#include <ArduinoSTL.h>
#include <string.h>
#include "config.h"
#include "constants.h"
#include "location_service.h"
#include "log_service.h"
#include "route.h"
#include "rail_motor.h"
#include "route_service.h"
#include "state_manager.h"

using namespace std;

RouteService::RouteService(int routeId) {
    currentRoutePtr = new Route(AppConfig::getInstance().routes.at(getRouteIndexById(AppConfig::getInstance().routes, routeId)));
}

RouteService::~RouteService() {
    delete currentRoutePtr;
}

void RouteService::start() {
    refreshLocation();

    // If route was already started, keeps going in the same direction as before
    if (activeRailPointPtr != NULL && !activeRailPointPtr->isDock()) {
        if (activeRailPointPtr->getMovingDirection() == MOVING_FORWARD) {
            RailMotor::getInstance().moveForward();
        } else {
            RailMotor::getInstance().moveBackward();
        }

        return;
    }

    LogService::getInstance().log(ROUTE_START, currentRoutePtr->id);

    if (currentRoutePtr->initialDirection == MOVING_FORWARD) {
        RailMotor::getInstance().moveForward();
    } else {
        RailMotor::getInstance().moveBackward();
    }
}

void RouteService::stop() {
    RailMotor::getInstance().stop();
}

void RouteService::refreshLocation() {
    char uid[20];
    bool hasRfidUid = LocationService::getInstance().readRfidUid(uid);

    // If UID is empty or equals to last one, it means no new RFID tag has been scanned, so we keep the active one
    if (!hasRfidUid || strcmp(uid, lastRfidUid) == 0) {
        return;
    }

    Serial.print(F("* new RFID uid: "));
    Serial.println(uid);

    checkpointsCount += 1;
    strcpy(lastRfidUid, uid);

    MovingDirection movingDirection = StateManager::getInstance().getMovingDirection();
    
    // New rail point detected: match RFID uid with corresponding rail point
    int pointIndex = getRailPointIndexFromRfid(AppConfig::getInstance().railPoints, uid, movingDirection);
    if (pointIndex == -1) {
        return;
    }

    if (activeRailPointPtr != NULL) {
        delete activeRailPointPtr;
        activeRailPointPtr = NULL;
    }

    activeRailPointPtr = new RailPoint(AppConfig::getInstance().railPoints.at(pointIndex));
    
    if (activeRailPointPtr->isReverse()) {
        RailMotor::getInstance().inverseMovingDirection();
    }

    // char message[] = "* active point: ";
    // Serial.println(strcat(message, activeRailPointPtr->name));

    if (isCompleted()) {
        LogService::getInstance().log(ROUTE_END, currentRoutePtr->id);
    }
}

bool RouteService::isCompleted() {
    return checkpointsCount > 1 && activeRailPointPtr != NULL && activeRailPointPtr->id == currentRoutePtr->endPointId;
}