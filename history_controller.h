#include "controller.h"
#include "meal.h"
#include "meal_service.h"

#ifndef HISTORY_CONTROLLER_H
#define HISTORY_CONTROLLER_H

using namespace std;

class HistoryController: public Controller {
    public:
        HistoryController();

        void handle();
        void escape();

    private:
        void displayHistoryScreen();
};

#endif