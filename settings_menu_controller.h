#include "controller.h"

#ifndef SETTINGS_MENU_CONTROLLER_H
#define SETTINGS_MENU_CONTROLLER_H

class SettingsMenuController: public Controller {
    public:
        void handle();
        void escape();

    private:
        void displaySettingsMenuScreen();
};

#endif