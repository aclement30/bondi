#include "controller.h"
#include "meal_service.h"

#ifndef AUTOMATIC_CONTROLLER_H
#define AUTOMATIC_CONTROLLER_H

class AutomaticController: public Controller {
    public:
        AutomaticController(MealService & mealServiceRef);
        void handle();
        void escape();

    private:
        MealService & mealService;

        void displayAutomaticModeScreen();
        bool displayEscapeConfirmationScreen();
};

#endif