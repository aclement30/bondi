#include <DS1302RTC.h>
#include <Time.h>
#include "constants.h"

#ifndef TIME_SERVICE_H
#define TIME_SERVICE_H

class TimeService {
    public:
        static TimeService & getInstance();
        time_t getTime();
        void getUTCDateTime(char * date);
        void getLocalDateTime(char * date);
        int getLocalMoment();
        void timestamp(char * date);

    private:
        DS1302RTC rtc;

        TimeService();

        unsigned int getTimezoneOffset();

        void operator = (TimeService const&); // Don't implement
};

#endif
