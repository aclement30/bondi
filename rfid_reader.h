#ifndef __EMSCRIPTEN__
#include <ArduinoSTL.h>
#include <string.h>
#include <sstream>
#include <SPI.h>
#include <RFID.h>
#endif

#ifndef RFIDREADER_H
#define RFIDREADER_H

class RfidReader {
    public:
        RfidReader(
            int rsaPin,
            int rstPin
        ) : 
            rfid(RFID(rsaPin, rstPin))
        {}
    
        void setup() {
            // Init SPI bus
            SPI.begin();
            rfid.init();
        }

        const char* read() {
            if (rfid.isCard()) {  
                if (rfid.readCardSerial()) {
                    //Serial.print("L'UID est: ");
                    std::stringstream charUid;

                    for(int n = 0; n < 5; n++) {
                        charUid << rfid.serNum[n] << ".";
                    }

                    const std::string& tmpUid = charUid.str();   
                    const char* cstr = tmpUid.c_str();
                    
                    // String message = "RFID point detected: ";
                    // String uid = charUid.str();
                    // Serial.println(String(message + uid));

                    return cstr;
                }

                rfid.halt();
            }

            return "";
        }

    private:
        RFID rfid;
};

#endif
