#ifndef __EMSCRIPTEN__
#include <ArduinoSTL.h>
#endif

#include "display_service.h"
#include "list_menu.h"

#ifndef NAVIGATIONMENU_H
#define NAVIGATIONMENU_H

using namespace std;

class NavigationMenu : public ListMenu {
    public:
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
        string createMenuItem(string & optionText, int index) {
            char menuOption[20];
            sprintf(menuOption, "[%d] %s", (index + 1), optionText.c_str());
            return string(menuOption);
        }
};

#endif
