#include <ArduinoSTL.h>

#ifndef LIST_MENU_H
#define LIST_MENU_H

using namespace std;
using MenuScreen = vector<string>;

class ListMenu {
    public:
        bool escapable = true;

        void build(string title, vector<string> & listItems, string escapeText = "Revenir");
        void show();
        int waitForSelection();

    protected:
        vector<MenuScreen> screens;
        int currentScreenIndex = 0;
        int itemsCount = 0;

        virtual string createMenuItem(string & itemText, int index);
        void nextScreen();
        void previousScreen();
};

#endif
