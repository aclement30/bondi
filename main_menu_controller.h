#include "controller.h"

#ifndef MAIN_MENU_CONTROLLER_H
#define MAIN_MENU_CONTROLLER_H

class MainMenuController: public Controller {
    public:
        void handle();
        void escape();

    private:
        void displayNavMenu();
};

#endif