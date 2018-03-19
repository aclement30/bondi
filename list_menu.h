#ifndef __EMSCRIPTEN__
#include <ArduinoSTL.h>
#endif

#include "display_service.h"

#ifndef LISTMENU_H
#define LISTMENU_H

using namespace std;
using MenuScreen = vector<string>;

class ListMenu {
    public:
        bool escapable = true;

        void build(
            string title, 
            vector<string> & listItems, 
            string escapeText = "Revenir"
        ) {
            itemsCount = listItems.size();

            int linesCount = itemsCount + 2; // Lines count includes title, border
            bool showEscapeLine = escapable && linesCount % 4 != 0;
            if (escapable && linesCount % 4 != 0) {
                linesCount += 1;
            }
            int screensCount = (linesCount + 4 - 1) / 4;

            string centeredTitle = DisplayService::getCenteredText(title);
            vector<string>::iterator itemItr = listItems.begin();
            
            for (unsigned int s = 0; s < screensCount; s++) {
                MenuScreen screen;

                // Add title & border in 1st screen
                if (s == 0) {
                  screen.push_back(centeredTitle);
                  screen.push_back(BORDER_LINE);
                }

                int screenLines = 4 - screen.size();
                for (unsigned int n = 0; n < screenLines; n++) {
                    if (itemItr != listItems.end()) {
                        int index = itemItr - listItems.begin();
                        string itemText = *itemItr;
                        string menuItem = createMenuItem(itemText, index);
                        screen.push_back(menuItem);
                        advance(itemItr, 1);
                    } else {
                        if (showEscapeLine && n == 3) {
                          // Add EXIT text on 4th line of last screen
                          string escapeLine = " -- " + escapeText + " [F4] -- ";
                          screen.push_back(escapeLine);
                        } else {
                          screen.push_back("");
                        }
                    }
                }

                screens.push_back(screen);
            }
        }

        void show() {
            MenuScreen currentScreen = screens.at(currentScreenIndex);

            DisplayService::getInstance().clearScreen();

            int row = 0;
             for (const auto& screenLine : currentScreen) {  
                DisplayService::getInstance().print(screenLine.c_str(), row);
                row += 1;
            }

            if (screens.size() > 1) {
                if (currentScreenIndex == 0) {
                    DisplayService::getInstance().addScrollArrow(3, ARROW_DOWN);
                } else {
                    if (currentScreenIndex < screens.size() - 1) {
                        DisplayService::getInstance().addScrollArrow(3, ARROW_DOWN);
                    }

                    DisplayService::getInstance().addScrollArrow(0, ARROW_UP);
                }
            }
        }

        int waitForSelection() {
            ApplicationMonitor.DisableWatchdog();
            Serial.println("<<<");

            while (!Serial.available()) {}

            ApplicationMonitor.EnableWatchdog(Watchdog::CApplicationMonitor::Timeout_4s);
            
            // read the incoming byte:
            int keyCode = Serial.read();

            int minOption = 49;
            int maxOption = minOption + itemsCount;
            
            // Accepted keys:
            // 49-57: #1-9
            // 43: + (next)
            // 45: - (prev)
            // 47: / (ESC)

            if (keyCode == 43) {
              nextScreen();
              return waitForSelection();
            } else if (keyCode == 45) {
              previousScreen();
              return waitForSelection();
            } else if (escapable && keyCode == 47) {
              return -1;
            } else if (keyCode >= minOption && keyCode <= maxOption) {
              return keyCode - 48;
            } else {
              show();
              return waitForSelection();
            }
        }

    protected:
        vector<MenuScreen> screens;
        int currentScreenIndex = 0;
        int itemsCount = 0;

        virtual string createMenuItem(string & itemText, int index) {
            return itemText;
        }

        void nextScreen() {
            if (screens.size() > 1 && currentScreenIndex < screens.size() - 1) {
                currentScreenIndex += 1;
            }

            show();
        }

        void previousScreen() {
            if (screens.size() > 1 && currentScreenIndex > 0) {
                currentScreenIndex -= 1;
            }

            show();
        }

};

#endif
