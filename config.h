#ifdef __EMSCRIPTEN__
#include "libs/ArduinoJson.h"
#else
#include "ArduinoJson.h"
#include <SD.h>
#endif

#include "constants.h"
#include "meal.h"
#include "rail_point.h"
#include "route.h"

#ifndef CONFIG_H
#define CONFIG_H

std::vector<RailPoint> loadRailPoints(JsonArray &data) {
    std::vector<RailPoint> points;

    for (auto& pointData : data) {
        int id = pointData["id"];
        const char* name = pointData["name"];
        const char* rfidUid = pointData["rfidUid"];
        
        RailPoint point(id, name, rfidUid);
        points.push_back(point);
    }

    return points;
}

std::vector<Route> loadRoutes(JsonArray &data, std::vector<RailPoint> railPoints) {
    std::vector<Route> routes;

    for (auto& routeData : data) {
        int id = routeData["id"];
        int initialDirection = (routeData["initialDirection"] == "forward") ? MOVING_FORWARD : MOVING_BACKWARD;
        int startPointId = routeData["startPointId"];
        int endPointId = routeData["endPointId"];
        
        Route route(id, initialDirection, getRailPointById(railPoints, startPointId), getRailPointById(railPoints, endPointId));
        routes.push_back(route);
    }

    return routes;
}

std::vector<MealSequence> loadMealSequences(JsonArray &data, std::vector<RailPoint> railPoints) {
    std::vector<MealSequence> sequences;
  
    for (auto& sequenceData : data) {
        const char* name = sequenceData["name"];
        int startPointId = sequenceData["startPointId"];
        int endPointId = sequenceData["endPointId"];
        int feed1Flow = sequenceData["feed1Flow"];
        int feed2Flow = sequenceData["feed2Flow"];

        MealSequence sequence(name, getRailPointById(railPoints, startPointId), getRailPointById(railPoints, endPointId), feed1Flow, feed2Flow);
        sequences.push_back(sequence);
    }

    return sequences;
}

std::vector<Meal> loadMeals(JsonArray &data, std::vector<Route> routes, std::vector<RailPoint> railPoints) {
    std::vector<Meal> meals;

    for (auto& mealData : data) {
        int id = mealData["id"];
        const char* name = mealData["name"];
        int startMoment = mealData["startMoment"];
        int routeId = mealData["routeId"];

        JsonArray& sequencesData = mealData["sequences"];
        std::vector<MealSequence> sequences = loadMealSequences(sequencesData, railPoints);

        Meal meal(id, name, startMoment, getRouteById(routes, routeId), sequences, sequences.size());
        meals.push_back(meal);
    }

    return meals;
}

struct Config {
    std::vector<RailPoint> railPoints;
    std::vector<Route> routes;
    std::vector<Meal> meals;
};

#ifndef __EMSCRIPTEN__

Config loadConfiguration() {
    const char *filename = "/config.json";  // <- SD library uses 8.3 filenames

    // Open file for reading
    File file = SD.open(filename);

    StaticJsonBuffer<3514> jsonBuffer;

    // Parse the root object
    JsonObject &root = jsonBuffer.parseObject(file);

    if (!root.success()) {
        Serial.println("Failed to read file, using default configuration");
    }

    Config config = Config();
    
    JsonArray& pointsData = root["points"];
    config.railPoints = loadRailPoints(pointsData);

    JsonArray& routesData = root["routes"];
    config.routes = loadRoutes(routesData, config.railPoints);

    JsonArray& mealsData = root["meals"];
    config.meals = loadMeals(mealsData, config.routes, config.railPoints);

    // Close the file (File's destructor doesn't close the file)
    file.close();

    return config;
}

#else

Config loadConfiguration() {    
    // POINTS
    RailPoint dockPoint = RailPoint(1000, "DOCK", "0.0.0.1.1");
    RailPoint pointG2 = RailPoint(2, "G-2", "0.0.0.0.1" );
    RailPoint pointG4 = RailPoint(4, "G-4", "0.0.0.0.2");

    // ROUTES
    Route route(1, MOVING_FORWARD, dockPoint, dockPoint);

    // MEALS

    MealSequence sequence1("G1", pointG2, pointG4, 500, 600);

    std::vector<MealSequence> repasMatin1Sequences = {
        sequence1
    };

    Config config = {
        {
            dockPoint,
            pointG2,
            pointG4,
        },
        {
            route
        },
        {
            Meal(1, "Repas matin - GE", 420, route, repasMatin1Sequences, 1)
        }
    };

    return config;
}

#endif
#endif