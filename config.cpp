#include <Arduino.h>
#include "rail_point.h"
#include "config.h"

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
        MovingDirection initialDirection = (routeData["initialDirection"] == "forward") ? MOVING_FORWARD : MOVING_BACKWARD;
        int startPointId = routeData["startPointId"];
        int endPointId = routeData["endPointId"];
        
        Route route(id, initialDirection, startPointId, endPointId);
        routes.push_back(route);
    }

    return routes;
}

// std::vector<MealSequence> loadMealSequences(JsonArray &data, std::vector<RailPoint> railPoints) {
//     std::vector<MealSequence> sequences;
  
//     for (auto& sequenceData : data) {
//         const char* name = sequenceData["name"];
//         int startPointId = sequenceData["startPointId"];
//         int endPointId = sequenceData["endPointId"];
//         int feed1Flow = sequenceData["feed1Flow"];
//         int feed2Flow = sequenceData["feed2Flow"];

//         MealSequence sequence(name, getRailPointById(railPoints, startPointId), getRailPointById(railPoints, endPointId), feed1Flow, feed2Flow);
//         sequences.push_back(sequence);
//     }

//     return sequences;
// }

// std::vector<Meal> loadMeals(JsonArray &data, std::vector<Route> routes, std::vector<RailPoint> railPoints) {
//     std::vector<Meal> meals;

//     for (auto& mealData : data) {
//         int id = mealData["id"];
//         const char* name = mealData["name"];
//         int startMoment = mealData["startMoment"];
//         int routeId = mealData["routeId"];

//         JsonArray& sequencesData = mealData["sequences"];
//         std::vector<MealSequence> sequences = loadMealSequences(sequencesData, railPoints);

//         Meal meal(id, name, startMoment, getRouteById(routes, routeId), sequences);
//         meals.push_back(meal);
//     }

//     return meals;
// }

Config loadStaticConfiguration() {
    std::vector<RailPoint> railPoints = {
        RailPoint(1000, "DOCK", "155.198.195.233.119"),
        RailPoint(2, "G-2", "197.8.161.185.213"),
        RailPoint(4, "G-4", "0.0.0.0.2"),
        RailPoint(101, "REV", "0.0.0.0.4"),
        RailPoint(1, "G-1", "197.8.161.185.213"),
        RailPoint(3, "G-3", "0.0.0.0.6"),
        RailPoint(5, "P-5",  "0.0.0.0.7"),
        RailPoint(7, "P-7", "0.0.0.0.8"),
        RailPoint(9, "P-9", "0.0.0.0.9"),
        RailPoint(102, "REV", "0.0.0.0.11"),
        RailPoint(8, "P-8", "0.0.0.0.12"),
        RailPoint(10, "P-10", "0.0.0.0.13"),
        RailPoint(12, "P-12", "0.0.0.0.14"),
        RailPoint(14, "P-14", "0.0.0.0.15")
    };

    std::vector<Route> routes = {
        Route(1, MOVING_FORWARD, 1000, 1000),
        Route(2, MOVING_BACKWARD, 1000, 1000)
    };

    std::vector<MealSequence> sequence1 = {
        MealSequence("G1", 1000, 2, 25, 50),
        MealSequence("G2", 2, 1000, 50, 75)
    };

    //     MealSequence("G1", getRailPointById(railPoints, 2), getRailPointById(railPoints, 4), 25, 25),
    //     MealSequence("G2", getRailPointById(railPoints, 4), getRailPointById(railPoints, 102), 50, 50),
    //     MealSequence("G3", getRailPointById(railPoints, 102), getRailPointById(railPoints, 1), 50, 50),
    //     MealSequence("G4", getRailPointById(railPoints, 1), getRailPointById(railPoints, 3), 100, 75)
    // };

    std::vector<MealSequence> sequence2 = {
        MealSequence("P1", 1000, 1, 50, 25),
        MealSequence("P2", 1, 1000, 25, 0),
    };
    //     MealSequence("P1", getRailPointById(railPoints, 5), getRailPointById(railPoints, 7), 50, 25),
    //     MealSequence("P2", getRailPointById(railPoints, 7), getRailPointById(railPoints, 9), 25, 0),
    //     MealSequence("P3", getRailPointById(railPoints, 9), getRailPointById(railPoints, 101), 50, 0),
    //     MealSequence("P4", getRailPointById(railPoints, 101), getRailPointById(railPoints, 8), 25, 0),
    //     MealSequence("P5", getRailPointById(railPoints, 8), getRailPointById(railPoints, 10), 75, 75),
    //     MealSequence("P6", getRailPointById(railPoints, 10), getRailPointById(railPoints, 12), 75, 75),
    //     MealSequence("P7", getRailPointById(railPoints, 12), getRailPointById(railPoints, 14), 50, 25)
    // };

    std::vector<Meal> meals = {
        Meal(1, "Matin Gd.E", 420, 1, sequence1),
        Meal(2, "Matin Pt.E", 450, 2, sequence2)
    };

    Config config = {
        railPoints,
        routes,
        meals
    };

    return config;
}

Config loadSDCardConfiguration() {
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

    // JsonArray& mealsData = root["meals"];
    // config.meals = loadMeals(mealsData, config.routes, config.railPoints);

    // Close the file (File's destructor doesn't close the file)
    file.close();

    return config;
}