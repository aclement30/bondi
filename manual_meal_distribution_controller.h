#include "controller.h"
#include "meal_service.h"
#include "meal.h"

#ifndef MANUAL_MEAL_DISTRIBUTION_CONTROLLER_H
#define MANUAL_MEAL_DISTRIBUTION_CONTROLLER_H

using namespace std;

class ManualMealDistributionController: public Controller {
    public:
        ManualMealDistributionController();
        ~ManualMealDistributionController();
        
        void handle();
        void resume();
        void escape();
        void safetyStop();
    private:
        MealService * mealServicePtr = NULL;

        bool hasCurrentMeal();
        void displayMealSelectionScreen();
        void displayDockingErrorScreen();
        bool displayEscapeConfirmationScreen();
};

#endif