#include "ArduinoSTL.h"
#include "trip.h"
#include "rail_point.h"

#ifndef MEAL_H
#define MEAL_H

class MealSequence {
    public:
        const char *name;
        const RailPoint startPoint;
        const RailPoint endPoint;
        const int feed1Flow;
        const int feed2Flow;

        MealSequence(
            const char *sequenceName, 
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
        const int startHour;
        const int startMinutes;
        const Trip trip;
        const std::vector<MealSequence> sequences;
        const int sequencesCount;

        Meal(
            int mealStartHour, 
            int mealStartMinutes, 
            Trip mealTrip,
            const std::vector<MealSequence> mealSequences, 
            int mealSequencesCount
        ) : 
            startHour(mealStartHour), 
            startMinutes(mealStartMinutes),
            trip(mealTrip),
            sequences(mealSequences),
            sequencesCount(mealSequencesCount)
        {}

        const MealSequence* getMealSequenceAt(RailPoint currentRailPoint) {
            for(int n = 0; n < sequencesCount; n++) {
                const MealSequence sequence = sequences[n];
                if (sequence.startPoint.id == currentRailPoint.id) {
                    return &sequences[n];
                }
            }

            return NULL;
        }
};

#endif