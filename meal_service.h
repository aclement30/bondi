#include "conveyor_motor.h"
#include "location_service.h"
#include "meal.h"
#include "route.h"

#ifndef __EMSCRIPTEN__
#include <TimeLib.h>
#endif

#ifndef MEALSERVICE_H
#define MEALSERVICE_H

using namespace std;

class MealService {
    public:
        vector<Meal> & meals;
        Meal *currentMealPtr = NULL;
        MealSequence *currentSequencePtr = NULL;

        MealService(
            ConveyorMotor & conveyorFrontRef,
            ConveyorMotor & conveyorBackRef,
            vector<Meal> & mealsRef,
            LocationService & locationServiceRef
        ) : 
            conveyorFront(conveyorFrontRef),
            conveyorBack(conveyorBackRef),
            meals(mealsRef),
            locationService(locationServiceRef)
        {}

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
                currentMealPtr = new Meal(meals.at(iterator - meals.begin()));

                char message[] = "Repas en cours: ";
                Serial.println(strcat(message, currentMealPtr->name));
            }
        }
    
        // Manually select a meal to distribute (ignore already distributed meals)
        void selectMeal(int mealId) {
            int mealIndex = getMealIndexById(meals, mealId);
            if (mealIndex == -1) {
                return;
            }

            if (currentMealPtr != NULL) {
                delete currentMealPtr;
                currentMealPtr = NULL;
            }

            currentMealPtr = new Meal(meals.at(mealIndex));
            
            char message[] = "Repas sélectionné: ";
            Serial.println(strcat(message, currentMealPtr->name));
        }

        void distributeMeal() {
            refreshDistributionState();
            refreshCurrentSequence();
            
            // Stop here if meal distribution just complete
            if (!hasCurrentMeal()) {
                return;
            }

            displayMealDistributionScreen(currentMealPtr->name);

            // Move feeder in corresponding direction
            if (locationService.isDocked()) {
                locationService.followRoute(currentMealPtr->route.id);
            }

            if (currentSequencePtr) {
                displaySequenceInfo(currentSequencePtr->name, currentSequencePtr->feed1Flow, currentSequencePtr->feed2Flow);

                ConveyorSide feedingSide = ((StateManager::getInstance().getMovingDirection() == MOVING_FORWARD) ? CONVEYOR_SIDE_RIGHT : CONVEYOR_SIDE_LEFT);
                if (currentSequencePtr->feed1Flow > 0) {
                    conveyorFront.start(feedingSide, currentSequencePtr->feed1Flow);
                }
                if (currentSequencePtr->feed2Flow > 0) {
                    conveyorBack.start(feedingSide, currentSequencePtr->feed2Flow);
                }
            } else {
                // Make sure all feed conveyors are stopped
                stopFeeding();
            }
        }

        void refreshCurrentSequence() {
            if (!hasCurrentMeal()) {
                return;
            }

            // Skip if active rail point hasn't changed
            if (currentPointId == locationService.activeRailPointPtr->id) {
                return;
            }

            lastPointId = currentPointId;
            currentPointId = locationService.activeRailPointPtr->id;

            if (currentSequencePtr != NULL) {
                delete currentSequencePtr;
                currentSequencePtr = NULL;
            }
            
            // Find corresponding meal sequence for active point
            int sequenceIndex = currentMealPtr->getMealSequenceIndex(locationService.activeRailPointPtr);
            if (sequenceIndex != -1) {
                currentSequencePtr = new MealSequence(currentMealPtr->sequences.at(sequenceIndex));
            }
        }

        void refreshDistributionState() {
            if (!hasCurrentMeal()) {
                return;
            }

            if (locationService.activeRailPointPtr->id == currentMealPtr->route.endPoint.id) {
                completeDistribution();
            }
        }

        void completeDistribution() {
            // Only save distribued meal ID if distributedMealIds doesn't already contains it
            if (!isMealDistributed(currentMealPtr->id)) {
                distributedMealIds.push_back(currentMealPtr->id);
            }

            char message[] = "Distribution du repas complétée: ";
            Serial.println(strcat(message, currentMealPtr->name));

            delete currentMealPtr;
            currentMealPtr = NULL;

            delete currentSequencePtr;
            currentSequencePtr = NULL;
        }

        void stopFeeding() {
            // Shutdown conveyors motor
            conveyorFront.stop();
            conveyorBack.stop();

            Serial.println("Arrêt des convoyeurs");
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
        LocationService &locationService;
        ConveyorMotor conveyorFront;
        ConveyorMotor conveyorBack;
        vector<int> distributedMealIds;
        int currentPointId = 0;
        int lastPointId = 0;

        bool isMealDistributed(int mealId) {
            return find(distributedMealIds.begin(), distributedMealIds.end(), mealId) != distributedMealIds.end();
        }

        void displayMealDistributionScreen(const char * mealName) {
            DisplayService::getInstance().clearScreen();
            DisplayService::getInstance().printTitle("DISTRIBUTION AUTO.");

            char mealLineText[20] = " Repas: ";
            DisplayService::getInstance().print(strcat(mealLineText, mealName), 1);            
        }

        void displaySequenceInfo(const char * name, int feed1Flow, int feed2Flow) {
            char nameLineText[20] = "Groupe: ";
            DisplayService::getInstance().print(strcat(nameLineText, name), 2);

            char feedLineText[20];
            sprintf(feedLineText, " Alim.: %d | %d", feed1Flow, feed2Flow);
            DisplayService::getInstance().print(feedLineText, 3);
        }
};

#endif