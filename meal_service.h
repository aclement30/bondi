#include <TimeLib.h>
#include "conveyor_motor.h"
#include "display_service.h"
#include "keypad_service.h"
#include "location_service.h"
#include "meal.h"
#include "meal_sequence.h"
#include "route.h"

#ifndef MEALSERVICE_H
#define MEALSERVICE_H

using namespace std;

class MealService {
    public:
        vector<Meal> & meals;
        Meal * currentMealPtr = NULL;
        MealSequence * currentSequencePtr = NULL;

        MealService(
            ConveyorMotor & conveyorFrontRef,
            ConveyorMotor & conveyorBackRef,
            vector<Meal> & mealsRef,
            LocationService & locationServiceRef
        );

        void refreshCurrentMeal();
        void selectMeal(int mealId);
        void distributeMeal();
        void refreshCurrentSequence();
        void completeDistribution();
        void abortDistribution();
        void stopFeeding();
        bool hasCurrentMeal();
        void resetDistributedMeals();
        bool isMealDistributed(int mealId);

    private:
        LocationService & locationService;
        ConveyorMotor conveyorFront;
        ConveyorMotor conveyorBack;
        vector<int> distributedMealIds;
        vector<MealSequence> mealSequences;
        int currentPointId = 0;
        int lastPointId = 0;
        int sequencesCount = 0;

        void displayMealDistributionScreen(const char * mealName);
        void displayMealCompletionScreen(const char * mealName);
        void displaySequenceInfo(const char * name, int feed1Flow, int feed2Flow);
};

#endif
