#include <Arduino.h>
#include <ArduinoSTL.h>
#include "rail_point.h"

#ifndef MEAL_SEQUENCE_H
#define MEAL_SEQUENCE_H

using namespace std;

struct MealSequence {
    const char * name;
    int startPointId;
    int endPointId;
    int feed1Flow;
    int feed2Flow;

    MealSequence(
        const char * sequenceName, 
        int sequenceStartPointId, 
        int sequenceEndPointId, 
        int sequenceFeed1Flow, 
        int sequenceFeed2Flow
    );

    static int getSequenceIndex(vector<MealSequence> & sequences, RailPoint * railPointPtr);
};

#endif