#include "controller.h"

#ifndef MANUAL_MENU_CONTROLLER_H
#define MANUAL_MENU_CONTROLLER_H

class ManualMenuController: public Controller {
    public:
        void handle();
        void escape();

    private:
        void displayManualMenuScreen();
};

#endif