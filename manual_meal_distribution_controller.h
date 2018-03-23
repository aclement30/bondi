#include "controller.h"
#include "meal_service.h"

#ifndef MANUAL_MEAL_DISTRIBUTION_CONTROLLER_H
#define MANUAL_MEAL_DISTRIBUTION_CONTROLLER_H

class ManualMealDistributionController: public Controller {
    public:
        ManualMealDistributionController(MealService & mealServiceRef);

        void handle();
        void escape();

    private:
        MealService & mealService;

        void displayMealSelectionScreen();
        bool displayEscapeConfirmationScreen();
};

#endif