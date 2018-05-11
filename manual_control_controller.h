#include "controller.h"

#ifndef MANUAL_CONTROL_CONTROLLER_H
#define MANUAL_CONTROL_CONTROLLER_H

class ManualControlController: public Controller {
    public:
        ManualControlController();

        void handle();
        void escape();

    private:
        void displayManualControlScreen(bool showInvite = true);
        void handleCommand(int key);
        void displayInvite();
        void displayMovingDirection(MovingDirection direction);
        void displayConveyorsState(bool frontConveyorMoving, bool backConveyorMoving);
};

#endif