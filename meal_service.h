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
        // vector<Meal> & meals;
        Meal * currentMealPtr = NULL;
        MealSequence * currentSequencePtr = NULL;

        MealService::MealService(int mealId);
        ~MealService();

        static int getScheduledMealId(vector<Meal> & meals);
        static MealService distributeMeal(int mealId);

        // void selectMeal(int mealId);
        void startDistribution();
        void distributeMeal();
        void refreshCurrentSequence();
        void completeDistribution();
        void abortDistribution();
        void stopFeeding();
        bool hasCurrentMeal();
        void resetDistributedMeals();

        void displayMealDistributionScreen();
        void displayMealCompletionScreen();

        static bool isMealDistributed(int mealId);
        bool isDistributionCompleted();

    private:
        RouteService * routeServicePtr;
        vector<int> distributedMealIds;
        vector<MealSequence> mealSequences;
        int currentPointId = 0;
        int lastPointId = 0;
        int sequencesCount = 0;

        void displaySequenceInfo(const char * name, int feed1Flow, int feed2Flow);
};

#endif
