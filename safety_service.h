#include "constants.h"
#include "meal_service.h"
#include "state_manager.h"

#ifndef SAFETY_SERVICE_H
#define SAFETY_SERVICE_H

using namespace std;

class SafetyService {
    public:
        SafetyService(
            MealService & mealServiceRef
        );

        void checkSafetyState();

    private:
        MealService & mealService;

        void displaySafetyStopWarning();
        bool isSafetyBarPressed();
};

#endif