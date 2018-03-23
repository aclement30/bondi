#include "controller.h"
#include "meal_service.h"

#ifndef HISTORY_CONTROLLER_H
#define HISTORY_CONTROLLER_H

using namespace std;

class HistoryController: public Controller {
    public:
        HistoryController(MealService & mealServiceRef);

        void handle();
        void escape();

    private:
        MealService & mealService;

        void displayHistoryScreen();
};

#endif