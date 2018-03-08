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

#include "../rail_point.h"

#ifndef SIMULATOR_H
#define SIMULATOR_H

#define HIGH 0x1
#define LOW 0x0
#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

const int A0 = 97;
const int A1 = 96;
const int A2 = 95;
const int A3 = 94;
const int A4 = 93;
const int A5 = 92;
const int A6 = 91;
const int A7 = 90;
const int A8 = 89;
const int A9 = 88;
const int A10 = 87;
const int A11 = 86;
const int A12 = 85;
const int A13 = 84;
const int A14 = 83;
const int A15 = 82;

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
        
        // size_t print(const __FlashStringHelper *);
        size_t print(const std::string &);
        size_t print(const char[]);
        size_t print(char);
        // size_t print(unsigned char, int = DEC);
        // size_t print(int, int = DEC);
        // size_t print(unsigned int, int = DEC);
        // size_t print(long, int = DEC);
        // size_t print(unsigned long, int = DEC);
        size_t print(double, int = 2);
        // size_t print(const Printable&);

        // size_t println(const __FlashStringHelper *);
        size_t println(const std::string &s);
        size_t println(const char[]);
        size_t println(char);
        // size_t println(unsigned char, int = DEC);
        // size_t println(int, int = DEC);
        // size_t println(unsigned int, int = DEC);
        // size_t println(long, int = DEC);
        // size_t println(unsigned long, int = DEC);
        size_t println(double, int = 2);
        // size_t println(const Printable&);
        size_t println(void);
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
                    return currentPoint.id;
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

#endif