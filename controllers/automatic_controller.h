#include "./controller.h"
#include "../feeder.h"
#include "../meal_service.h"
#include "../state_manager.h"

#ifndef CONTROLLERS_AUTOMATICCONTROLLER_H
#define CONTROLLERS_AUTOMATICCONTROLLER_H

using namespace std;

class AutomaticController: public Controller {
    public:
        AutomaticController(
            //MealService &mealServiceRef
        ) 
           // mealService(mealServiceRef)
        {}

        void handle() {
            // TODO: Show auto mode screen

            // mealService.refreshCurrentMeal();

            // if (mealService.hasCurrentMeal()) {
            //     Meal currentMeal = mealService.getCurrentMeal();

            //     // TODO: Show meal distribution screen

            //     mealService.distributeMeal(currentMeal);
            // } else {
            //     Serial.println("Attente...");
            //     delay(1000);
            // }
        }

    private:
        // MealService &mealService;
};

#endif