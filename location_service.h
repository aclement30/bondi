#include <RFID.h>

#include "rail_point.h"
#include "route.h"

#ifndef LOCATION_SERVICE_H
#define LOCATION_SERVICE_H

class LocationService {
    public:
        static LocationService & getInstance();
        void setup();
        bool isDocked();
        bool readRfidUid(char * uid);

    private:
        LocationService();

        RFID rfid;

        void operator = (LocationService const&); // Don't implement
};

#endif
