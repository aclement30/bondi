#include "constants.h"
#include "state_manager.h"

#ifndef SAFETY_SERVICE_H
#define SAFETY_SERVICE_H

using namespace std;

class SafetyService {
    public:
        SafetyService();

        void checkSafetyState();

    private:        
        void displaySafetyStopWarning();
        bool isSafetyBarPressed();
};

#endif