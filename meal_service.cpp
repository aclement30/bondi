#include <Arduino.h>
#include <TimeLib.h>
#include "constants.h"
#include "conveyor_motor.h"
#include "display_service.h"
#include "keypad_service.h"
#include "location_service.h"
#include "meal.h"
#include "route.h"
#include "meal_service.h"
#include "state_manager.h"

using namespace std;

MealService::MealService(
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

void MealService::refreshCurrentMeal() {
    // Skip if there is already a meal being distributed
    if (hasCurrentMeal()) return;

    // Search meals scheduled within the last hour inclusively
    int minMoment = ((hour() - 1) * 60) + minute();
    int maxMoment = (hour() * 60) + minute();

    vector<Meal>::iterator iterator = find_if (meals.begin(), meals.end(), [&](const Meal & meal) {
        return meal.startMoment > minMoment && meal.startMoment <= maxMoment && !isMealDistributed(meal.id);
    });

    selectMeal(iterator->id);
}

// Select a meal to distribute (ignore already distributed meals)
void MealService::selectMeal(int mealId) {
    int mealIndex = getMealIndexById(meals, mealId);
    if (mealIndex == -1) {
        return;
    }

    if (currentMealPtr != NULL) {
        delete currentMealPtr;
        currentMealPtr = NULL;
    }

    currentMealPtr = new Meal(meals.at(mealIndex));
    sequencesCount = 0;
    displayMealDistributionScreen(currentMealPtr->name);

    // char message[] = "Repas sélectionné: ";
    // Serial.println(strcat(message, currentMealPtr->name));
}

void MealService::distributeMeal() {
    refreshCurrentSequence();

    // Stop here if meal distribution just complete
    if (!hasCurrentMeal()) {
        return;
    }

    // Move feeder in corresponding direction
    if (locationService.isDocked()) {
        locationService.followRoute(currentMealPtr->routeId);
    }

    Serial.println("* distributeMeal | sequence");
    delay(250);

    if (currentSequencePtr) {
        Serial.println("* display sequence info");
        delay(250);

        displaySequenceInfo(currentSequencePtr->name, currentSequencePtr->feed1Flow, currentSequencePtr->feed2Flow);

        Serial.println("* start conveyor");
        delay(250);

        ConveyorSide feedingSide = ((StateManager::getInstance().getMovingDirection() == MOVING_FORWARD) ? CONVEYOR_SIDE_RIGHT : CONVEYOR_SIDE_LEFT);
        if (currentSequencePtr->feed1Flow > 0) {
            conveyorFront.start(feedingSide, currentSequencePtr->feed1Flow);
        }
        if (currentSequencePtr->feed2Flow > 0) {
            conveyorBack.start(feedingSide, currentSequencePtr->feed2Flow);
        }
    } else {
        Serial.println("* stop feeding");
        delay(250);

        // Make sure all feed conveyors are stopped
        stopFeeding();
    }
}

void MealService::refreshCurrentSequence() {
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

    sequencesCount++;

    // All sequences have been done, complete distribution
    if (sequencesCount > currentMealPtr->sequences.size()) {
        completeDistribution();
        return;
    }
    
    // Find corresponding meal sequence for active point
    int sequenceIndex = currentMealPtr->getMealSequenceIndex(locationService.activeRailPointPtr);
    if (sequenceIndex != -1) {
        currentSequencePtr = new MealSequence(currentMealPtr->sequences.at(sequenceIndex));
    }
}

void MealService::completeDistribution() {
    // Make sure all feed conveyors are stopped
    stopFeeding();

    // Only save distribued meal ID if distributedMealIds doesn't already contains it
    if (!isMealDistributed(currentMealPtr->id)) {
        distributedMealIds.push_back(currentMealPtr->id);
    }

    char message[] = "Distribution du repas complétée: ";
    Serial.println(strcat(message, currentMealPtr->name));

    displayMealCompletionScreen(currentMealPtr->name);
    KeypadService::getInstance().waitForActivity(3000);

    delete currentMealPtr;
    currentMealPtr = NULL;

    delete currentSequencePtr;
    currentSequencePtr = NULL;

    lastPointId = 0;
    sequencesCount = 0;
}

void MealService::abortDistribution() {
    stopFeeding();
    locationService.unfollowRoute();
    
    delete currentMealPtr;
    currentMealPtr = NULL;

    delete currentSequencePtr;
    currentSequencePtr = NULL;

    lastPointId = 0;
    sequencesCount = 0;
}

void MealService::stopFeeding() {
    // Shutdown conveyors motor
    conveyorFront.stop();
    conveyorBack.stop();

    Serial.println("Arrêt des convoyeurs");
}

bool MealService::hasCurrentMeal() {
    return currentMealPtr != NULL;
}

// TODO: Execute daily
void MealService::resetDistributedMeals() {
    distributedMealIds.clear();
    
    Serial.println("Réinitialisation des repas distribués");
}

bool MealService::isMealDistributed(int mealId) {
    return find(distributedMealIds.begin(), distributedMealIds.end(), mealId) != distributedMealIds.end();
}

// PRIVATE

void MealService::displayMealDistributionScreen(const char * mealName) {
    DisplayService::getInstance().clearScreen();
    
    MachineState currentState = StateManager::getInstance().getState();
    if (currentState == Automatic) {
        DisplayService::getInstance().printTitle("DISTRIBUTION AUTO.");
    } else {
        DisplayService::getInstance().printTitle("DISTRIBUTION REPAS");
    }

    char mealLineText[20] = " Repas: ";
    DisplayService::getInstance().print(strcat(mealLineText, mealName), 1);            
}

void MealService::displayMealCompletionScreen(const char * mealName) {
    DisplayService::getInstance().clearScreen();
    
    DisplayService::getInstance().printTitle("FIN DISTRIBUTION");
    DisplayService::getInstance().addBorder();

    char mealLineText[20] = "Repas: ";
    DisplayService::getInstance().printCenter(strcat(mealLineText, mealName), 2);            
}

void MealService::displaySequenceInfo(const char * name, int feed1Flow, int feed2Flow) {
    char nameLineText[20] = "Groupe: ";
    DisplayService::getInstance().print(strcat(nameLineText, name), 2);

    char feedLineText[20];
    sprintf(feedLineText, " Alim.: %d | %d", feed1Flow, feed2Flow);
    DisplayService::getInstance().print(feedLineText, 3);
}