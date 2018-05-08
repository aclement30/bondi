#include "constants.h"
#include "meal_service.h"
#include "state_manager.h"

#ifndef SAFETY_SERVICE_H
#define SAFETY_SERVICE_H

using namespace std;

class SafetyService {
    public:
        static SafetyService & getInstance();

        void listenForTrigger();
        void stopListeningForTrigger();
        void displaySafetyStopWarning();
        void displayAutoDisengagement();
        void displayMovingWarning();
        static void safetyBarInterrupt();

    private:
        SafetyService() {
            pinMode(SAFETY_SENSOR, INPUT);
        }

        // C++ 03
        // ========
        // Don't forget to declare these two. You want to make sure they
        // are unacceptable otherwise you may accidentally get copies of
        // your singleton appearing.
        // DisplayService(DisplayService const&);   // Don't Implement
        void operator = (SafetyService const &); // Don't implement
        
};

#endif