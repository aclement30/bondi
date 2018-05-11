#include <ArduinoSTL.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "display_service.h"
#include "string.h"

using namespace std;

DisplayService & DisplayService::getInstance() {
    static DisplayService instance; // Guaranteed to be destroyed.
                                    // Instantiated on first use.
    return instance;
}

void DisplayService::printTitle(string title) {
    printCenter(title, 0);
}

void DisplayService::printCenter(string text, int row) {
    string centeredText = getCenteredText(text);
    print(centeredText, row);
}

void DisplayService::print(string text, int row) {
    clearLine(row);
    
    lcd.setCursor(0, row);
    lcd.print(text.c_str());
}

void DisplayService::print(const char *text, int row) {
    clearLine(row);
    
    lcd.setCursor(0, row);
    lcd.print(text);
}

void DisplayService::write(char letter) {
    lcd.write(letter);
}

void DisplayService::addBorder(int row) {
    lcd.setCursor(0, row);
    lcd.print(BORDER_LINE);
}

void DisplayService::addScrollArrow(int row, ArrowType type) {
    lcd.setCursor(19, row);
    lcd.print(type == ARROW_UP ? char(0) : char(1));
}

void DisplayService::clearScreen() {
    lcd.clear();
}

void DisplayService::showErrorScreen(vector<string> &message, string okButtonText) {
    clearScreen();
    printCenter("ERREUR", 0);
    print(message.at(0), 1);
    if (message.size() > 1) {
        print(message.at(1), 2);
    }
    if (okButtonText.length() > 0) {
        printCenter(okButtonText + " [#]", 3);
    }
}

void DisplayService::showWarningScreen(vector<string> &message, string okButtonText) {
    clearScreen();
    printCenter("ATTENTION", 0);
    print(message.at(0), 1);
    if (message.size() > 1) {
        print(message.at(1), 2);
    }
    printCenter(okButtonText + " [#]", 3);
}

string DisplayService::getCenteredText(string & text) {
    string centeredText;
    centeredText.insert(centeredText.begin(), DisplayService::getLeftPadding(text), ' ');
    centeredText.append(text);
    return centeredText;
}

int DisplayService::getLeftPadding(string text) {
    int length = text.length();
    int padding = 20 - length;
    if (padding % 2 != 0) {
        return (padding - 1) / 2;
    }
    return padding / 2;
}

void DisplayService::clearLine(int row) {
    lcd.setCursor(0, row);
    lcd.print("                    ");
}

void DisplayService::showCursor(int row, int column) {
    lcd.setCursor(column, row);
    lcd.blink();
}

void DisplayService::hideCursor() {
    lcd.noBlink();
}

// PRIVATE

DisplayService::DisplayService() : lcd(LiquidCrystal_I2C(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE)) {
    lcd.begin(20, 4);

    int charBitmapSize = (sizeof(charBitmap ) / sizeof (charBitmap[0]));
    for (int i = 0; i < charBitmapSize; i++) {
        lcd.createChar(i, (uint8_t *) charBitmap[i]);
    }

    lcd.backlight();
}