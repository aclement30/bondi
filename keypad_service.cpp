#include <ArduinoSTL.h>
#include <Keypad.h>
#include "keypad_service.h"

int KeypadService::waitForSelection() {
    waitForKey = true;
    Serial.println(">");
    
    char keyPressed = keypad.waitForKey();
    waitForKey = false;

    return getKeyCode(keyPressed);
}

int KeypadService::getKeyPressed() {
    char keyPressed = keypad.getKey();
    return getKeyCode(keyPressed);
}

void KeypadService::listenForEscape() {
    attachInterrupt(digitalPinToInterrupt(2), KeypadService::keypadInterrupt, FALLING);
}

void KeypadService::keypadInterrupt() {
    if (KeypadService::getInstance().waitForKey) return;

    int keyCode = KeypadService::getInstance().getKeyPressed();

    if (keyCode == KeypadService::EscapeKey) {
        Serial.println("[ESC]");
        KeypadService::getInstance().escKeyPressed = true;
    }
}

// Non-blocking wait that allows ESC interrupt
void KeypadService::waitForActivity(unsigned long milliseconds) {
    unsigned long startTime = millis();
    unsigned long endTime = startTime + milliseconds;

    Serial.println("(wait)");
    
    char keyPressed = keypad.getKey();
    while (millis() < endTime && !escKeyPressed && keyPressed == NO_KEY) {
        keyPressed = keypad.getKey();
    }
}

int KeypadService::getKeyCode(char keyPressed) {
    switch(keyPressed) {
        case '#':
            return KeypadService::EnterKey;
            break;
        case '*':
            return KeypadService::EscapeKey;
            break;
        case 'A':
            return KeypadService::ArrowUpKey;
            break;
        case 'B':
            return KeypadService::ArrowDownKey;
            break;
        case 'C':
        case 'D':
            // Unused keys
            return 100;
            break;
        default:
            return keyPressed - 48;
    }
}

bool KeypadService::waitForConfirmation() {
    waitForKey = true;
    Serial.println(">");
    
    char keyPressed = keypad.waitForKey();
    waitForKey = false;
    
    Serial.println(keyPressed);

    if (keyPressed == KeypadService::EnterKeyChar) {
        return true;
    } else if (keyPressed == KeypadService::EscapeKeyChar) {
        return false;
    } else {
        return waitForConfirmation();
    }
}

bool KeypadService::isEscapeKeyPressed() {
    return escKeyPressed;
}

void KeypadService::resetEscapeKey() {
    escKeyPressed = false;
}

int KeypadService::EnterKey = 13;
int KeypadService::EscapeKey = 27;
int KeypadService::ArrowUpKey = 33;
int KeypadService::ArrowDownKey = 34;
char KeypadService::EnterKeyChar = '#';
char KeypadService::EscapeKeyChar = '*';

/*keymap defines the key pressed according to the row and columns just as appears on the keypad*/
char KeypadService::keymap[numRows][numCols] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

//Code that shows the the keypad connections to the arduino terminals
byte KeypadService::rowPins[numRows] = {23, 25, 27, 2}; //Rows 0 to 3
byte KeypadService::colPins[numCols] = {3, 29, 31, 33}; //Columns 0 to 3
