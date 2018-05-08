#include "controller.h"
#include "location_service.h"
#include "meal_service.h"

#ifndef AUTOMATIC_CONTROLLER_H
#define AUTOMATIC_CONTROLLER_H

using namespace std;

class AutomaticController: public Controller {
    public:
        AutomaticController();
        ~AutomaticController();

        void handle();
        void resume();
        void escape();
        void safetyStop();
        
    private:
        MealService * mealServicePtr = NULL;

        bool hasCurrentMeal();
        void displayAutomaticModeScreen();
        bool displayEscapeConfirmationScreen();
};

#endif