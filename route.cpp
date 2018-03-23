#include <Arduino.h>
#include <ArduinoSTL.h>
#include "route.h"
#include "constants.h"

using namespace std;

// TODO: Transform into struct
Route::Route(
    int routeId,
    MovingDirection routeInitialDirection, 
    int routeStartPointId, 
    int routeEndPointId
) {
    id = routeId;
    initialDirection = routeInitialDirection;
    startPointId = routeStartPointId;
    endPointId = routeEndPointId;
}

int getRouteIndexById(vector<Route> routes, int routeId) {
    vector<Route>::iterator iterator = find_if (routes.begin(), routes.end(), [&](const Route & route) {
        return route.id == routeId;
    });

    return iterator - routes.begin();
}
