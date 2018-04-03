#include <ArduinoSTL.h>
#include <string.h>
#include <SPI.h>
#include <RFID.h>
#include "config.h"
#include "constants.h"
#include "rail_point.h"
#include "state_manager.h"
#include "location_service.h"

using namespace std;

static LocationService & LocationService::getInstance() {
    static LocationService instance;    // Guaranteed to be destroyed.
                                        // Instantiated on first use.
    return instance;
}

void LocationService::setConfig(vector<RailPoint> & railPointsRef) {
    railPoints = railPointsRef;
}

void LocationService::setup() {
    LocationService::rfid.init();
}

bool LocationService::isDocked() {
    MovingDirection movingDirection = StateManager::getInstance().getMovingDirection();

    if (movingDirection != MOVING_IDLE) {
        Serial.println(F("[!] feeder still moving"));
        return false;
    }

    char uid[20];
    bool hasRfidUid = readRfidUid(uid);

    if (!hasRfidUid) {
        Serial.println(F("[!] no RFID uid detected"));
        return false;
    }
    
    // Find matching rail point for RFID uid
    int pointIndex = getRailPointIndexFromRfid(AppConfig::getInstance().railPoints, uid, movingDirection);
    if (pointIndex == -1) {
        Serial.println(F("[!] no corresponding rail point"));
        return false;
    }

    return AppConfig::getInstance().railPoints.at(pointIndex).isDock();
}

// Read RFID tag and copy it in *uid / returns TRUE if a tag is found
bool LocationService::readRfidUid(char * uid) {
    Serial.println(F("* detecting RFID point"));

    if (rfid.isCard()) {  
        Serial.println(F("* RFID card detected"));
        if (rfid.readCardSerial()) {
            Serial.println(F("* reading RFID card serial"));
            sprintf(uid, "%d.%d.%d.%d.%d", rfid.serNum[0], rfid.serNum[1], rfid.serNum[2], rfid.serNum[3], rfid.serNum[4]);
            
            return true;
        }

        rfid.halt();
    }

    return false;
}

// PRIVATE

LocationService::LocationService() : rfid(RFID(RFID_RSA_PIN, RFID_RST_PIN)) {}