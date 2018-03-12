#include "./controller.h"

#ifndef CONTROLLERS_OFFCONTROLLER_H
#define CONTROLLERS_OFFCONTROLLER_H

// Empty controller, only used by the StateManager
class OffController: public Controller {
    public:
        OffController() {}

        void handle() {}
};

#endif