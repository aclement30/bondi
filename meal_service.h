#include "meal.h"
#include "route.h"

#ifndef __EMSCRIPTEN__
#include <TimeLib.h>
#endif

#ifndef MEALSERVICE_H
#define MEALSERVICE_H

class MealService {
    public:
        std::vector<Meal> meals;

        Meal *currentMealPtr = NULL;

        MealService(
            std::vector<Meal> availableMeals
        ) : 
            meals(availableMeals)
        {}

        Meal getCurrentMeal() {
            return Meal(* currentMealPtr);
        }

        void refreshCurrentMeal() {
            // Skip if there is already a meal being distributed
            if (currentMealPtr != NULL) return;

            // Search meals scheduled within the last hour inclusively
            int minMoment = ((hour() - 1) * 60) + minute();
            int maxMoment = (hour() * 60) + minute();

            std::vector<Meal>::iterator iterator = std::find_if (meals.begin(), meals.end(), [&](const Meal & meal) {
                return meal.startMoment > minMoment && meal.startMoment <= maxMoment && !isMealDistributed(meal.id);
            });

            if (iterator != meals.end()) {
                currentMealPtr = &*iterator;

                Meal currentMeal = Meal(* currentMealPtr);
                String message = "Current meal: ";
                Serial.println(message + currentMeal.name);
            }
        }
    
        // Manually select a meal to distribute (ignore already distributed meals)
        void selectMeal(int mealId) {
            Meal selectedMeal = getMealById(meals, mealId);

            currentMealPtr = &selectedMeal;
            
            String message = "Meal selected: ";
            Serial.println(message + selectedMeal.name);
        }

        void completeDistribution() {
            Meal currentMeal = getCurrentMeal();

            // Only save distribued meal ID if distributedMealIds doesn't already contains it
            if (!isMealDistributed(currentMeal.id)) {
                distributedMealIds.push_back(currentMeal.id);
            }

            String message = "Complete meal distribution: ";
            Serial.println(message + currentMeal.name);

            currentMealPtr = NULL;
        }

        bool hasCurrentMeal() {
            return currentMealPtr != NULL;
        }

        // TODO: Execute daily
        void resetDistributedMeals() {
            distributedMealIds.clear();
            
            Serial.println("Reset distributed meals");
        }

    private:
        std::vector<int> distributedMealIds;

        bool isMealDistributed(int mealId) {
            return std::find(distributedMealIds.begin(), distributedMealIds.end(), mealId) != distributedMealIds.end();
        }
};

#endif