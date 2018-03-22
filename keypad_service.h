#include <ArduinoSTL.h>
#include <Keypad.h>

#ifndef KEYPAD_SERVICE_H
#define KEYPAD_SERVICE_H

using namespace std;

const byte numRows = 4; //number of rows on the keypad
const byte numCols = 4; //number of columns on the keypad

/*keymap defines the key pressed according to the row and columns just as appears on the keypad*/
char keymap[numRows][numCols] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

//Code that shows the the keypad connections to the arduino terminals
byte rowPins[numRows] = {23, 25, 27, 2}; //Rows 0 to 3
byte colPins[numCols] = {3, 29, 31, 33}; //Columns 0 to 3

class KeypadService {
    public:
        static KeypadService& getInstance() {
            static KeypadService instance; // Guaranteed to be destroyed.
                                            // Instantiated on first use.
            return instance;
        }

        static int EnterKey;
        static int EscapeKey;
        static int ArrowUpKey;
        static int ArrowDownKey;

        static char EnterKeyChar;
        static char EscapeKeyChar;

        int waitForSelection() {
            waitForKey = true;
            Serial.println(">");
            
            char keyPressed = keypad.waitForKey();
            waitForKey = false;

            return getKeyCode(keyPressed);
        }

        int getKeyPressed() {
            char keyPressed = keypad.getKey();
            return getKeyCode(keyPressed);
        }

        void listenForEscape() {
            attachInterrupt(digitalPinToInterrupt(2), KeypadService::keypadInterrupt, FALLING);
        }

        static void keypadInterrupt() {
            if (KeypadService::getInstance().waitForKey) return;

            int keyCode = KeypadService::getInstance().getKeyPressed();

            if (keyCode == KeypadService::EscapeKey) {
                Serial.println("[ESC]");
                KeypadService::getInstance().escKeyPressed = true;
            }
        }

        // Non-blocking wait that allows ESC interrupt
        void waitForActivity(unsigned long milliseconds) {
            unsigned long startTime = millis();
            unsigned long endTime = startTime + milliseconds;

            Serial.println("(wait)");
            
            char keyPressed = keypad.getKey();
            while (millis() < endTime && !escKeyPressed && keyPressed == NO_KEY) {
                keyPressed = keypad.getKey();
            }
        }

        int getKeyCode(char keyPressed) {
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
        
        bool waitForConfirmation() {
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

        bool isEscapeKeyPressed() {
            return escKeyPressed;
        }

        void resetEscapeKey() {
            escKeyPressed = false;
        }

    private:
        bool waitForKey = false;
        bool escKeyPressed = false;

        KeypadService() :
            keypad(Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols))
        {}

        // C++ 03
        // ========
        // Don't forget to declare these two. You want to make sure they
        // are unacceptable otherwise you may accidentally get copies of
        // your singleton appearing.
        // DisplayService(DisplayService const&);   // Don't Implement
        void operator = (KeypadService const&); // Don't implement

        Keypad keypad;
};

int KeypadService::EnterKey = 13;
int KeypadService::EscapeKey = 27;
int KeypadService::ArrowUpKey = 33;
int KeypadService::ArrowDownKey = 34;
char KeypadService::EnterKeyChar = '#';
char KeypadService::EscapeKeyChar = '*';
#endif
