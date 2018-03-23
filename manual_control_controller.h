#include "controller.h"

#ifndef MANUAL_CONTROL_CONTROLLER_H
#define MANUAL_CONTROL_CONTROLLER_H

class ManualControlController: public Controller {
    public:
        ManualControlController(MealService & mealServiceRef);

        void handle();
        void escape();

    private:
        MealService & mealService;

        void displayManualControlScreen();
};

#endif