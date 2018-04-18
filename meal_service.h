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
        void resetDistributedMeals();
        void displayMealDistributionScreen();
        void displayMealCompletionScreen();
        bool isDistributionCompleted();

    private:
        RouteService * routeServicePtr;
        vector<int> distributedMealIds;
        vector<MealSequence> mealSequences;
        char * startTime;
        char * endTime;
        int currentPointId = 0;
        int lastPointId = 0;
        int sequencesCount = 0;
        char * missingSequences = "";
        int safetyStops = 0;

        void refreshCurrentSequence();
        void completeDistribution();
        void displaySequenceInfo(const char * name, int feed1Flow, int feed2Flow);
};

#endif
