#include <DS1302RTC.h>
#include <Time.h>
#include "constants.h"

#ifndef TIME_SERVICE_H
#define TIME_SERVICE_H

class TimeService {
    public:
        static TimeService & getInstance();
        time_t getTime();
        time_t getLocalTimeFromUTC(time_t utcTime);
        void getUTCDateTime(char * date);
        void getLocalDateTime(char * date);
        int getLocalMoment();
        void timestamp(char * date);
        void setDateTime(int hours, int minutes, int seconds, int days, int months, int years);

    private:
        DS1302RTC rtc;

        TimeService();

        signed long getTimezoneOffset(time_t currentTime);
        signed long getTimezoneOffset();

        void operator = (TimeService const&); // Don't implement
};

#endif
