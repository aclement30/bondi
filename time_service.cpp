#include <DS1302RTC.h>
#include <Time.h>
#include "constants.h"
#include "config.h"
#include "string.h"
#include "time_service.h"

TimeService & TimeService::getInstance() {
    static TimeService instance;    // Guaranteed to be destroyed.
                                    // Instantiated on first use.
    return instance;
}

// Returns current UTC time
time_t TimeService::getTime() {
    return rtc.get();
}

void TimeService::getUTCDateTime(char * date) {
    sprintf(date, "%d-%02d-%02d %02d:%02d:%02d", year(), month(), day(), hour(), minute(), second());
}

void TimeService::getLocalDateTime(char * date) {
    unsigned int timezoneOffset = getTimezoneOffset() * 3600;
    time_t utcTime = now();
    time_t localTime = utcTime + timezoneOffset;

    sprintf(date, "%d-%02d-%02d %02d:%02d:%02d", year(localTime), month(localTime), day(localTime), hour(localTime), minute(localTime), second(localTime));
}

int TimeService::getLocalMoment() {
    unsigned int timezoneOffset = getTimezoneOffset() * 3600;
    time_t utcTime = now();
    time_t localTime = utcTime + timezoneOffset;

    return (hour(localTime) * 60) + minute(localTime);
}

void TimeService::timestamp(char * date) {
    sprintf(date, "%d-%02d-%02dT%02d:%02d:%02dZ", year(), month(), day(), hour(), minute(), second());
}

// PRIVATE

TimeService::TimeService() : rtc(DS1302RTC(RTC_CE_PIN, RTC_IO_PIN, RTC_SCLK_PIN)) {
    setSyncProvider(rtc.get);

    // time_t t = 1525028530;
    // rtc.set(t);   // set the RTC and the system time to the received value
    // setTime(t);
}

unsigned int TimeService::getTimezoneOffset() {
    time_t utcTime = now();

    for (int n = 0; n < 20; n++) {
        if (TIMEZONE_OFFSETS[n][0] > utcTime) {
            return TIMEZONE_OFFSETS[n-1][1];
            break;
        }
    }

    return 0;
}