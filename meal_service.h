#include <TimeLib.h>
#include "conveyor_motor.h"
#include "display_service.h"
#include "keypad_service.h"
#include "meal.h"
#include "meal_sequence.h"
#include "route.h"
#include "route_service.h"

#ifndef MEALSERVICE_H
#define MEALSERVICE_H

using namespace std;

class MealService {
    public:
        Meal * currentMealPtr = NULL;
        MealSequence * currentSequencePtr = NULL;

        MealService(int mealId);
        ~MealService();

        static int getScheduledMealId(vector<Meal> & meals);
        static bool isMealDistributed(int mealId);

        void startDistribution();
        void distributeMeal();
        void stopFeeding();
        void safetyStop();
        void displayMealDistributionScreen();
        void displayMealCompletionScreen();
        bool isDistributionCompleted();

    private:
        RouteService * routeServicePtr;
        vector<MealSequence> mealSequences;
        vector<int> completedSequenceIndexes;
        time_t startTime;
        time_t endTime;
        int currentPointId = 0;
        int lastPointId = 0;
        int sequencesCount = 0;
        unsigned long lastSafetyTrigger = 0;
        int unacknowledgedSafetyTriggers = 0;

        void refreshCurrentSequence();
        void completeDistribution();
        void getMissingSequences(char * missingSequences);
        void displaySequenceInfo(const char * name, int feed1Flow, int feed2Flow);
};

#endif
