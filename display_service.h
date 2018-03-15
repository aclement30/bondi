#ifndef __EMSCRIPTEN__
#include <ArduinoSTL.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#endif

#ifndef DISPLAYSERVICE_H
#define DISPLAYSERVICE_H

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
        static DisplayService& getInstance() {
            static DisplayService instance; // Guaranteed to be destroyed.
                                            // Instantiated on first use.
            return instance;
        }

        void printTitle(string title) {
            printCenter(title, 0);
        }

        void printCenter(string text, int row) {
            string centeredText = getCenteredText(text);
            print(centeredText, row);
        }

        void print(string text, int row) {
            clearLine(row);
            
            lcd.setCursor(0, row);
            lcd.print(text.c_str());
        }

        void print(const char *text, int row) {
            clearLine(row);
            
            lcd.setCursor(0, row);
            lcd.print(text);
        }

        void addBorder(int row = 1) {
            lcd.setCursor(0, row);
            lcd.print(BORDER_LINE);
        }

        void addScrollArrow(int row, ArrowType type) {
            lcd.setCursor(19, row);
            lcd.print(type == ARROW_UP ? char(0) : char(1));
        }
        
        void clearScreen() {
            lcd.clear();
        }

        void showErrorScreen(vector<string> &message, string okButtonText) {
            clearScreen();
            printCenter("ERREUR", 0);
            print(message.at(0), 1);
            if (message.size() > 1) {
                print(message.at(1), 2);
            }
            printCenter(okButtonText + " [F1]", 3);
        }

        static string getCenteredText(string &text) {
            string centeredText;
            centeredText.insert(centeredText.begin(), DisplayService::getLeftPadding(text), ' ');
            centeredText.append(text);
            return centeredText;
        }

        static int getLeftPadding(string text) {
            int length = text.length();
            int padding = 20 - length;
            if (padding % 2 != 0) {
                return (padding - 1) / 2;
            }
            return padding / 2;
        }

    private:
        DisplayService() :
            lcd(LiquidCrystal_I2C(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE))
        {
            lcd.begin(20, 4);

            int charBitmapSize = (sizeof(charBitmap ) / sizeof (charBitmap[0]));
            for (int i = 0; i < charBitmapSize; i++) {
                lcd.createChar(i, (uint8_t *) charBitmap[i]);
            }

            lcd.backlight();
        }

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

        void clearLine(int row) {
            lcd.setCursor(0, row);
            lcd.print("                    ");
        }
};

#endif
