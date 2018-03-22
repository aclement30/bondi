#include <ArduinoSTL.h>

#include "display_service.h"
#include "keypad_service.h"
#include "list_menu.h"

#ifndef NAVIGATIONMENU_H
#define NAVIGATIONMENU_H

using namespace std;

class NavigationMenu : public ListMenu {
    protected:
        string createMenuItem(string & optionText, int index) {
            char menuOption[20];
            sprintf(menuOption, "[%d] %s", (index + 1), optionText.c_str());
            return string(menuOption);
        }
};

#endif
