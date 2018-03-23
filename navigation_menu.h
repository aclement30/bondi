#include "list_menu.h"

#ifndef NAVIGATION_MENU_H
#define NAVIGATION_MENU_H

class NavigationMenu : public ListMenu {
    protected:
        string createMenuItem(string & optionText, int index);
};

#endif
