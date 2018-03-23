#include <ArduinoSTL.h>
#include "display_service.h"
#include "keypad_service.h"
#include "list_menu.h"

using namespace std;
using MenuScreen = vector<string>;

void ListMenu::build(string title, vector<string> & listItems, string escapeText) {
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

void ListMenu::show() {
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

int ListMenu::waitForSelection() {
    int keyPressed = KeypadService::getInstance().waitForSelection();

    int minOption = 1;
    int maxOption = minOption + itemsCount;
    
    // Accepted keys:
    // 1-9
    // A: (prev)
    // B: (next)
    // *: (ESC)
    
    if (keyPressed == KeypadService::ArrowDownKey) {
        nextScreen();
        return waitForSelection();
    } else if (keyPressed == KeypadService::ArrowUpKey) {
        previousScreen();
        return waitForSelection();
    } else if (escapable && keyPressed == KeypadService::EscapeKey) {
        return -1;
    } else if (keyPressed >= minOption && keyPressed <= maxOption) {
        return keyPressed;
    } else {
        show();
        return waitForSelection();
    }
}

// PRIVATE

string ListMenu::createMenuItem(string & itemText, int index) {
    return itemText;
}

void ListMenu::nextScreen() {
    if (screens.size() > 1 && currentScreenIndex < screens.size() - 1) {
        currentScreenIndex += 1;
    }

    show();
}

void ListMenu::previousScreen() {
    if (screens.size() > 1 && currentScreenIndex > 0) {
        currentScreenIndex -= 1;
    }

    show();
}
