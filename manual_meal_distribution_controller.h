#include "controller.h"
#include "meal_service.h"

#ifndef MANUAL_MEAL_DISTRIBUTION_CONTROLLER_H
#define MANUAL_MEAL_DISTRIBUTION_CONTROLLER_H

class ManualMealDistributionController: public Controller {
    public:
        ManualMealDistributionController(
            MealService & mealServiceRef,
            LocationService & locationServiceRef
        );

        void handle();
        void escape();

    private:
        MealService & mealService;
        LocationService & locationService;

        void displayMealSelectionScreen();
        void displayDockingErrorScreen();
        bool displayEscapeConfirmationScreen();
};

#endif