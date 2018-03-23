#include "list_menu.h"
#include "navigation_menu.h"

string NavigationMenu::createMenuItem(string & optionText, int index) {
    char menuOption[20];
    sprintf(menuOption, "[%d] %s", (index + 1), optionText.c_str());
    return string(menuOption);
}