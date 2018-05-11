#include "display_service.h"
#include "keypad_service.h"
#include "navigation_menu.h"
#include "state_manager.h"
#include "string.h"
#include "time_service.h"
#include "time_setting_controller.h"

using namespace std;

void TimeSettingController::handle() {
    displayConfigurationScreen();
}

void TimeSettingController::escape() {
    DisplayService::getInstance().hideCursor();
    StateManager::getInstance().changeState(SettingsMenu);
    return;
}

// PRIVATE

void TimeSettingController::displayConfigurationScreen() {
    const static char title[] PROGMEM = "DATE & HEURE";

    char line[20] = "> ";
    char dateTime[17];
    TimeService::getInstance().getLocalDateTime(dateTime);
    strcat(line, dateTime);

    DisplayService::getInstance().clearScreen();
    DisplayService::getInstance().printTitle(getString(title));
    DisplayService::getInstance().addBorder();
    DisplayService::getInstance().print(line, 2);

    char stringYear[5] = "";
    char stringMonth[3] = "";
    char stringDay[3] = "";
    char stringHour[3] = "";
    char stringMinute[3] = "";

    DisplayService::getInstance().showCursor(2, 2);
    KeypadService::getInstance().queryNumericInput(stringYear, 4);
    if (strlen(stringYear) < 4) {
        escape();
        return;
    }

    DisplayService::getInstance().showCursor(2, 7);
    KeypadService::getInstance().queryNumericInput(stringMonth, 2);
    if (strlen(stringMonth) < 2) {
        escape();
        return;
    }

    DisplayService::getInstance().showCursor(2, 10);
    KeypadService::getInstance().queryNumericInput(stringDay, 2);
    if (strlen(stringDay) < 2) {
        escape();
        return;
    }

    DisplayService::getInstance().showCursor(2, 13);
    KeypadService::getInstance().queryNumericInput(stringHour, 2);
    if (strlen(stringHour) < 2) {
        escape();
        return;
    }

    DisplayService::getInstance().showCursor(2, 16);
    KeypadService::getInstance().queryNumericInput(stringMinute, 2);
    if (strlen(stringMinute) < 2) {
        escape();
        return;
    }

    DisplayService::getInstance().hideCursor();

    // Validate input
    int years = atoi(stringYear);
    if (years < 2018 || years > 2050) {
        years = 2018;
    }

    int months = atoi(stringMonth);
    if (months < 1 || months > 12) {
        months = 1;
    }

    int days = atoi(stringDay);
    if (days < 1 || days > 31) {
        days = 1;
    }
    
    int hours = atoi(stringHour);
    if (hours < 0 || hours > 23) {
        hours = 0;
    }

    int minutes = atoi(stringMinute);
    if (minutes < 0 || minutes > 59) {
        minutes = 0;
    }

    TimeService::getInstance().setDateTime(hours, minutes, 0, days, months, years);

    displaySavingScreen();
}


void TimeSettingController::displaySavingScreen() {
    const static char title[] PROGMEM = "DATE & HEURE";
    const static char message1[] PROGMEM = "Enregistrement";
    const static char message2[] PROGMEM = "en cours";

    DisplayService::getInstance().clearScreen();
    DisplayService::getInstance().printTitle(getString(title));
    DisplayService::getInstance().addBorder();
    DisplayService::getInstance().printCenter(getString(message1), 2);
    DisplayService::getInstance().printCenter(getString(message2), 3);

    KeypadService::getInstance().waitForActivity(1000);

    displayConfigurationScreen();
}

