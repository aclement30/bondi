#ifndef __EMSCRIPTEN__
#include <ArduinoSTL.h>
#include <string.h>
//#include <sstream>
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
        {
            // Init SPI bus
            SPI.begin();

            rfid.init();
        }
    
        const char* read() {
            if (rfid.isCard()) {  
                if (rfid.readCardSerial()) {
                    Serial.print("L'UID est: ");
                    std::stringstream charUid;

                    for(int n = 0; n < 5; n++) {
                        charUid << rfid.serNum[n] << ".";
                    }

                    return charUid.str();

                    // std::string const UID = std::to_string(rfid.serNum[0]) + "." + 
                    //                         std::to_string(rfid.serNum[1]) + "." + 
                    //                         std::to_string(rfid.serNum[2]) + "." + 
                    //                         std::to_string(rfid.serNum[3]) + "." + 
                    //                         std::to_string(rfid.serNum[4]);

                    // std::vector<char> rfidUid(UID.begin(), UID.end());
                    // rfidUid.push_back('\0');

                    // return rfidUid;
                }

                rfid.halt();
            }

            return "";
        }

    private:
        RFID rfid;
};

#endif