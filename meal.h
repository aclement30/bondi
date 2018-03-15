#ifndef __EMSCRIPTEN__
#include "ArduinoSTL.h"
#endif

#include "rail_point.h"
#include "route.h"

#ifndef MEAL_H
#define MEAL_H

class MealSequence {
    public:
        string name;
        RailPoint startPoint;
        RailPoint endPoint;
        int feed1Flow;
        int feed2Flow;

        MealSequence(
            string sequenceName, 
            RailPoint sequenceStartPoint, 
            RailPoint sequenceEndPoint, 
            int sequenceFeed1Flow, 
            int sequenceFeed2Flow
        ) : 
            name(sequenceName), 
            startPoint(sequenceStartPoint),
            endPoint(sequenceEndPoint),
            feed1Flow(sequenceFeed1Flow),
            feed2Flow(sequenceFeed2Flow)
        {}
};

class Meal {
    public:
        int id;
        const char *name;
        int startMoment;
        int routeId;
        std::vector<MealSequence> sequences;

        Meal(
            int mealId,
            const char *mealName,
            int mealStartMoment, 
            int mealRouteId;
            std::vector<MealSequence> mealSequences
        ) : 
            id(mealId),
            name(mealName),
            startMoment(mealStartMoment), 
            routeId(mealRouteId),
            sequences(mealSequences)
        {}

        MealSequence* getMealSequenceAt(RailPoint currentRailPoint) {
            for(int n = 0; n < sequences.size(); n++) {
                const MealSequence sequence = sequences[n];
                if (sequence.startPoint.id == currentRailPoint.id) {
                    return &sequences[n];
                }
            }

            return NULL;
        }
};

int getMealIndexById(std::vector<Meal> meals, int mealId) {
    std::vector<Meal>::iterator iterator = std::find_if (meals.begin(), meals.end(), [&](const Meal & meal) {
        return meal.id == mealId;
    });

    if (iterator == meals.end()) {
        return -1;
    }

    return iterator - meals.begin();
}

#endif