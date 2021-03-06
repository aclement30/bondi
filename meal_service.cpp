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
#include "safety_service.h"
#include "state_manager.h"
#include "time_service.h"
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
    int maxMoment = TimeService::getInstance().getLocalMoment();
    int minMoment = maxMoment - 60;
    if (minMoment < 0) {
        minMoment = 0;
    }

    vector<Meal>::iterator iterator = find_if (meals.begin(), meals.end(), [&](const Meal & meal) {
        return meal.startMoment > minMoment && meal.startMoment <= maxMoment && !isMealDistributed(meal.id);
    });

    if (iterator == meals.end()) {
        return 0;
    }

    return iterator->id;
}

bool MealService::isMealDistributed(int mealId) {
    vector<int> distributedMealIds = LogService::getInstance().getDistributedMealIds();
    return find(distributedMealIds.begin(), distributedMealIds.end(), mealId) != distributedMealIds.end();
}

void MealService::startDistribution() {
    startTime = TimeService::getInstance().getTime();

    SafetyService::getInstance().listenForTrigger();

    LogService::getInstance().log(MEAL_DISTRIBUTION_START, currentMealPtr->name);
    routeServicePtr->start();
}

void MealService::distributeMeal() {
    if (StateManager::getInstance().isSafetyMode()) {
        return;
    }
    
    routeServicePtr->refreshLocation();

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

        // Serial.println(F("* start conveyor"));
        // delay(250);

        ConveyorSide feedingSide = ((StateManager::getInstance().getMovingDirection() == MOVING_FORWARD) ? CONVEYOR_SIDE_RIGHT : CONVEYOR_SIDE_LEFT);
        if (currentSequencePtr->feed1Flow > 0) {
            FrontConveyor::getInstance().start(feedingSide, currentSequencePtr->feed1Flow);
        }
        if (currentSequencePtr->feed2Flow > 0) {
            BackConveyor::getInstance().start(feedingSide, currentSequencePtr->feed2Flow);
        }
    } else {
        // Serial.println(F("* stop feeding"));
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

void MealService::safetyStop() {
    SafetyService::getInstance().displaySafetyStopWarning();

    unsigned long currentTime = millis();
    if (lastSafetyTrigger < currentTime - SAFETY_STOP_RESET_INTERVAL) {
        // When last safety trigger was before reset interval, automatically resets unacknowledged counter
        // This prevents a meal distribution complete unwanted stop after 3 unrelated occurences
        unacknowledgedSafetyTriggers = 0;
    }

    lastSafetyTrigger = currentTime;
    
    if (unacknowledgedSafetyTriggers < 3) {
        // Wait 30 seconds for confirmation/escape, then disengage safety mode if no answer
        ConfirmationResponse canContinue = KeypadService::getInstance().waitForConfirmation(SAFETY_STOP_RETRY_DELAY);

        if (canContinue == CONFIRMATION_TIMEOUT) {
            unacknowledgedSafetyTriggers += 1;
        }

        if (canContinue != CONFIRMATION_DENIED) {
            StateManager::getInstance().disengageSafetyMode(false);
        } else {
            StateManager::getInstance().getCurrentController()->escape();
        }
    } else {
        // Wait until someone acknowledge the safety stop and allows distribution to resume
        bool canContinue = KeypadService::getInstance().waitForConfirmation();

        if (canContinue) {
            StateManager::getInstance().disengageSafetyMode(true);
        } else {
            StateManager::getInstance().getCurrentController()->escape();
        }
    }
}

bool MealService::isDistributionCompleted() {
    // Distribution is completed when the last sequence is the last of the meal OR the route is completed (failsafe)
    return currentSequencePtr == NULL && completedSequenceIndexes.size() > 1 && (completedSequenceIndexes.back() == (mealSequences.size() - 1) || routeServicePtr->isCompleted());
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

    // // All sequences have been done, complete distribution
    // if (isDistributionCompleted()) {
    //     return;
    // }
    
    // Find corresponding meal sequence for active point
    int sequenceIndex = MealSequence::getSequenceIndex(mealSequences, routeServicePtr->activeRailPointPtr);
    if (sequenceIndex != -1 && find(completedSequenceIndexes.begin(), completedSequenceIndexes.end(), sequenceIndex) == completedSequenceIndexes.end()) {
        completedSequenceIndexes.push_back(sequenceIndex);
        currentSequencePtr = new MealSequence(mealSequences.at(sequenceIndex));
    }
}

void MealService::completeDistribution() {
    endTime = TimeService::getInstance().getTime();

    // Make sure all feed conveyors are stopped
    stopFeeding();
    routeServicePtr->stop();

    SafetyService::getInstance().stopListeningForTrigger();

    char missingSequences[100] = "";
    getMissingSequences(missingSequences);

    LogService::getInstance().log(MEAL_DISTRIBUTION_END, currentMealPtr->name);
    LogService::getInstance().logDistribution(currentMealPtr->id, startTime, endTime, missingSequences);
}

void MealService::getMissingSequences(char * missingSequences) {
    for(int n = 0; n < mealSequences.size(); n++) {
        if (find(completedSequenceIndexes.begin(), completedSequenceIndexes.end(), n) == completedSequenceIndexes.end()) {
            // Meal sequence not found in completed sequences list
            if (strlen(missingSequences) > 0) {
                strcpy(missingSequences, ",");
            }

            sprintf(missingSequences, "%d", n);
        }
    }
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