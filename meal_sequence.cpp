#include <Arduino.h>
#include <ArduinoSTL.h>
#include "meal_sequence.h"

using namespace std;

MealSequence::MealSequence(
    const char * sequenceName,
    int sequenceStartPointId, 
    int sequenceEndPointId, 
    int sequenceFeed1Flow, 
    int sequenceFeed2Flow
) {
    name = sequenceName;
    startPointId = sequenceStartPointId;
    endPointId = sequenceEndPointId;
    feed1Flow = sequenceFeed1Flow;
    feed2Flow = sequenceFeed2Flow;
}

int MealSequence::getSequenceIndex(vector<MealSequence> & sequences, RailPoint * railPointPtr) {
    for(int n = 0; n < sequences.size(); n++) {
        const MealSequence sequence = sequences[n];
        if (sequence.startPointId == railPointPtr->id) {
            return n;
        }
    }

    return -1;
}