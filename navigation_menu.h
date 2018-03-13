#ifndef __EMSCRIPTEN__
#include <ArduinoSTL.h>
#endif

#include "display_service.h"

#ifndef NAVIGATIONMENU_H
#define NAVIGATIONMENU_H

using namespace std;
using MenuScreen = vector<string>;

class NavigationMenu {
    public:
        NavigationMenu(
            string title,
            vector<string> &menuOptions,
            string exitText = "Revenir"     // Empty exitText will disable exit button
        ) {
            canExit = exitText.size() > 0;
            optionsCount = menuOptions.size();

            int linesCount = optionsCount + 2; // Lines count includes title, border
            bool showExitLine = canExit && linesCount % 4 != 0;
            if (canExit && linesCount % 4 != 0) {
                linesCount += 1;
            }
            int screensCount = (linesCount + 4 - 1) / 4;
            string centeredTitle = DisplayService::getCenteredText(title);
            vector<string>::iterator optionItr = menuOptions.begin();
            
            for (unsigned int s = 0; s < screensCount; s++) {
                MenuScreen screen;

                // Add title & border in 1st screen
                if (s == 0) {
                  screen.push_back(centeredTitle);
                  screen.push_back(BORDER_LINE);
                }

                int screenLines = 4 - screen.size();
                for (unsigned int n = 0; n < screenLines; n++) {
                    //screen.push_back("line");
                    if (optionItr != menuOptions.end()) {
                        int index = optionItr - menuOptions.begin();
                        string optionText = *optionItr;
                        string menuOption = createMenuOption(optionText, index + 1);
                        screen.push_back(menuOption);
                        advance(optionItr, 1);
                    } else {
                        if (showExitLine && n == 3) {
                          // Add EXIT text on 4th line of last screen
                          string exitLine = " -- " + exitText + " [F4] -- ";
                          screen.push_back(exitLine);
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
                Serial.println(screenLine.c_str());
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
            Serial.println("Choisissez une option de menu");

            while (!Serial.available()) {}

            // read the incoming byte:
            int keyCode = Serial.read();

            int minOption = 49;
            int maxOption = minOption + optionsCount;
            
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
            } else if (canExit && keyCode == 47) {
              Serial.print("ESC");
              return -1;
            } else if (keyCode >= minOption && keyCode <= maxOption) {
              return keyCode - 48;
            } else {
              show();
              return waitForSelection();
            }
        }

        static bool waitForConfirmation() {
            Serial.println("Appuyez sur F1 pour continuer...");

            while (!Serial.available()) {}

            // read the incoming byte:
            int keyCode = Serial.read();

            if (keyCode == 32) {
              return true;
            } else if (keyCode == 47) {
              Serial.print("ESC");
              return false;
            } else {
                return waitForConfirmation();
            }
        }

    private:
        vector<MenuScreen> screens;
        int currentScreenIndex = 0;
        int optionsCount = 0;
        bool canExit = true;

        string createMenuOption(string &optionText, int number) {
            char menuOption[20];
            sprintf(menuOption, "[%d] %s", number, optionText.c_str());
            return string(menuOption);
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
