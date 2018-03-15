#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <vector>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#ifndef SIMULATOR_H
#define SIMULATOR_H

#define HIGH 0x1
#define LOW 0x0
#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define A0 97
#define A1 96
#define A2 95
#define A3 94
#define A4 93
#define A5 92
#define A6 91
#define A7 90
#define A8 89
#define A9 88
#define A10 87
#define A11 86
#define A12 85
#define A13 84
#define A14 83
#define A15 82

void pinMode(uint8_t pinNumber, uint8_t mode);
void digitalWrite(uint8_t pinNumber, uint8_t value);
int digitalRead(uint8_t pinNumber);
void analogWrite(uint8_t pinNumber, uint8_t value);
void delay(unsigned long microseconds);
int hour() { return 0; };
int minute() { return 0; };

class Serial_ {
    public:
        Serial_() {};
        void begin(unsigned long);
        void end(void);
        bool available() { return false; }
        uint8_t read() { return 1; }

        // size_t print(const __FlashStringHelper *);
        void print(const std::string &) {}
        void print(const char[]) {}
        void print(char) {}
        // size_t print(unsigned char, int = DEC);
        // size_t print(int, int = DEC);
        // size_t print(unsigned int, int = DEC);
        // size_t print(long, int = DEC);
        // size_t print(unsigned long, int = DEC);
        void print(double, int = 2) {}
        // size_t print(const Printable&);

        // size_t println(const __FlashStringHelper *);
        void println(const std::string &s) {}
        void println(const char[]) {}
        void println(char) {}
        // size_t println(unsigned char, int = DEC);
        // size_t println(int, int = DEC);
        // size_t println(unsigned int, int = DEC);
        // size_t println(long, int = DEC);
        // size_t println(unsigned long, int = DEC);
        // size_t println(double, int = 2);
        // size_t println(const Printable&);
        // size_t println(void);
};

Serial_ Serial = Serial_();

class RFID {
    public:
        int serNum[5];

        RFID(
            int rsaPin,
            int rstPin
        ) {}

        void init() {}
        void halt() {}

        bool isCard() {
            currentPointId = getCurrentPointId();
            return currentPointId != 0;
        }

        bool readCardSerial() {
            // if (currentPointId != 0) {
            //     RailPoint railpoint = getRailPointById(locationService.railPoints, currentPointId);

            //     // Split RFID UID into serial number array
            //     std::string number;
            //     std::string::size_type size;
            //     int i = 0;
            //     std::istringstream tokenStream(railpoint.rfidUid);
            //     while (std::getline(tokenStream, number, '.')) {
            //         serNum[i] = std::stoi(number, &size);
            //         i++;
            //     }
                
            //     return true;
            // }

            return false;
        }
    
    private:
        int currentPointId = 0;

        int getCurrentPointId() {
            return EM_ASM_INT({
                console.log('getCurrentRailPoint()');
                const currentPoint = window.simulator && window.simulator.service.getCurrentRailPoint();
                if (currentPoint) {
                    return currentPoint.pointId;
                }
                return 0;
            });
        }
};

class SPI_ {
    public:
        void begin() {}
};

SPI_ SPI = SPI_();

class SD_ {
    public:
        bool begin(int chipSelect) {
            return true;
        }
};

SD_ SD = SD_();

typedef enum { POSITIVE, NEGATIVE } t_backlighPol;

#define LCD_8BITMODE            0x10
#define LCD_4BITMODE            0x00
#define LCD_2LINE               0x08
#define LCD_1LINE               0x00
#define LCD_5x10DOTS            0x04
#define LCD_5x8DOTS             0x00

class LiquidCrystal_I2C {
    public:

        LiquidCrystal_I2C(uint8_t lcd_Addr, uint8_t En, uint8_t Rw, uint8_t Rs,
                        uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
                        uint8_t backlighPin, t_backlighPol pol) {}

    void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS) {}
    void send(uint8_t value, uint8_t mode) {}
    void setBacklight ( uint8_t value ) {}
    void setCursor(uint8_t col, uint8_t row) {}
    void createChar(uint8_t location, uint8_t charmap[]) {}
    void clear() {}
    void home() {}
    void backlight() {}
    void noBacklight() {}
    void print(const std::string &) {}
    void print(const char[]) {}
    void print(char) {}
};

#endif