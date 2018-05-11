#include "controller.h"

#ifndef TIME_SETTING_CONTROLLER_H
#define TIME_SETTING_CONTROLLER_H

class TimeSettingController: public Controller {
    public:
        void handle();
        void escape();

    private:
        void displayConfigurationScreen();
        void displaySavingScreen();
};

#endif