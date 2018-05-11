#include <ArduinoSTL.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#ifndef DISPLAY_SERVICE_H
#define DISPLAY_SERVICE_H

const char BORDER_LINE[21] = "--------------------";

const uint8_t charBitmap[][8] = {
    // Arrow up
    { 
        0x00,
        0x04,
        0x0E,
        0x15,
        0x04,
        0x04,
        0x04,
        0x04
    },
    // Arrow down
    {
        0x00,
        0x04,
        0x04,
        0x04,
        0x04,
        0x15,
        0x0E,
        0x04
    },
    // Checkmark
    {
        0x00,
        0x00,
        0x01,
        0x02,
        0x14,
        0x08,
        0x00,
        0x00
    }
};

enum ArrowType {
  ARROW_UP,
  ARROW_DOWN
};

using namespace std;

class DisplayService {
    public:
        static DisplayService& getInstance();
        
        void printTitle(string title);
        void printCenter(string text, int row);
        void print(string text, int row);
        void print(const char *text, int row);
        void write(char letter);
        void addBorder(int row = 1);
        void addScrollArrow(int row, ArrowType type);
        void clearScreen();
        void showErrorScreen(vector<string> &message, string okButtonText);
        void showWarningScreen(vector<string> &message, string okButtonText);
        static string getCenteredText(string &text);
        static int getLeftPadding(string text);
        void clearLine(int row);
        void showCursor(int row, int column = 0);
        void hideCursor();

    private:
        DisplayService();

        // C++ 03
        // ========
        // Don't forget to declare these two. You want to make sure they
        // are unacceptable otherwise you may accidentally get copies of
        // your singleton appearing.
        // DisplayService(DisplayService const&);   // Don't Implement
        void operator = (DisplayService const&); // Don't implement

        // C++ 11
        // =======
        // We can use the better technique of deleting the methods
        // we don't want.

        LiquidCrystal_I2C lcd;
};

#endif
