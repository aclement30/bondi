#include "meal.h"
#include "route.h"

#ifndef __EMSCRIPTEN__
#include <TimeLib.h>
#endif

#ifndef MEALSERVICE_H
#define MEALSERVICE_H

using namespace std;

class MealService: public LocationAware {
    public:
        vector<Meal> &meals;
        Feeder &feeder;

        Meal *currentMealPtr = NULL;
        MealSequence *currentSequencePtr = NULL;

        MealService(
            vector<Meal> &availableMeals,
            Feeder &feederRef
        ) : 
            meals(availableMeals),
            feeder(feederRef)
        {
            feeder.subscribeToLocation(this);
        }

        Meal getCurrentMeal() {
            return Meal(* currentMealPtr);
        }

        void refreshCurrentMeal() {
            // Skip if there is already a meal being distributed
            if (hasCurrentMeal()) return;

            // Search meals scheduled within the last hour inclusively
            int minMoment = ((hour() - 1) * 60) + minute();
            int maxMoment = (hour() * 60) + minute();

            vector<Meal>::iterator iterator = find_if (meals.begin(), meals.end(), [&](const Meal & meal) {
                return meal.startMoment > minMoment && meal.startMoment <= maxMoment && !isMealDistributed(meal.id);
            });

            if (iterator != meals.end()) {
                currentMealPtr = &*iterator;

                Meal currentMeal = Meal(* currentMealPtr);
                char message[] = "Repas en cours: ";
                Serial.println(strcat(message, currentMeal.name));
            }
        }
    
        // Manually select a meal to distribute (ignore already distributed meals)
        void selectMeal(int mealId) {
            Meal selectedMeal = getMealById(meals, mealId);

            currentMealPtr = &selectedMeal;
            
            char message[] = "Repas sélectionné: ";
            Serial.println(strcat(message, selectedMeal.name));
        }

        void distributeMeal(Meal meal) {
            // Move feeder in corresponding direction
            if (feeder.isDocked()) {
                feeder.followRoute(meal.route);
            }

            if (currentSequencePtr) {
                MealSequence sequence = MealSequence(* currentSequencePtr);

                int feedingSide = ((feeder.getMovingDirection() == MOVING_FORWARD) ? CONVEYOR_SIDE_RIGHT : CONVEYOR_SIDE_LEFT);
                if (sequence.feed1Flow > 0) {
                    feeder.conveyorFront.start(feedingSide, sequence.feed1Flow);
                }
                if (sequence.feed2Flow > 0) {
                    feeder.conveyorBack.start(feedingSide, sequence.feed2Flow);
                }
            } else {
                // Make sure all feed conveyors are stopped
                feeder.stopFeeding();
            }
        }

        void didUpdateLocation(RailPoint railPoint) {
            if (!hasCurrentMeal()) {
                return;
            }

            Meal currentMeal = getCurrentMeal();
            currentSequencePtr = currentMeal.getMealSequenceAt(railPoint);

            RailPoint lastPoint = RailPoint(* lastPointPtr);
            if (railPoint.id == currentMeal.route.endPoint.id && railPoint.id != lastPoint.id) {
                completeDistribution();
            }

            lastPointPtr = &railPoint;
        }

        void completeDistribution() {
            Meal currentMeal = getCurrentMeal();

            // Only save distribued meal ID if distributedMealIds doesn't already contains it
            if (!isMealDistributed(currentMeal.id)) {
                distributedMealIds.push_back(currentMeal.id);
            }

            char message[] = "Distribution du repas complétée: ";
            Serial.println(strcat(message, currentMeal.name));

            currentMealPtr = NULL;
            currentSequencePtr = NULL;
        }

        bool hasCurrentMeal() {
            return currentMealPtr != NULL;
        }

        // TODO: Execute daily
        void resetDistributedMeals() {
            distributedMealIds.clear();
            
            Serial.println("Réinitialisation des repas distribués");
        }

    private:
        vector<int> distributedMealIds;
        RailPoint *lastPointPtr = NULL;

        bool isMealDistributed(int mealId) {
            return find(distributedMealIds.begin(), distributedMealIds.end(), mealId) != distributedMealIds.end();
        }
};

#endif