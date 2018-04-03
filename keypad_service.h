#include <ArduinoSTL.h>
#include <Keypad.h>

#ifndef KEYPAD_SERVICE_H
#define KEYPAD_SERVICE_H

using namespace std;

const byte numRows = 4; //number of rows on the keypad
const byte numCols = 4; //number of columns on the keypad

class KeypadService {
    public:        
        static int EnterKey;
        static int EscapeKey;
        static int ArrowUpKey;
        static int ArrowDownKey;

        static char EnterKeyChar;
        static char EscapeKeyChar;

        static KeypadService & getInstance();
        static void keypadInterrupt();

        int waitForSelection();
        int getKeyPressed();
        void listenForEscape();
        void waitForActivity(unsigned long milliseconds);
        int getKeyCode(char keyPressed);
        bool waitForConfirmation();
        bool isEscapeKeyPressed();
        void resetEscapeKey();

    private:
        bool waitForKey = false;
        bool escKeyPressed = false;
        static byte rowPins[numRows];
        static byte colPins[numRows];
        static char keymap[numRows][numCols];
        
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

#endif
