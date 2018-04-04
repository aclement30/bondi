#include "./controller.h"

#ifndef OFF_CONTROLLER_H
#define OFF_CONTROLLER_H

// Empty controller, only used by the StateManager
class OffController: public Controller {
    public:
        void handle();
};

#endif