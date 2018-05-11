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

time_t TimeService::getLocalTimeFromUTC(time_t utcTime) {
    signed long timezoneOffset = getTimezoneOffset(utcTime) * 3600;
    return utcTime + timezoneOffset;
}

void TimeService::getUTCDateTime(char * date) {
    sprintf(date, "%d-%02d-%02d %02d:%02d:%02d", year(), month(), day(), hour(), minute(), second());
}

void TimeService::getLocalDateTime(char * date) {
    signed long timezoneOffset = getTimezoneOffset() * 3600;
    time_t utcTime = now();
    time_t localTime = utcTime + timezoneOffset;

    sprintf(date, "%d-%02d-%02d %02d:%02d", year(localTime), month(localTime), day(localTime), hour(localTime), minute(localTime));
}

int TimeService::getLocalMoment() {
    signed long timezoneOffset = getTimezoneOffset() * 3600;
    time_t utcTime = now();
    time_t localTime = utcTime + timezoneOffset;

    return (hour(localTime) * 60) + minute(localTime);
}

void TimeService::timestamp(char * date) {
    sprintf(date, "%d-%02d-%02dT%02d:%02d:%02dZ", year(), month(), day(), hour(), minute(), second());
}

void TimeService::setDateTime(int hours, int minutes, int seconds, int days, int months, int years) {
    tmElements_t tm;
    tm.Year = years - 1970;
    tm.Month = months;
    tm.Day = days;
    tm.Hour = hours;
    tm.Minute = minutes;
    tm.Second = seconds;
    
    time_t localTime = makeTime(tm);
    signed long timezoneOffset = getTimezoneOffset(localTime) * 3600;
    time_t utcTime = localTime - timezoneOffset;

    rtc.set(utcTime);
    setTime(utcTime);
}

// PRIVATE

TimeService::TimeService() : rtc(DS1302RTC(RTC_CE_PIN, RTC_IO_PIN, RTC_SCLK_PIN)) {
    setSyncProvider(rtc.get);
}

signed long TimeService::getTimezoneOffset(time_t currentTime) {
    for (int n = 0; n < 20; n++) {
        if (TIMEZONE_OFFSETS[n][0] > currentTime) {
            return TIMEZONE_OFFSETS[n-1][1];
            break;
        }
    }

    return 0;
}

signed long TimeService::getTimezoneOffset() {
    time_t utcTime = now();

    return getTimezoneOffset(utcTime);
}