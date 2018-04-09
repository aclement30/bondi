#include <Arduino.h>
#include <TimeLib.h>
#include "config.h"
#include "constants.h"
#include "conveyor_motor.h"
#include "display_service.h"
#include "keypad_service.h"
#include "log_service.h"
#include "meal.h"
#include "route.h"
#include "route_service.h"
#include "meal_service.h"
#include "state_manager.h"
#include "string.h"

using namespace std;

MealService::MealService(int mealId) {
    int mealIndex = getMealIndexById(AppConfig::getInstance().meals, mealId);
    if (mealIndex == -1) {
        mealSequences = vector<MealSequence>();
        return;
    }

    currentMealPtr = new Meal(AppConfig::getInstance().meals.at(mealIndex));
    routeServicePtr = new RouteService(currentMealPtr->routeId);
    mealSequences = loadMealSequences(getString(FILE_MEAL_SEQUENCES), currentMealPtr->id);
}

MealService::~MealService() {
    delete currentMealPtr;
    delete routeServicePtr;
}

int MealService::getScheduledMealId(vector<Meal> & meals) {
    // Search meals scheduled within the last hour inclusively
    int minMoment = ((hour() - 1) * 60) + minute();
    int maxMoment = (hour() * 60) + minute();

    vector<Meal>::iterator iterator = find_if (meals.begin(), meals.end(), [&](const Meal & meal) {
        return meal.startMoment > minMoment && meal.startMoment <= maxMoment && !isMealDistributed(meal.id);
    });

    if (iterator == meals.end()) {
        return 0;
    }

    return iterator->id;
}

bool MealService::isMealDistributed(int mealId) {
    return false;
    // return find(distributedMealIds.begin(), distributedMealIds.end(), mealId) != distributedMealIds.end();
}

void MealService::startDistribution() {
    LogService::getInstance().log(MEAL_DISTRIBUTION_START, currentMealPtr->name);
    routeServicePtr->start();
}

void MealService::distributeMeal() {
    routeServicePtr->refreshLocation();

    Serial.println("* distributeMeal");
    delay(250);

    refreshCurrentSequence();

    // All sequences have been done, complete distribution
    if (isDistributionCompleted()) {
        completeDistribution();
        return;
    }

    // Make sure feeder continues moving in the right direction (eg. after safety stop)
    if (StateManager::getInstance().getMovingDirection() == MOVING_IDLE) {
        routeServicePtr->start();
    }

    // Serial.println(F("* distributeMeal | sequence"));
    // delay(250);

    if (currentSequencePtr) {
        // Serial.println(F("* display sequence info"));
        // delay(250);

        displaySequenceInfo(currentSequencePtr->name, currentSequencePtr->feed1Flow, currentSequencePtr->feed2Flow);

        Serial.println(F("* start conveyor"));
        // delay(250);

        ConveyorSide feedingSide = ((StateManager::getInstance().getMovingDirection() == MOVING_FORWARD) ? CONVEYOR_SIDE_RIGHT : CONVEYOR_SIDE_LEFT);
        if (currentSequencePtr->feed1Flow > 0) {
            FrontConveyor::getInstance().start(feedingSide, currentSequencePtr->feed1Flow);
        }
        if (currentSequencePtr->feed2Flow > 0) {
            BackConveyor::getInstance().start(feedingSide, currentSequencePtr->feed2Flow);
        }
    } else {
        Serial.println(F("* stop feeding"));
        // delay(250);

        // Make sure all feed conveyors are stopped
        stopFeeding();
    }
}

void MealService::stopFeeding() {
    // Shutdown conveyors motor
    FrontConveyor::getInstance().stop();
    BackConveyor::getInstance().stop();

    Serial.println(F("Arrêt des convoyeurs"));
}

// TODO: Execute daily
void MealService::resetDistributedMeals() {
    distributedMealIds.clear();
    
    Serial.println(F("Réinitialisation des repas distribués"));
}

bool MealService::isDistributionCompleted() {
    return sequencesCount > mealSequences.size();
}

// PRIVATE

void MealService::refreshCurrentSequence() {
    // Skip if active rail point hasn't changed
    if (currentPointId == routeServicePtr->activeRailPointPtr->id) {
        return;
    }

    lastPointId = currentPointId;
    currentPointId = routeServicePtr->activeRailPointPtr->id;

    if (currentSequencePtr != NULL) {
        delete currentSequencePtr;
        currentSequencePtr = NULL;
    }

    sequencesCount++;

    // All sequences have been done, complete distribution
    if (isDistributionCompleted()) {
        return;
    }
    
    // Find corresponding meal sequence for active point
    int sequenceIndex = MealSequence::getSequenceIndex(mealSequences, routeServicePtr->activeRailPointPtr);
    if (sequenceIndex != -1) {
        currentSequencePtr = new MealSequence(mealSequences.at(sequenceIndex));
    }
}

void MealService::completeDistribution() {
    // Make sure all feed conveyors are stopped
    stopFeeding();
    routeServicePtr->stop();

    // Only save distribued meal ID if distributedMealIds doesn't already contains it
    if (!isMealDistributed(currentMealPtr->id)) {
        distributedMealIds.push_back(currentMealPtr->id);
    }

    LogService::getInstance().log(MEAL_DISTRIBUTION_END, currentMealPtr->name);
}

void MealService::displayMealDistributionScreen() {
    DisplayService::getInstance().clearScreen();
    
    MachineState currentState = StateManager::getInstance().getState();
    if (currentState == Automatic) {
        const static char title[] PROGMEM = "DISTRIBUTION AUTO.";
        DisplayService::getInstance().printTitle(getString(title));
    } else {
        const static char title[] PROGMEM = "DISTRIBUTION REPAS";
        DisplayService::getInstance().printTitle(getString(title));
    }

    char mealLineText[20] = " Repas: ";
    DisplayService::getInstance().print(strcat(mealLineText, currentMealPtr->name), 1);            
}


void MealService::displayMealCompletionScreen() {
    const static char title[] PROGMEM = "FIN DISTRIBUTION";

    DisplayService::getInstance().clearScreen();
    
    DisplayService::getInstance().printTitle(getString(title));
    DisplayService::getInstance().addBorder();

    char mealLineText[20] = "Repas: ";
    DisplayService::getInstance().printCenter(strcat(mealLineText, currentMealPtr->name), 2);  

    KeypadService::getInstance().waitForActivity(3000);          
}

void MealService::displaySequenceInfo(const char * name, int feed1Flow, int feed2Flow) {
    char nameLineText[20] = "Groupe: ";
    DisplayService::getInstance().print(strcat(nameLineText, name), 2);

    char feedLineText[20];
    sprintf(feedLineText, " Alim.: %d | %d", feed1Flow, feed2Flow);
    DisplayService::getInstance().print(feedLineText, 3);
}