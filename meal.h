#ifndef __EMSCRIPTEN__
#include "ArduinoSTL.h"
#endif

#include "rail_point.h"
#include "route.h"

#ifndef MEAL_H
#define MEAL_H

class MealSequence {
    public:
        const char name[11];
        RailPoint startPoint;
        RailPoint endPoint;
        int feed1Flow;
        int feed2Flow;

        MealSequence(
            const char sequenceName[11], 
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
        const char name[11];
        int startMoment;
        Route route;
        std::vector<MealSequence> sequences;
        int sequencesCount;

        Meal(
            int mealId,
            const char mealName[11],
            int mealStartMoment, 
            Route mealRoute,
            std::vector<MealSequence> mealSequences, 
            int mealSequencesCount
        ) : 
            id(mealId),
            name(mealName),
            startMoment(mealStartMoment), 
            route(mealRoute),
            sequences(mealSequences),
            sequencesCount(mealSequencesCount)
        {}

        MealSequence* getMealSequenceAt(RailPoint currentRailPoint) {
            for(int n = 0; n < sequencesCount; n++) {
                const MealSequence sequence = sequences[n];
                if (sequence.startPoint.id == currentRailPoint.id) {
                    return &sequences[n];
                }
            }

            return NULL;
        }
};

Meal getMealById(std::vector<Meal> meals, int mealId) {
    std::vector<Meal>::iterator iterator = std::find_if (meals.begin(), meals.end(), [&](const Meal & meal) {
        return meal.id == mealId;
    });

    return *iterator;
}

#endif